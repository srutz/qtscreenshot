

#include "toast.h"
#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include <QTimer>
#include <QDebug>
#include <QApplication>


static QMainWindow* getMainWindow(const QWidget *widget) {
    return qobject_cast<QMainWindow *>(widget->window());
}


Toast::Toast(const QString &message, int duration, QWidget *parent)
    : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_ShowWithoutActivating);
    auto layout = new QVBoxLayout(this);
    auto label = new QLabel(message, this);
    label->setStyleSheet("color: white; background-color: rgba(0, 0, 0, 0.85); padding: 10px; border-radius: 5px;");
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label, Qt::AlignCenter);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);
    QTimer::singleShot(duration, this, &Toast::deleteLater);
}

void Toast::showAtTopCenterOf(QWidget *parentWindow)
{
    if (!parentWindow)
        return;
    QRect parentGeometry = parentWindow->geometry();
    int x = parentGeometry.x() + (parentGeometry.width() - this->width()) / 2;
    //int y = parentGeometry.y() + parentGeometry.height() - this->height() - 8;
    int y = parentGeometry.y() + 8;
    move(x, y);
    show();
}

void Toast::showToast(const QWidget *widget, const QString &message, int duration)
{
    // Show a toast notification at the top center of the main window
    auto mainWindow = getMainWindow(widget);
    Toast *toast = new Toast(message, duration);
    toast->resize(300, 80);
    toast->showAtTopCenterOf(mainWindow ? mainWindow : widget->window());
}


