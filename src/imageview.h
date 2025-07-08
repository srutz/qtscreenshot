#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include "animatedwidget.h"
#include <QWidget>
#include <QPaintEvent>

/*
 * Shows a single pixmap and a label for it
 */
class ImageView : public AnimatedWidget
{
    Q_OBJECT

    QString m_label;

public:
    explicit ImageView(QWidget *parent = nullptr);

    QString label();
    void setLabel(const QString &l);

    void showPixmap(QPixmap pixmap);

signals:
};

#endif // IMAGEVIEW_H
