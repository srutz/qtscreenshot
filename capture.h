#ifndef CAPTURE_H
#define CAPTURE_H

#include <QWidget>

class Capture {
public:
    static void captureScreenshot(const QWidget *toastParent, QRect *selectionRect = nullptr);
};



#endif // CAPTURE_H

