#include "kitchen_page.h"
#include "ui_kitchen_page.h"
#include "models.h"

#include <QMessageBox>
#include <QListWidget>
#include <QDateTime>
#include <algorithm>

KitchenPage::KitchenPage(StaffMember* staff, QWidget *parent)
    : QWidget(parent),
    ui(new Ui::KitchenPage),
    m_staff(staff) {
    ui->setupUi(this);

    if (m_staff) {
        ui->greetingLabel->setText(QString("Welcome, %1").arg(m_staff->name));
    }

    // Connect button event listeners
    connect(ui->orderQueue, &QListWidget::itemSelectionChanged, this, &KitchenPage::displaySelectedOrder);
    connect(ui->completeOrderButton, &QPushButton::clicked, this, &KitchenPage::completeSelectedOrder);
    connect(ui->logoutButton, &QPushButton::clicked, this, &KitchenPage::logout);

    ui->completeOrderButton->setEnabled(false);
    populateOrderQueue();
}

KitchenPage::~KitchenPage() {
    delete ui;
}

void KitchenPage::populateOrderQueue() {
    ui->orderQueue->clear();
    ui->orderDetailsDisplay->clear();
    m_selectedOrderNumber = -1;
    ui->completeOrderButton->setEnabled(false);

    DataStore& store = DataStore::instance();
    QVector<Order*> activeOrders;

    // Collect all orders that are still in progress.
    for (Order& order : store.orders()) {
        if (order.status == OrderStatus::InProgress) {
            activeOrders.append(&order);
        }
    }

    // Sort by order number so the oldest order appears first.
    std::sort(activeOrders.begin(), activeOrders.end(),
              [](Order* a, Order* b) {
                  return a->orderNumber < b->orderNumber;
              });

    // Add each order to the kitchen queue.
    for (Order* order : activeOrders) {
        QString text = QString("Order #%1 - %2").arg(order->orderNumber).arg(orderStatusText(order->status));
        QListWidgetItem* item = new QListWidgetItem(text);

        item->setData(Qt::UserRole, order->orderNumber);
        ui->orderQueue->addItem(item);
    }

    if (ui->orderQueue->count() == 0) {
        ui->orderDetailsDisplay->setPlainText("There are currently no orders in the queue.");
    }
    else {
        ui->orderQueue->setCurrentRow(0);
    }
}

void KitchenPage::displaySelectedOrder() {
    QListWidgetItem* item = ui->orderQueue->currentItem();

    if (!item) {
        m_selectedOrderNumber = -1;
        ui->completeOrderButton->setEnabled(false);
        ui->orderDetailsDisplay->clear();
        return;
    }

    m_selectedOrderNumber = item->data(Qt::UserRole).toInt();
    DataStore& store = DataStore::instance();
    Order* order = store.orderByNumber(m_selectedOrderNumber);

    if (order) {
        showOrderDetails(order);
        ui->completeOrderButton->setEnabled(order->status == OrderStatus::InProgress);
    }
}

void KitchenPage::showOrderDetails(Order* order) {
    if (!order) {
        return;
    }

    DataStore& store = DataStore::instance();
    Customer* customer = store.customerByUsername(order->customerUsername);
    QString customerName = customer ? customer->name : order->customerUsername;
    QString details;

    details += QString("Order #%1\n").arg(order->orderNumber);
    details += QString("Customer: %1\n").arg(customerName);
    QDateTime orderDateTime = QDateTime::fromString(order->placedAt, Qt::ISODate);

    if (!orderDateTime.isValid()) {
        orderDateTime = QDateTime::fromString(order->placedAt, "yyyy-MM-dd hh:mm:ss");
    }

    QString formattedDateTime = orderDateTime.isValid()
                                    ? orderDateTime.toString("ddd, MMM d, yyyy 'at' h:mm AP")
                                    : order->placedAt;

    details += QString("Date and Time: %1\n").arg(formattedDateTime);
    details += QString("Status: %1\n\n").arg(orderStatusText(order->status));

    details += "ORDER ITEMS\n";
    details += "------------------------------\n";

    for (const OrderLine& line : order->lines) {
        details += QString("%1 x %2\n").arg(line.quantity).arg(line.item.name);
        details += QString("Price: $%1\n").arg(line.item.price * line.quantity, 0, 'f', 2);
        details += "Ingredients:\n";

        for (const IngredientQuantity& ingredient : line.item.ingredients) {
            details += QString("  - %1: %2 %3\n").arg(ingredient.name).arg(ingredient.quantity).arg(ingredient.unit);
        }

        details += "\n";
    }

    ui->orderDetailsDisplay->setPlainText(details);
}

void KitchenPage::completeSelectedOrder() {
    if (m_selectedOrderNumber == -1) {
        return;
    }

    DataStore& store = DataStore::instance();
    Order* order = store.orderByNumber(m_selectedOrderNumber);

    if (!order || order->status != OrderStatus::InProgress) {
        QMessageBox::warning(this, "Order Error", "This order is no longer in progress.");
        populateOrderQueue();
        return;
    }

    // Mark the order as completed.
    order->status = OrderStatus::Completed;

    // Add loyalty points only if none were redeemed.
    if (order->loyaltyPointsUsed == 0 && order->loyaltyPointsEarned == 0) {
        order->loyaltyPointsEarned = static_cast<int>(order->amountCharged / 3.0);

        Customer* customer = store.customerByUsername(order->customerUsername);

        if (customer) {
            customer->loyaltyPoints += order->loyaltyPointsEarned;
        }
    }

    QMessageBox::information(this, "Order Completed", QString("Order #%1 has been completed.").arg(order->orderNumber));

    // Remove the completed order from the active queue.
    populateOrderQueue();
}

void KitchenPage::logout() {
    this->close();
}
