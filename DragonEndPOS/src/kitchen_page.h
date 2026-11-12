#ifndef KITCHEN_PAGE_H
#define KITCHEN_PAGE_H

#include <QWidget>
#include "models.h"

class QListWidgetItem;

namespace Ui {
class KitchenPage;
}

class KitchenPage : public QWidget {
    Q_OBJECT

public:
    explicit KitchenPage(StaffMember* staff, QWidget *parent = nullptr);
    ~KitchenPage();

private slots:
    void displaySelectedOrder();
    void completeSelectedOrder();
    void logout();

private:
    void populateOrderQueue();
    void showOrderDetails(Order* order);

    Ui::KitchenPage *ui;
    StaffMember* m_staff;
    int m_selectedOrderNumber = -1;
};

#endif
