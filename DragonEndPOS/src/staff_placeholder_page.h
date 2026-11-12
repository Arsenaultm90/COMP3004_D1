#ifndef STAFF_PLACEHOLDER_PAGE_H
#define STAFF_PLACEHOLDER_PAGE_H

#include <QWidget>
#include "models.h"

class QLabel;
class QPushButton;

class StaffPlaceholderPage : public QWidget {
    Q_OBJECT

public:
    explicit StaffPlaceholderPage(
        StaffMember* staff,
        const QString& roleName,
        QWidget* parent = nullptr);

protected:
    void setupPlaceholder(
        QLabel* greetingLabel,
        QLabel* typeLabel,
        QPushButton* logoutButton);

private:
    StaffMember* m_staff;
    QString m_roleName;
};

#endif
