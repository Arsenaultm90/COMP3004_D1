DragonEndPOS - Customer Dashboard starter code
================================================

This package contains the C++ logic for the customer dashboard using the
customer_page.ui file you designed in Qt Designer. It does not contain a
.pro file.

Files
-----
models.h / models.cpp
    In-memory menu, customer, order models, and starter data.

customerpage.h / customerpage.cpp
    Loads and connects the Designer form, menu/cart handling, totals,
    order placement, customer balance updates, logout, and a future
    kitchen-facing status update slot.

displayorder.h / displayorder.cpp
    A small order-status dialog built in C++ because no
    displayOrder.ui file was supplied. It shows order details and animates
    the "Order in progress..." dots using QTimer.

main.cpp
    Minimal dashboard-only entry point. It opens the demo customer directly;
    there is no login or kitchen screen yet.

Qt Designer form
----------------
Keep your existing customer_page.ui in the project. Its generated UI header
must be named ui_customer_page.h (Qt's normal uic output for customer_page.ui).
The root form class in the supplied file is customerPage.

Expected object names used by the code
--------------------------------------
logoutButton, menuList, comboBox, addItemButton, orderDisplay,
subtotalMaount, taxAmount, finalAmount, loyaltyAmount, creditAmount,
dueAmount, orderButton, listWidget, loyaltyPointsSpinBox, creditSpinBox,
greetingLabel, emailLabel, phoneNumberLabel, addressLabel,
loyaltyNumberLabel, loyaltyPointsLabel, accountCreditLabel.

Add these source/header files to your existing qmake project:
    main.cpp
    models.cpp
    customerpage.cpp
    displayorder.cpp
    models.h
    customerpage.h
    displayorder.h

The project must link Qt Widgets. Do not add a second main.cpp if your
project already has one; instead instantiate CustomerPage from your
existing startup code.

Business rules implemented
--------------------------
- 15% tax is calculated on the full subtotal.
- Loyalty points redeem at $1 per point, up to 50% of the pre-tax subtotal.
- Account credit is applied after tax and loyalty redemption.
- Orders are recorded as paid in memory; no payment provider is called.
- A customer can have only one current order at a time.
- New orders begin In Progress.
- Loyalty points are earned at one point per $3 charged, rounded down,
  only when an order reaches Completed and no points were redeemed.
- Order status can be updated later through CustomerPage::updateOrderStatus().

Important D1 work still required
--------------------------------
This is a dashboard-only starter, not the complete Deliverable 1 dataset.
The sample store currently seeds one demo customer and ten menu items.
D1 also requires 11 users, five customers with five historical orders each,
and consistent initial balances/history. Add that required dataset before
submission. The order history list currently displays a placeholder.

Future kitchen connection
-------------------------
A future kitchen widget can connect its status signal to this page's slot:

    connect(kitchenWidget, &KitchenWidget::orderStatusUpdated,
            customerPage, &CustomerPage::updateOrderStatus);

The kitchen widget's signal should carry (int orderNumber, OrderStatus status).
Alternatively, it can listen to CustomerPage::orderPlaced(int) to learn when
a new order is created.
