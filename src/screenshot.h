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
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:

private:
    enum ResizeDirection {
        NoResize = 0x0,
        North = 0x1,
        South = 0x2,
        East = 0x4,
        West = 0x8,
        NorthWest = North | West,
        NorthEast = North | East,
        SouthWest = South | West,
        SouthEast = South | East
    };

    QAction *m_aboutAction;
    QAction *m_settingsAction;
    Overlay  *m_overlay;
    QSpinBox *m_delayBox;
    GalleryView *m_galleryView;
    Sheet *m_sheet = nullptr;
    QWidget *m_sheetContent = nullptr;

    bool m_resizing = false;
    int m_resizeDirection = NoResize;
    QPoint m_lastMousePos;
    
    void setupSheet();
    int getResizeDirection(const QPoint &pos);
    void setCursorShape(int direction);
};

#endif // SCREENSHOT_H
