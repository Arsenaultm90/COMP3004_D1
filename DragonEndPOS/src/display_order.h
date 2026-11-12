#ifndef DISPLAY_ORDER_H
#define DISPLAY_ORDER_H

#include "models.h"
#include <QDialog>

namespace Ui {
class DisplayOrderDialog;
}

class DisplayOrderDialog : public QDialog {
    Q_OBJECT

public:
    explicit DisplayOrderDialog(const Order& order, QWidget* parent = nullptr);
    ~DisplayOrderDialog();

public slots:
    void setOrderStatus(OrderStatus status);

private:
    void refreshStatus();

    Ui::DisplayOrderDialog* ui;
    Order m_order;
};

#endif
