#ifndef TOAST_H
#define TOAST_H

#include <QWidget>
#include <QString>


class Toast : public QWidget {
    Q_OBJECT
public:
    explicit Toast(const QString &message, int duration, QWidget *parent = nullptr);
    void showAtTopCenterOf(QWidget *parentWindow);
    static void showToast(const QWidget *widget, const QString &message, int duration = 3000);
};

#endif // TOAST_H
