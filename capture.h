#ifndef CAPTURE_H
#define CAPTURE_H

#include "screenshot.h"
#include <QWidget>
#include <functional>

using std::function;

class Capture {
public:
    static void captureScreenshot(const Screenshot *screenshot, bool hasSelection, QRect selectionRect, function<void()> onComplete);

private:
    static void captureScreenshotWorker(const Screenshot *screenshot, bool hasSelection, QRect selectionRect, function<void()> onComplete);
};



#endif // CAPTURE_H

