#ifndef ADMIN_PAGE_H
#define ADMIN_PAGE_H

#include "staff_placeholder_page.h"

namespace Ui {
class AdminPage;
}

class AdminPage : public StaffPlaceholderPage {
    Q_OBJECT

public:
    explicit AdminPage(StaffMember* staff,
                       QWidget* parent = nullptr);
    ~AdminPage();

private:
    Ui::AdminPage* ui;
};

#endif
