#ifndef CUSTOMER_PAGE_H
#define CUSTOMER_PAGE_H

#include "models.h"

#include <QWidget>
#include <QTimer>

namespace Ui {
class customerPage;
}

class DisplayOrderDialog;

class CustomerPage : public QWidget
{
    Q_OBJECT

public:
    explicit CustomerPage(Customer* customer, QWidget* parent = nullptr);
    ~CustomerPage();

signals:
    void orderPlaced(int orderNumber);
    void orderStatusChanged(int orderNumber, OrderStatus status);

public slots:
    void updateOrderStatus(int orderNumber, OrderStatus status);

private slots:
    void animateCurrentOrderStatus();
    void openCurrentOrder();
    void addSelectedItem();
    void updateTotals();
    void placeOrder();
    void logout();

private:
    void loadCustomerDetails();
    void loadMenu();
    void loadPreviousOrders();
    void updateCartDisplay();
    double currentSubtotal() const;
    int selectedLoyaltyPoints() const;
    double selectedCredit() const;
    void showMessage(const QString& title, const QString& message);
    Order* currentCustomerOrder();
    QTimer* m_statusTimer;
    int m_dotCount;
    void updateCurrentOrderButton();

    Ui::customerPage* ui;
    Customer* m_customer;
    QVector<OrderLine> m_cart;
    DisplayOrderDialog* m_orderDialog;
};

#endif // CUSTOMER_PAGE_H
