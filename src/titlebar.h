
#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QPushButton>
#include <QHBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QMouseEvent>

class TitleBar : public QWidget
{
    Q_OBJECT

public:
    TitleBar(QWidget *parent = nullptr);
    QPushButton* menuButton() const;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    bool m_dragging;
    QPoint m_dragPosition;
    QPushButton *m_menuButton;
};

#endif // TITLEBAR_H

