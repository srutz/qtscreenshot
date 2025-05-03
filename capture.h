#ifndef CAPTURE_H
#define CAPTURE_H

#include "screenshot.h"
#include <QWidget>

class Capture {
public:
    static void captureScreenshot(const Screenshot *screenshot, QRect *selectionRect = nullptr);
};



#endif // CAPTURE_H

