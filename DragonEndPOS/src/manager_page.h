#ifndef MANAGER_PAGE_H
#define MANAGER_PAGE_H

#include "staff_placeholder_page.h"

namespace Ui {
class ManagerPage;
}

class ManagerPage : public StaffPlaceholderPage {
    Q_OBJECT

public:
    explicit ManagerPage(StaffMember* staff,
                         QWidget* parent = nullptr);
    ~ManagerPage();

private:
    Ui::ManagerPage* ui;
};

#endif
