
#include "capture.h"
#include "configmanager.h"
#include "toast.h"
#include <QGuiApplication>
#include <QApplication>
#include <QScreen>
#include <QPixmap>
#include <QClipboard>
#include <QTimer>

void Capture::captureScreenshot(const Screenshot *screenshot, bool hasSelection, QRect selectionRect, function<void()> onComplete)
{
    auto delay = screenshot->delaySeconds();
    if (delay == 0) {
        captureScreenshotWorker(screenshot, hasSelection, selectionRect, onComplete);
    } else {
        QTimer::singleShot(delay * 1000, screenshot, [=]() {
            captureScreenshotWorker(screenshot, hasSelection, selectionRect, onComplete);
        });
    }
}

void Capture::captureScreenshotWorker(const Screenshot *screenshot, bool hasSelection, QRect selectionRect, function<void()> onComplete)
{
    auto mutable_screenshot = const_cast<Screenshot*>(screenshot);
    mutable_screenshot->hide();
    QTimer::singleShot(200, [=]() {
        auto screen = QGuiApplication::primaryScreen();
        if (screen) {
            auto image = hasSelection
                ? screen->grabWindow(0, selectionRect.x(), selectionRect.y(), selectionRect.width(), selectionRect.height())
                : screen->grabWindow(0);
            QApplication::clipboard()->setPixmap(image);
            auto msg = hasSelection
                ? QString::fromUtf8(u8"📸 Capture complete. Screenshot of size %1 x %2 saved to clipboard.").arg(selectionRect.size().width()).arg(selectionRect.size().height())
                : QString::fromUtf8(u8"📸 Capture complete. Screenshot saved to clipboard.");
            Toast::showToast(screenshot->overlay(), msg, 3000);
            QApplication::beep();
            auto imageList = ImageList::instance();
            QString filename = ConfigManager::instance().filenameMask();
            // replace %d with the current date
            filename.replace("%d", QDateTime::currentDateTime().toString("yyyy-MM-dd"));
            // replace %t with the current time
            filename.replace("%t", QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss"));

            imageList->addImage(ImageSpec{true, image, filename});
            imageList->setIndex(imageList->size() - 1);
        }
        mutable_screenshot->show();
        onComplete();
    });
}
