
#include "models.h"
#include <QDateTime>
#include <QtMath>


DataStore& DataStore::instance() {
    static DataStore store;
    return store;
}

DataStore::DataStore()
    : m_nextOrderNumber(1001) {
    seedData();
}

void DataStore::seedData() {
    // Menu items
    m_menuItems = {
        {
            1, "Dragon Chicken Sandwich", "Sandwiches", 9.50,
            {
                {"Bread", 2, "slices"},
                {"Chicken", 150, "g"},
                {"Lettuce", 20, "g"}
            }
        },
        {
            2, "Harold's Ham Sandwich", "Sandwiches", 8.75,
            {
                {"Bread", 2, "slices"},
                {"Ham", 100, "g"},
                {"Cheese", 1, "slice"}
            }
        },
        {
            3, "Timmy's Veggie Sandwich", "Sandwiches", 8.25,
            {
                {"Bread", 2, "slices"},
                {"Tomato", 50, "g"},
                {"Lettuce", 20, "g"},
                {"Cheese", 1, "slice"}
            }
        },
        {
            4, "Americano", "Hot Beverages", 3.00,
            {
                {"Coffee", 18, "g"},
                {"Water", 240, "ml"}
            }
        },
        {
            5, "Latte", "Hot Beverages", 4.25,
            {
                {"Coffee", 18, "g"},
                {"Milk", 200, "ml"}
            }
        },
        {
            6, "Cappuccino", "Hot Beverages", 4.00,
            {
                {"Coffee", 18, "g"},
                {"Milk", 150, "ml"}
            }
        },
        {
            7, "Hot Chocolate", "Hot Beverages", 3.75,
            {
                {"Milk", 240, "ml"},
                {"Chocolate", 25, "g"}
            }
        },
        {
            8, "Iced Coffee", "Cold Beverages", 3.50,
            {
                {"Coffee", 18, "g"},
                {"Milk", 100, "ml"},
                {"Ice", 100, "g"}
            }
        },
        {
            9, "Iced Tea", "Cold Beverages", 3.00,
            {
                {"Tea", 2, "g"},
                {"Water", 240, "ml"},
                {"Ice", 100, "g"}
            }
        },
        {
            10, "Lemonade", "Cold Beverages", 3.25,
            {
                {"Lemon", 50, "ml"},
                {"Water", 200, "ml"},
                {"Ice", 100, "g"}
            }
        }
    };



    // Customers
    m_customers.clear();

    Customer c1;
    c1.name = "Doug Dimmadome";
    c1.username = "doug";
    c1.email = "doug@hotmail.com";
    c1.phone = "555-555-0101";
    c1.address = "123 Stadium Ave, Whoville";
    c1.loyaltyNumber = "100001";
    c1.loyaltyPoints = 120;
    c1.accountCredit = 15.00;
    c1.hasCurrentOrder = false;
    m_customers.append(c1);

    Customer c2;
    c2.name = "Zephram Foggarty";
    c2.username = "zephram";
    c2.email = "zephram@gmail.com";
    c2.phone = "555-555-0102";
    c2.address = "568 Red Castle Road, Whoville";
    c2.loyaltyNumber = "100002";
    c2.loyaltyPoints = 100;
    c2.accountCredit = 30.00;
    c2.hasCurrentOrder = false;
    m_customers.append(c2);

    Customer c3;
    c3.name = "Bart Saxon";
    c3.username = "bart";
    c3.email = "bar@proton.com";
    c3.phone = "555-555-0103";
    c3.address = "45 Hidden Street, Whoville";
    c3.loyaltyNumber = "100003";
    c3.loyaltyPoints = 150;
    c3.accountCredit = 25.00;
    c3.hasCurrentOrder = false;
    m_customers.append(c3);

    Customer c4;
    c4.name = "Jimothy Jones";
    c4.username = "jimothy";
    c4.email = "jimothy@yahoocom";
    c4.phone = "555-555-0104";
    c4.address = "56 Kingfisher Ave, Whoville";
    c4.loyaltyNumber = "100004";
    c4.loyaltyPoints = 200;
    c4.accountCredit = 40.00;
    c4.hasCurrentOrder = false;
    m_customers.append(c4);

    Customer c5;
    c5.name = "Axel Dingleberry";
    c5.username = "axel";
    c5.email = "axel@tuta.com";
    c5.phone = "555-555-0105";
    c5.address = "98 Tudor Lane, Whoville";
    c5.loyaltyNumber = "100005";
    c5.loyaltyPoints = 80;
    c5.accountCredit = 20.00;
    c5.hasCurrentOrder = false;
    m_customers.append(c5);



    // Staff members
    m_staffMembers.clear();

    StaffMember s1;
    s1.name = "Morgan Darlan";
    s1.username = "chef1";
    s1.role = UserRole::KitchenStaff;
    s1.staffID = "K001";
    m_staffMembers.append(s1);

    StaffMember s2;
    s2.name = "Garum Garfunkel";
    s2.username = "chef2";
    s2.role = UserRole::KitchenStaff;
    s2.staffID = "K002";
    m_staffMembers.append(s2);

    StaffMember s3;
    s3.name = "Jaxon Fontend";
    s3.username = "cashier1";
    s3.role = UserRole::Cashier;
    s3.staffID = "C001";
    m_staffMembers.append(s3);

    StaffMember s4;
    s4.name = "Bjorn Steffardson";
    s4.username = "cashier2";
    s4.role = UserRole::Cashier;
    s4.staffID = "C002";
    m_staffMembers.append(s4);

    StaffMember s5;
    s5.name = "Quinton Marvelo";
    s5.username = "manager";
    s5.role = UserRole::Manager;
    s5.staffID = "M001";
    m_staffMembers.append(s5);

    StaffMember s6;
    s6.name = "Kevin Treeborn";
    s6.username = "admin";
    s6.role = UserRole::Administrator;
    s6.staffID = "A001";
    m_staffMembers.append(s6);



    // Historical dummy data
    m_orders.clear();

    int orderNumber = 1001;

    // Create a historical order and calculate its financial values.
    auto addHistoricalOrder = [&](const QString& username,
                                  int menuItemIndex,
                                  int quantity,
                                  OrderStatus status,
                                  int pointsUsed,
                                  double creditUsed,
                                  PurchaseType purchaseType)
    {
        Order order;

        const MenuItem& item = m_menuItems[menuItemIndex];

        order.orderNumber = orderNumber++;
        order.customerUsername = username;

        order.lines.append({item, quantity});

        order.subtotal = item.price * quantity;
        order.tax = qRound(order.subtotal * 0.15 * 100.0) / 100.0;
        order.finalTotal = order.subtotal + order.tax;

        order.loyaltyPointsUsed = pointsUsed;
        order.accountCreditUsed = creditUsed;

        double maximumPointsValue = order.subtotal * 0.50;

        if (pointsUsed > maximumPointsValue) {
            order.loyaltyPointsUsed = 0;
        }

        // Account credit cannot exceed the remaining balance.
        double remainingAmount =
            order.finalTotal - order.loyaltyPointsUsed;

        if (order.accountCreditUsed > remainingAmount) {
            order.accountCreditUsed = 0.0;
        }

        order.amountCharged =
            order.finalTotal
            - order.loyaltyPointsUsed
            - order.accountCreditUsed;

        // No points are earned if loyalty points were redeemed.
        if (order.loyaltyPointsUsed == 0) {
            order.loyaltyPointsEarned =
                static_cast<int>(order.amountCharged / 3.0);
        } else {
            order.loyaltyPointsEarned = 0;
        }

        order.status = status;
        order.purchaseType = purchaseType;
        order.paid = true;
        order.placedAt =
            QDateTime::currentDateTime()
                .addDays(-(order.orderNumber - 1000))
                .toString("yyyy-MM-dd hh:mm:ss");

        m_orders.append(order);
    };

    // DOUG DIMMADOME
    addHistoricalOrder("doug", 0, 1, OrderStatus::PickedUp, 0, 0.00, PurchaseType::Online);
    addHistoricalOrder("doug", 4, 2, OrderStatus::PickedUp, 0, 0.00, PurchaseType::InPerson);
    addHistoricalOrder("doug", 1, 2, OrderStatus::Cancelled, 5, 5.00, PurchaseType::Online);
    addHistoricalOrder("doug", 7, 1, OrderStatus::PickedUp, 0, 0.00, PurchaseType::Online);
    addHistoricalOrder("doug", 8, 2, OrderStatus::Cancelled, 0, 3.00, PurchaseType::InPerson);

    // ZEPHRAM FOGGARTY
    addHistoricalOrder("zephram", 2, 1, OrderStatus::PickedUp, 0, 0.00, PurchaseType::Online);
    addHistoricalOrder("zephram", 3, 2, OrderStatus::PickedUp, 3, 0.00, PurchaseType::Online);
    addHistoricalOrder("zephram", 5, 1, OrderStatus::Cancelled, 0, 0.00, PurchaseType::InPerson);
    addHistoricalOrder("zephram", 6, 2, OrderStatus::PickedUp, 0, 0.00, PurchaseType::Online);
    addHistoricalOrder("zephram", 9, 1, OrderStatus::Cancelled, 0, 2.00, PurchaseType::InPerson);

    // BART SAXON
    addHistoricalOrder("bart", 0, 2, OrderStatus::PickedUp, 0, 0.00, PurchaseType::Online);
    addHistoricalOrder("bart", 4, 1, OrderStatus::PickedUp, 0, 4.00, PurchaseType::Online);
    addHistoricalOrder("bart", 8, 2, OrderStatus::Cancelled, 0, 0.00, PurchaseType::InPerson);
    addHistoricalOrder("bart", 1, 1, OrderStatus::PickedUp, 0, 0.00, PurchaseType::Online);
    addHistoricalOrder("bart", 6, 1, OrderStatus::Cancelled, 0, 0.00, PurchaseType::InPerson);

    // JIMOTHY JONES
    addHistoricalOrder("jimothy", 2, 2, OrderStatus::PickedUp, 0, 0.00, PurchaseType::Online);
    addHistoricalOrder("jimothy", 3, 1, OrderStatus::PickedUp, 0, 0.00, PurchaseType::InPerson);
    addHistoricalOrder("jimothy", 5, 2, OrderStatus::Cancelled, 0, 0.00, PurchaseType::Online);
    addHistoricalOrder("jimothy", 7, 2, OrderStatus::PickedUp, 0, 0.00, PurchaseType::InPerson);
    addHistoricalOrder("jimothy", 9, 2, OrderStatus::Cancelled, 0, 0.00, PurchaseType::Online);

    // AXEL DINGLEBERRY
    addHistoricalOrder("axel", 0, 1, OrderStatus::PickedUp, 0, 0.00, PurchaseType::Online);
    addHistoricalOrder("axel", 4, 1, OrderStatus::PickedUp, 0, 0.00, PurchaseType::InPerson);
    addHistoricalOrder("axel", 6, 1, OrderStatus::Cancelled, 0, 0.00, PurchaseType::Online);
    addHistoricalOrder("axel", 8, 2, OrderStatus::PickedUp, 0, 0.00, PurchaseType::InPerson);
    addHistoricalOrder("axel", 1, 1, OrderStatus::Cancelled, 0, 0.00, PurchaseType::Online);



    // Calculating customer balances using historical data
    for (Customer& customer : m_customers) {
        int pointsUsed = 0;
        int pointsEarned = 0;
        double creditUsed = 0.0;

        for (const Order& order : m_orders) {
            if (order.customerUsername != customer.username)
                continue;

            pointsUsed += order.loyaltyPointsUsed;
            pointsEarned += order.loyaltyPointsEarned;
            creditUsed += order.accountCreditUsed;
        }

        customer.loyaltyPoints += pointsEarned - pointsUsed;
        customer.accountCredit -= creditUsed;
        customer.hasCurrentOrder = false;
    }

    // The next new order gets the next available number.
    m_nextOrderNumber = orderNumber;
}


// Data access functions
QVector<MenuItem>& DataStore::menuItems() {
    return m_menuItems;
}

QVector<Customer>& DataStore::customers() {
    return m_customers;
}

QVector<StaffMember>& DataStore::staffMembers() {
    return m_staffMembers;
}

QVector<Order>& DataStore::orders() {
    return m_orders;
}

Customer* DataStore::customerByUsername(const QString& username) {
    for (Customer& customer : m_customers) {
        if (customer.username.compare(
                username, Qt::CaseInsensitive) == 0) {
            return &customer;
        }
    }

    return nullptr;
}

StaffMember* DataStore::staffByUsername(const QString& username) {
    for (StaffMember& staff : m_staffMembers) {
        if (staff.username.compare(
                username, Qt::CaseInsensitive) == 0) {
            return &staff;
        }
    }

    return nullptr;
}

Order* DataStore::orderByNumber(int orderNumber) {
    for (Order& order : m_orders) {
        if (order.orderNumber == orderNumber)
            return &order;
    }

    return nullptr;
}

int DataStore::nextOrderNumber() {
    return m_nextOrderNumber++;
}



// Helper functions
QString orderStatusText(OrderStatus status) {
    switch (status) {
    case OrderStatus::InProgress:
        return "In Progress";

    case OrderStatus::Completed:
        return "Completed";

    case OrderStatus::PickedUp:
        return "Picked Up";

    case OrderStatus::Cancelled:
        return "Cancelled";
    }

    return "Unknown";
}
