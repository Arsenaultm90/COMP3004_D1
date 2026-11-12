
#ifndef MODELS_H
#define MODELS_H

#include <QString>
#include <QVector>

// Enums
enum class UserRole {
    Customer,
    KitchenStaff,
    Cashier,
    Manager,
    Administrator
};

enum class OrderStatus {
    InProgress,
    Completed,
    PickedUp,
    Cancelled
};

enum class PurchaseType {
    Online,
    InPerson
};

// Ingredients and quantities
struct IngredientQuantity {
    QString name;
    double quantity;
    QString unit;
};

// Menu items
struct MenuItem {
    int id;
    QString name;
    QString category;
    double price;
    QVector<IngredientQuantity> ingredients;
};

// Order items
struct OrderLine {
    MenuItem item;
    int quantity;
};

// Orders
struct Order {
    int orderNumber = 0;
    QString customerUsername;
    QVector<OrderLine> lines;

    double subtotal = 0.0;
    double tax = 0.0;
    double finalTotal = 0.0;

    int loyaltyPointsUsed = 0;
    double accountCreditUsed = 0.0;
    double amountCharged = 0.0;
    int loyaltyPointsEarned = 0;

    OrderStatus status = OrderStatus::InProgress;
    PurchaseType purchaseType = PurchaseType::Online;

    bool paid = false;
    QString placedAt;
};

// Customers
struct Customer {
    QString name;
    QString username;
    QString email;
    QString phone;
    QString address;
    QString loyaltyNumber;

    int loyaltyPoints = 0;
    double accountCredit = 0.0;
    bool hasCurrentOrder = false;
};

// Staff members
struct StaffMember {
    QString name;
    QString username;
    UserRole role;
    QString staffID;
};

// Data storage
class DataStore {
public:
    static DataStore& instance();

    QVector<MenuItem>& menuItems();
    QVector<Customer>& customers();
    QVector<StaffMember>& staffMembers();
    QVector<Order>& orders();

    Customer* customerByUsername(const QString& username);
    StaffMember* staffByUsername(const QString& username);
    Order* orderByNumber(int orderNumber);
    int nextOrderNumber();

private:
    DataStore();
    void seedData();

    QVector<MenuItem> m_menuItems;
    QVector<Customer> m_customers;
    QVector<StaffMember> m_staffMembers;
    QVector<Order> m_orders;

    int m_nextOrderNumber;
};

// Helper functions
QString orderStatusText(OrderStatus status);

#endif // MODELS_H
