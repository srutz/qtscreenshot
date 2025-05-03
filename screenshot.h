#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include "overlay.h"

#include <QPixmap>
#include <QWidget>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QPushButton>


class Screenshot : public QWidget
{
    Q_OBJECT

public:
    Screenshot();
    ~Screenshot() override;

protected:

private slots:

private:
    Overlay  *m_overlay;
};

#endif // SCREENSHOT_H
