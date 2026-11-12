#ifndef LOGIN_PAGE_H
#define LOGIN_PAGE_H

#include <QWidget>

class CustomerPage;
class KitchenPage;
class AdminPage;
class CashierPage;
class ManagerPage;

namespace Ui {
class login_page;
}

class login_page : public QWidget {
    Q_OBJECT

public:
    explicit login_page(QWidget *parent = nullptr);
    ~login_page();

private slots:
    void handleLogin();

private:
    Ui::login_page *ui;
    CustomerPage* m_customerPage = nullptr;
    KitchenPage* m_kitchenPage = nullptr;
    AdminPage* m_adminPage = nullptr;
    CashierPage* m_cashierPage = nullptr;
    ManagerPage* m_managerPage = nullptr;
};

#endif // LOGIN_PAGE_H
