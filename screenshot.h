#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include "overlay.h"
#include "galleryview.h"
#include <QPixmap>
#include <QMainWindow>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QPushButton>


class Screenshot : public QMainWindow
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
    GalleryView *m_galleryView;
};

#endif // SCREENSHOT_H
