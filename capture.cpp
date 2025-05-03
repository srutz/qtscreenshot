
#include "capture.h"
#include "toast.h"
#include <QGuiApplication>
#include <QApplication>
#include <QScreen>
#include <QPixmap>
#include <QClipboard>


void Capture::captureScreenshot(const Screenshot *screenshot, QRect *selectionRect)
{
    auto screen = QGuiApplication::primaryScreen();
    if (screen) {
        auto image = selectionRect
            ? screen->grabWindow(0, selectionRect->x(), selectionRect->y(), selectionRect->width(), selectionRect->height())
            : screen->grabWindow(0);
        QApplication::clipboard()->setPixmap(image);
        auto msg = selectionRect 
            ? QString::fromUtf8(u8"📸 Capture complete. Screenshot of size %1 x %2 saved to clipboard.").arg(selectionRect->size().width()).arg(selectionRect->size().height())
            : QString::fromUtf8(u8"📸 Capture complete. Screenshot saved to clipboard.");
        Toast::showToast(screenshot->overlay(), msg, 3000);
        QApplication::beep();
    }
}
