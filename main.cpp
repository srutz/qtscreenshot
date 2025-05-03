
#include <QApplication>
#include <QScreen>
#include <QClipboard>

#include "screenshot.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QObject::connect(&app, &QApplication::aboutToQuit, []() {
        QApplication::clipboard()->clear();
    });
    Screenshot screenshot;
    screenshot.move(screenshot.screen()->availableGeometry().topLeft() + QPoint(20, 20));
    screenshot.show();
    return app.exec();
}


