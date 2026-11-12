#include "cashier_page.h"
#include "ui_cashier_page.h"

CashierPage::CashierPage(StaffMember* staff, QWidget* parent)
    : StaffPlaceholderPage(staff, "Cashier", parent),
    ui(new Ui::CashierPage)
{
    ui->setupUi(this);

    setupPlaceholder(
        ui->greetingLabel,
        ui->typeLabel,
        ui->logoutButton);
}

CashierPage::~CashierPage()
{
    delete ui;
}
