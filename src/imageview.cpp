#include "imageview.h"

#include <QPainter>

ImageView::ImageView(QWidget *parent)
    : AnimatedWidget{parent}
    , m_label(QString())
{
    m_errorPixmap.load(":/resources/images/block_640.png");
    showErrorImage();
}


void ImageView::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    auto size = this->size();
    if (!m_pixmap.isNull()) {
        auto pixmapSize = m_pixmap.size();

        if (pixmapSize.width() <= size.width() && pixmapSize.height() <= size.height()) {
            auto pos = QPoint(
                (size.width() - pixmapSize.width()) / 2,
                (size.height() - pixmapSize.height()) / 2);
            painter.drawPixmap(pos, m_pixmap);
        } else {
            auto pixmapAspectRatio = static_cast<double>(pixmapSize.width()) / pixmapSize.height();
            double widgetAspectRatio = static_cast<double>(size.width()) / size.height();

            QRectF targetRect;
            if (widgetAspectRatio > pixmapAspectRatio) {
                double scaledWidth = pixmapAspectRatio * size.height();
                targetRect = QRectF((size.width() - scaledWidth) / 2, 0, scaledWidth, size.height());
            } else {
                qreal scaledHeight = size.width() / pixmapAspectRatio;
                targetRect = QRectF(0, (size.height() - scaledHeight) / 2, size.width(), scaledHeight);
            }
            painter.drawPixmap(targetRect, m_pixmap, m_pixmap.rect());
        }
    }
}


QPixmap ImageView::getImage() const
{
    return m_pixmap;
}


QString ImageView::label()
{
    return m_label;
}


void ImageView::setLabel(const QString &label)
{
    this->m_label = label;
}


void ImageView::showPixmap(QPixmap pixmap)
{
    m_pixmap = pixmap;
    update();
}


void ImageView::showErrorImage()
{
    showPixmap(m_errorPixmap);
    m_label = "";
}


void ImageView::showFile(QString path)
{
    QPixmap pixmap;
    bool loaded = pixmap.load(path);
    if (!loaded) {
        showErrorImage();
    }
    m_label = path;
    showPixmap(pixmap);
}
