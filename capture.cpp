
#include "capture.h"
#include "toast.h"
#include <QGuiApplication>
#include <QApplication>
#include <QScreen>
#include <QPixmap>
#include <QClipboard>


void Capture::captureScreenshot(const QWidget *toastParent, QRect *selectionRect)
{
    auto screen = QGuiApplication::primaryScreen();
    if (screen) {
        auto screenshot = selectionRect
            ? screen->grabWindow(0, selectionRect->x(), selectionRect->y(), selectionRect->width(), selectionRect->height())
            : screen->grabWindow(0);
        QApplication::clipboard()->setPixmap(screenshot);
        auto msg = selectionRect 
            ? QString::fromUtf8(u8"📸 Capture complete. Screenshot of size %1 x %2 saved to clipboard.").arg(selectionRect->size().width()).arg(selectionRect->size().height())
            : QString::fromUtf8(u8"📸 Capture complete. Screenshot saved to clipboard.");
        Toast::showToast(toastParent, msg, 3000);
        QApplication::beep();
    }
}
