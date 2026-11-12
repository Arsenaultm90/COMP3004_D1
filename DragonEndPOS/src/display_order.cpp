#include "display_order.h"
#include "ui_display_order.h"

#include <QListWidget>

DisplayOrderDialog::DisplayOrderDialog(const Order& order, QWidget* parent)
    : QDialog(parent),
    ui(new Ui::DisplayOrderDialog),
    m_order(order) {
    ui->setupUi(this);

    refreshStatus();
}

DisplayOrderDialog::~DisplayOrderDialog() {
    delete ui;
}

void DisplayOrderDialog::refreshStatus() {
    // Display order number
    ui->orderNumberLabel->setText(QString("Order #%1").arg(m_order.orderNumber));

    // Display order status
    ui->orderStatusLabel->setText("Status: " + orderStatusText(m_order.status));

    // Clear old items before repopulating
    ui->itemList->clear();

    // Add each order line to the list
    for (const OrderLine& line : m_order.lines) {
        QString itemText = QString("%1 x %2 — $%3").arg(line.quantity).arg(line.item.name).arg(line.item.price * line.quantity, 0, 'f', 2);
        ui->itemList->addItem(itemText);
    }

    // Display order total
    ui->priceLabel->setText(QString("$%1").arg(m_order.finalTotal, 0, 'f', 2));
}

void DisplayOrderDialog::setOrderStatus(OrderStatus status) {
    m_order.status = status;
    refreshStatus();
}
