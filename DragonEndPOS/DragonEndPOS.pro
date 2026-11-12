QT += widgets
CONFIG += c++11
TEMPLATE = app
TARGET = DragonEndPOS

SOURCES += \
    src/admin_page.cpp \
    src/cashier_page.cpp \
    src/customer_page.cpp \
    src/display_order.cpp \
    src/kitchen_page.cpp \
    src/login_page.cpp \
    src/main.cpp \
    src/manager_page.cpp \
    src/models.cpp \
    src/staff_placeholder_page.cpp

HEADERS += \
    src/admin_page.h \
    src/cashier_page.h \
    src/customer_page.h \
    src/display_order.h \
    src/kitchen_page.h \
    src/login_page.h \
    src/manager_page.h \
    src/models.h \
    src/staff_placeholder_page.h

FORMS += \
    src/admin_page.ui \
    src/cashier_page.ui \
    src/customer_page.ui \
    src/display_order.ui \
    src/kitchen_page.ui \
    src/login_page.ui \
    src/manager_page.ui
