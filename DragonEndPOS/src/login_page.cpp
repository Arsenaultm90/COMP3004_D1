#include "login_page.h"
#include "ui_login_page.h"

#include "models.h"
#include "customer_page.h"
#include "kitchen_page.h"
#include "admin_page.h"
#include "cashier_page.h"
#include "manager_page.h"

#include <QMessageBox>

login_page::login_page(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::login_page) {
    ui->setupUi(this);

    // Connect Login button
    connect(ui->loginButton, &QPushButton::clicked, this, &login_page::handleLogin);

    // Hide password controls for D1
    ui->passwordLabel->hide();
    ui->passwordInput->hide();
}

login_page::~login_page() {
    delete ui;
}

void login_page::handleLogin() {
    QString username = ui->usernameInput->text().trimmed();

    // Make sure username isn't empty
    if (username.isEmpty()) {
        QMessageBox::warning(
            this,
            "Login Error",
            "Please enter your username."
            );
        return;
    }

    DataStore& store = DataStore::instance();

    // Check whether the username belongs to a customer
    Customer* customer = store.customerByUsername(username);

    if (customer) {
        m_customerPage = new CustomerPage(customer);
        m_customerPage->setWindowTitle("DragonEndPOS - Customer Dashboard");
        m_customerPage->setAttribute(Qt::WA_DeleteOnClose);

        // If the dashboard closes, show login again
        connect(m_customerPage, &QObject::destroyed, this, [this]() {
            m_customerPage = nullptr;
            this->show();
        });

        m_customerPage->show();
        this->hide();

        return;
    }

    // Check whether the username belongs to staff
    StaffMember* staff = store.staffByUsername(username);

    if (staff) {
        QWidget* staffPage = nullptr;

        if (staff->role == UserRole::Administrator) {
            m_adminPage = new AdminPage(staff);
            staffPage = m_adminPage;
        }
        else if (staff->role == UserRole::Cashier) {
            m_cashierPage = new CashierPage(staff);
            staffPage = m_cashierPage;
        }
        else if (staff->role == UserRole::Manager) {
            m_managerPage = new ManagerPage(staff);
            staffPage = m_managerPage;
        }
        else if (staff->role == UserRole::KitchenStaff) {
            m_kitchenPage = new KitchenPage(staff);
            staffPage = m_kitchenPage;
        }

        if (staffPage) {
            staffPage->setAttribute(Qt::WA_DeleteOnClose);

            connect(staffPage, &QObject::destroyed, this, [this]() {
                m_adminPage = nullptr;
                m_cashierPage = nullptr;
                m_managerPage = nullptr;

                ui->usernameInput->clear();
                this->show();
            });

            staffPage->show();
            this->hide();
            return;
        }
    }

    // Username wasn't found
    QMessageBox::warning(this, "Login Error", "Username not found. Please try again.");

    ui->usernameInput->clear();
    ui->usernameInput->setFocus();
}
