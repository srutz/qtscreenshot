#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include "overlay.h"
#include "galleryview.h"
#include "sheet.h"
#include <QPixmap>
#include <QMainWindow>
#include <QLabel>
#include <QSpinBox>
#include <QAction>
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
    void keyPressEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:

private:
    QAction *m_aboutAction;
    QAction *m_settingsAction;
    Overlay  *m_overlay;
    QSpinBox *m_delayBox;
    GalleryView *m_galleryView;
    Sheet *m_sheet = nullptr;
    QWidget *m_sheetContent = nullptr;
    
    void setupSheet();
};

#endif // SCREENSHOT_H
