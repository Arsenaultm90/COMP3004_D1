#include "staff_placeholder_page.h"

#include <QLabel>
#include <QPushButton>

StaffPlaceholderPage::StaffPlaceholderPage(
    StaffMember* staff,
    const QString& roleName,
    QWidget* parent)
    : QWidget(parent),
    m_staff(staff),
    m_roleName(roleName) {
}

void StaffPlaceholderPage::setupPlaceholder(
    QLabel* greetingLabel,
    QLabel* typeLabel,
    QPushButton* logoutButton) {

    if (m_staff) {
        greetingLabel->setText(QString("Welcome, %1").arg(m_staff->name));
    }

    typeLabel->setText(m_roleName);
    connect(logoutButton, &QPushButton::clicked, this, &QWidget::close);
}
