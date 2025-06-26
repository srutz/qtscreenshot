
#ifndef OVERLAY_H
#define OVERLAY_H
#include <QWidget>
#include <QLabel>

enum OverlayVisiblity { HIDDEN, CAPTURING, VISIBLE };

class Overlay : public QWidget {
    Q_OBJECT

public:
    Overlay(QWidget *parent = nullptr);
    void showForSelection();
    void dismiss();

private:
    void updateUi() const;
    void captureScreenshot();

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QTimer *m_countdownTimer;
    int m_shownTicks;
    int m_visibleTickCount;
    QLabel *m_infoLabel;
    bool m_mouseDown;
    QPoint m_mouseDownPos;
    QPoint m_mousePos;
    bool m_capturing;

signals:
    void visibilityChanged(OverlayVisiblity visible);
};

#endif // OVERLAY_H

