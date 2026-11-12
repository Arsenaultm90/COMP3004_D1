#include "customer_page.h"
#include "ui_customer_page.h"
#include "display_order.h"

#include <QMessageBox>
#include <QListWidgetItem>
#include <QApplication>
#include <QDateTime>
#include <QtMath>
#include <algorithm>

static const double TAX_RATE = 0.15;

CustomerPage::CustomerPage(Customer* customer, QWidget* parent)
    : QWidget(parent),
      ui(new Ui::customerPage),
      m_customer(customer),
      m_orderDialog(nullptr) {
    ui->setupUi(this);

    if (!m_customer) {
        showMessage("Customer Error", "No customer was supplied to the customer dashboard.");
        setEnabled(false);
        return;
    }

    loadCustomerDetails();
    loadMenu();
    loadPreviousOrders();

    ui->comboBox->clear();
    for (int quantity = 1; quantity <= 10; ++quantity)
        ui->comboBox->addItem(QString::number(quantity), quantity);

    ui->loyaltyPointsSpinBox->setMinimum(0);
    ui->loyaltyPointsSpinBox->setMaximum(m_customer->loyaltyPoints);
    ui->creditSpinBox->setMinimum(0.0);
    ui->creditSpinBox->setMaximum(m_customer->accountCredit);
    ui->creditSpinBox->setDecimals(2);
    ui->creditSpinBox->setSingleStep(0.50);

    // Setup for button even listeners
    connect(ui->addItemButton, &QPushButton::clicked, this, &CustomerPage::addSelectedItem);
    connect(ui->orderButton, &QPushButton::clicked, this, &CustomerPage::placeOrder);
    connect(ui->loyaltyPointsSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &CustomerPage::updateTotals);
    connect(ui->creditSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &CustomerPage::updateTotals);
    connect(ui->logoutButton, &QPushButton::clicked, this, &CustomerPage::logout);
    connect(ui->inProgressButton, &QPushButton::clicked, this, &CustomerPage::openCurrentOrder);

    // Setup for blinking in progress dots
    m_dotCount = 0;
    m_statusTimer = new QTimer(this);
    connect(m_statusTimer, &QTimer::timeout,
            this, &CustomerPage::animateCurrentOrderStatus);

    updateTotals();
    updateCurrentOrderButton();
}

CustomerPage::~CustomerPage() {
    delete ui;
}

void CustomerPage::loadCustomerDetails() {
    ui->greetingLabel->setText("Greetings, " + m_customer->name);
    ui->emailLabel->setText(m_customer->email);
    ui->phoneNumberLabel->setText(m_customer->phone);
    ui->addressLabel->setText(m_customer->address);
    ui->loyaltyNumberLabel->setText(m_customer->loyaltyNumber);
    ui->loyaltyPointsLabel->setText(QString::number(m_customer->loyaltyPoints));
    ui->accountCreditLabel->setText(QString("$%1").arg(m_customer->accountCredit, 0, 'f', 2));
}

void CustomerPage::loadMenu() {
    ui->menuList->clear();

    const QVector<MenuItem>& menu =
        DataStore::instance().menuItems();

    QString lastCategory;

    for (const MenuItem& item : menu) {
        // Menu Heading
        if (item.category != lastCategory) {
            QListWidgetItem* heading =
                new QListWidgetItem(item.category.toUpper());

            QFont headingFont;
            headingFont.setBold(true);
            headingFont.setPointSize(12);
            heading->setFont(headingFont);
            heading->setForeground(QColor("#56B8A8"));
            heading->setFlags(Qt::ItemIsEnabled);
            heading->setData(Qt::UserRole, -1);
            heading->setSizeHint(QSize(0, 36));

            ui->menuList->addItem(heading);

            lastCategory = item.category;
        }

        // Menu item
        QListWidgetItem* menuRow =
            new QListWidgetItem(
                QString("%1    $%2")
                    .arg(item.name)
                    .arg(item.price, 0, 'f', 2)
                );

        QFont itemFont;
        itemFont.setPointSize(10);
        menuRow->setFont(itemFont);
        menuRow->setForeground(QColor("#EEF1EF"));
        menuRow->setData(Qt::UserRole, item.id);
        menuRow->setSizeHint(QSize(0, 30));

        ui->menuList->addItem(menuRow);
    }
}

void CustomerPage::loadPreviousOrders() {
    ui->listWidget->clear();

    DataStore& store = DataStore::instance();
    bool foundOrders = false;

    for (const Order& order : store.orders()) {
        if (order.customerUsername != m_customer->username) {
            continue;
        }

        if (order.status != OrderStatus::PickedUp &&
            order.status != OrderStatus::Cancelled) {
            continue;
        }

        foundOrders = true;
        QString status = orderStatusText(order.status);
        QString orderText = QString("Order #%1 | %2 | $%3").arg(order.orderNumber).arg(status).arg(order.finalTotal, 0, 'f', 2);
        ui->listWidget->addItem(orderText);
    }

    if (!foundOrders) {
        ui->listWidget->addItem("No previous orders available.");
    }
}

void CustomerPage::addSelectedItem() {
    QListWidgetItem* selected = ui->menuList->currentItem();
    if (!selected) {
        showMessage("Select an Item", "Please select a menu item first.");
        return;
    }

    const int itemId = selected->data(Qt::UserRole).toInt();
    if (itemId < 0) {
        showMessage("Select an Item", "Please select a menu item, not a category heading.");
        return;
    }

    const QVector<MenuItem>& menu = DataStore::instance().menuItems();
    auto it = std::find_if(menu.begin(), menu.end(), [itemId](const MenuItem& item) { return item.id == itemId; });
    if (it == menu.end())
        return;

    const int quantity = ui->comboBox->currentData().toInt();
    if (quantity < 1)
        return;

    auto cartIt = std::find_if(m_cart.begin(), m_cart.end(), [itemId](const OrderLine& line) {
        return line.item.id == itemId;
    });

    if (cartIt == m_cart.end())
        m_cart.append({*it, quantity});
    else
        cartIt->quantity += quantity;

    updateCartDisplay();
    updateTotals();
}

void CustomerPage::updateCartDisplay() {
    ui->orderDisplay->clear();

    for (const OrderLine& line : m_cart) {
        ui->orderDisplay->addItem(
            QString("%1 x %2    $%3")
                .arg(line.item.name)
                .arg(line.quantity)
                .arg(line.item.price * line.quantity, 0, 'f', 2));
    }
}

double CustomerPage::currentSubtotal() const {
    double subtotal = 0.0;
    for (const OrderLine& line : m_cart)
        subtotal += line.item.price * line.quantity;
    return subtotal;
}

int CustomerPage::selectedLoyaltyPoints() const {
    const double maxRedeemable = currentSubtotal() * 0.50;
    const int maxPoints = qFloor(maxRedeemable);
    return qBound(0, ui->loyaltyPointsSpinBox->value(), qMin(m_customer->loyaltyPoints, maxPoints));
}

double CustomerPage::selectedCredit() const {
    const double totalAfterLoyalty = currentSubtotal() * (1.0 + TAX_RATE) - selectedLoyaltyPoints();
    return qBound(0.0, ui->creditSpinBox->value(), qMin(m_customer->accountCredit, qMax(0.0, totalAfterLoyalty)));
}

void CustomerPage::updateTotals() {
    const double subtotal = currentSubtotal();
    const double tax = subtotal * TAX_RATE;
    const double finalTotal = subtotal + tax;
    const int points = selectedLoyaltyPoints();
    const double credit = selectedCredit();
    const double due = qMax(0.0, finalTotal - points - credit);

    ui->loyaltyPointsSpinBox->setMaximum(qMin(m_customer->loyaltyPoints, qFloor(subtotal * 0.50)));
    ui->creditSpinBox->setMaximum(qMin(m_customer->accountCredit, qMax(0.0, finalTotal - points)));
    ui->subtotalMaount->setText(QString("$%1").arg(subtotal, 0, 'f', 2));
    ui->taxAmount->setText(QString("$%1").arg(tax, 0, 'f', 2));
    ui->finalAmount->setText(QString("$%1").arg(finalTotal, 0, 'f', 2));
    ui->loyaltyAmount->setText(QString("$%1").arg(static_cast<double>(points), 0, 'f', 2));
    ui->creditAmount->setText(QString("$%1").arg(credit, 0, 'f', 2));
    ui->dueAmount->setText(QString("$%1").arg(due, 0, 'f', 2));
}

void CustomerPage::placeOrder() {
    if (!m_customer) return;

    if (m_customer->hasCurrentOrder) {
        showMessage("Current Order Exists",
                    "Only one order can be placed at a time.");
        return;
    }

    if (m_cart.isEmpty()) {
        showMessage("Empty Order", "Please add at least one item before placing your order.");
        return;
    }

    const double subtotal = currentSubtotal();
    const double tax = subtotal * TAX_RATE;
    const double finalTotal = subtotal + tax;
    const int pointsUsed = selectedLoyaltyPoints();
    const double creditUsed = selectedCredit();
    const double amountDue = qMax(0.0, finalTotal - pointsUsed - creditUsed);

    Order order;
    order.orderNumber = DataStore::instance().nextOrderNumber();
    order.customerUsername = m_customer->username;
    order.lines = m_cart;
    order.subtotal = subtotal;
    order.tax = tax;
    order.finalTotal = finalTotal;
    order.loyaltyPointsUsed = pointsUsed;
    order.accountCreditUsed = creditUsed;
    order.amountCharged = amountDue;
    order.status = OrderStatus::InProgress;
    order.paid = true;
    order.placedAt = QDateTime::currentDateTime().toString(Qt::ISODate);
    order.loyaltyPointsEarned = 0;

    DataStore::instance().orders().append(order);

    m_customer->loyaltyPoints -= pointsUsed;
    m_customer->accountCredit -= creditUsed;
    m_customer->hasCurrentOrder = true;

    emit orderPlaced(order.orderNumber);

    m_cart.clear();
    updateCartDisplay();
    ui->loyaltyPointsSpinBox->setValue(0);
    ui->creditSpinBox->setValue(0.0);
    updateTotals();
    loadCustomerDetails();

    if (m_orderDialog)
        m_orderDialog->close();

    m_orderDialog = new DisplayOrderDialog(order, this);
    m_orderDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(m_orderDialog, &QObject::destroyed, this, [this]() {
        m_orderDialog = nullptr;
    });
    m_orderDialog->show();
    updateCurrentOrderButton();
}

void CustomerPage::updateOrderStatus(int orderNumber, OrderStatus status) {
    Order* order = DataStore::instance().orderByNumber(orderNumber);
    if (!order || order->customerUsername != m_customer->username)
        return;

    order->status = status;

    if (status == OrderStatus::Completed && order->loyaltyPointsUsed == 0) {
        order->loyaltyPointsEarned = qFloor(order->amountCharged / 3.0);
        m_customer->loyaltyPoints += order->loyaltyPointsEarned;
    }

    if (status == OrderStatus::PickedUp || status == OrderStatus::Cancelled)
        m_customer->hasCurrentOrder = false;

    loadCustomerDetails();

    if (m_orderDialog && m_orderDialog->isVisible())
        m_orderDialog->setOrderStatus(status);

    updateCurrentOrderButton();
    emit orderStatusChanged(orderNumber, status);
}

void CustomerPage::logout() {
    this->close();
}

void CustomerPage::showMessage(const QString& title, const QString& message) {
    QMessageBox::information(this, title, message);
}

Order* CustomerPage::currentCustomerOrder() {
    auto& orders = DataStore::instance().orders();

    for (auto it = orders.rbegin(); it != orders.rend(); ++it) {
        if (it->customerUsername == m_customer->username &&
            (it->status == OrderStatus::InProgress ||
             it->status == OrderStatus::Completed)) {
            return &(*it);
        }
    }

    return nullptr;
}

void CustomerPage::updateCurrentOrderButton() {
    Order* order = currentCustomerOrder();

    if (!order || order->status != OrderStatus::InProgress) {
        ui->orderLabel->hide();
        ui->statusLabel->hide();
        ui->inProgressButton->hide();

        if (m_statusTimer->isActive()) {
            m_statusTimer->stop();
        }

        return;
    }

    ui->orderLabel->show();
    ui->statusLabel->show();
    ui->inProgressButton->show();

    ui->orderLabel->setText(
        QString("Order #%1").arg(order->orderNumber));

    ui->inProgressButton->setText(
        "Click to view order details");

    if (!m_statusTimer->isActive()) {
        m_statusTimer->start(1000);
    }
}

void CustomerPage::openCurrentOrder() {
    Order* order = currentCustomerOrder();

    if (!order)
        return;

    if (m_orderDialog) {
        m_orderDialog->close();
        m_orderDialog = nullptr;
    }

    m_orderDialog = new DisplayOrderDialog(*order, this);
    m_orderDialog->setAttribute(Qt::WA_DeleteOnClose);

    connect(m_orderDialog, &QObject::destroyed, this, [this]() {
        m_orderDialog = nullptr;
    });

    m_orderDialog->show();
}

void CustomerPage::animateCurrentOrderStatus() {
    Order* order = currentCustomerOrder();

    if (!order || order->status != OrderStatus::InProgress) {
        updateCurrentOrderButton();
        return;
    }

    m_dotCount = (m_dotCount % 3) + 1;

    ui->statusLabel->setText(
        "In Progress" + QString(m_dotCount, '.'));
}
