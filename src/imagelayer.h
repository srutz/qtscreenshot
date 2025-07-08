#ifndef IMAGELAYER_H
#define IMAGELAYER_H

#include <QWidget>


class ImageLayer : public QWidget
{
    Q_OBJECT
    QPixmap m_pixmap;
    QPixmap m_errorPixmap;

public:
    explicit ImageLayer(QWidget *parent = nullptr);

    QPixmap getImage() const;
    void showPixmap(QPixmap);
    void showErrorImage();
    void showFile(QString path);
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override;
};

#endif // IMAGELAYER_H