#include "imagelayer.h"
#include <QPainter>
#include <QPixmap>

ImageLayer::ImageLayer(QWidget *parent)
    : QWidget(parent)
{
    m_errorPixmap.load(":/resources/images/block_640.png");
    showErrorImage();
}


QPixmap ImageLayer::getImage() const
{
    return m_pixmap;
}

void ImageLayer::showPixmap(QPixmap pixmap)
{
    m_pixmap = pixmap;
    update();
}


void ImageLayer::showErrorImage()
{
    showPixmap(m_errorPixmap);
}


void ImageLayer::showFile(QString path)
{
    QPixmap pixmap;
    bool loaded = pixmap.load(path);
    if (!loaded) {
        showErrorImage();
    }
    showPixmap(pixmap);
}


void ImageLayer::paintEvent(QPaintEvent *event)
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


QSize ImageLayer::sizeHint() const
{
    if (!m_pixmap.isNull()) {
        return m_pixmap.size();
    }
    return QSize(400, 300); // Default size
}



