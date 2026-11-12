#include "manager_page.h"
#include "ui_manager_page.h"

ManagerPage::ManagerPage(StaffMember* staff, QWidget* parent)
    : StaffPlaceholderPage(staff, "Manager", parent),
    ui(new Ui::ManagerPage)
{
    ui->setupUi(this);

    setupPlaceholder(
        ui->greetingLabel,
        ui->typeLabel,
        ui->logoutButton);
}

ManagerPage::~ManagerPage()
{
    delete ui;
}
