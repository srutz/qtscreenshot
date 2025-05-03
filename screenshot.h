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

    Overlay* overlay() const { return m_overlay; }
    int delaySeconds() const { return m_delayBox->value(); }

protected:

private slots:

private:
    Overlay  *m_overlay;
    QSpinBox *m_delayBox;
};

#endif // SCREENSHOT_H
