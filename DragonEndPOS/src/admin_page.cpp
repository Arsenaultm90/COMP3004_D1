#include "admin_page.h"
#include "ui_admin_page.h"

AdminPage::AdminPage(StaffMember* staff, QWidget* parent)
    : StaffPlaceholderPage(staff, "Admin", parent),
    ui(new Ui::AdminPage) {
    ui->setupUi(this);

    setupPlaceholder(
        ui->greetingLabel,
        ui->typeLabel,
        ui->logoutButton);
}

AdminPage::~AdminPage() {
    delete ui;
}
