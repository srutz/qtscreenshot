
#include <QApplication>
#include <QScreen>
#include <QClipboard>
#include <QFontDatabase>


#include "screenshot.h"

#ifdef Q_OS_MAC
#include <AVFoundation/AVFoundation.h>

bool requestScreenRecordingPermission() {
    if (@available(macOS 10.15, *)) {
        CGRequestScreenCaptureAccess();
        return CGPreflightScreenCaptureAccess();
    }
    return true;
}
#else

bool requestScreenRecordingPermission() {
    // On non-macOS platforms, we assume permission is granted.
    return true;
}

#endif



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QObject::connect(&app, &QApplication::aboutToQuit, []() {
        QApplication::clipboard()->clear();
    });
    int fontId = QFontDatabase::addApplicationFont(":/common/resources/fonts/Lucide.ttf");
    if (fontId != -1) {
        QString iconFontFamily = QFontDatabase::applicationFontFamilies(fontId).first();
        qDebug() << "Icon font loaded:" << iconFontFamily;
    } else {
        qWarning("Failed to load icon font.");
        exit(1);
    }   
    Screenshot screenshot;
    //screenshot.move(screenshot.screen()->availableGeometry().topLeft() + QPoint(20, 20));
    screenshot.show();
    return app.exec();
}

