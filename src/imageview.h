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

    QPixmap m_pixmap;
    QPixmap m_errorPixmap;
    QString m_label;

public:
    explicit ImageView(QWidget *parent = nullptr);

    QPixmap getImage() const;
    void showPixmap(QPixmap);
    void showErrorImage();
    void showFile(QString path);
    void paintEvent(QPaintEvent *event) override;

    QString label();
    void setLabel(const QString &l);

signals:
};

#endif // IMAGEVIEW_H
