#ifndef CASHIER_PAGE_H
#define CASHIER_PAGE_H

#include "staff_placeholder_page.h"

namespace Ui {
class CashierPage;
}

class CashierPage : public StaffPlaceholderPage
{
    Q_OBJECT

public:
    explicit CashierPage(StaffMember* staff,
                         QWidget* parent = nullptr);
    ~CashierPage();

private:
    Ui::CashierPage* ui;
};

#endif
