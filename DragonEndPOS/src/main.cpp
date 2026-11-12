#include "login_page.h"
#include "models.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QPalette palette;

    // Colour scheme for consistency between systems
    palette.setColor(QPalette::Window, QColor(25, 25, 25));
    palette.setColor(QPalette::WindowText, QColor(200, 200, 200));
    palette.setColor(QPalette::Base, QColor(25, 25, 25));
    palette.setColor(QPalette::AlternateBase, QColor(40, 40, 40));
    palette.setColor(QPalette::Text, QColor(225, 225, 225));
    palette.setColor(QPalette::Button, QColor(230, 230, 230));
    palette.setColor(QPalette::ButtonText, QColor(225, 225, 225));
    palette.setColor(QPalette::Highlight, QColor(50, 120, 200));
    palette.setColor(QPalette::HighlightedText, Qt::white);

    app.setPalette(palette);

    // Initialize the in-memory data.
    DataStore::instance();

    // Initialize login window
    login_page login;
    login.setWindowTitle("DragonEndPOS - Login");
    login.show();

    return app.exec();
}
