#include "imageview.h"
#include "imagelayer.h"
#include "absolutelayout.h"

#include <QPainter>


ImageView::ImageView(QWidget *parent)
    : AnimatedWidget{parent}
    , m_label(QString())
{
    this->setLayout(new AbsoluteLayout(this));
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
    // remove all children
    for (auto child : this->findChildren<QWidget*>()) {
        child->deleteLater();
    }

    // add a new ImageLayer containing the pixmap
    auto imageLayer = new ImageLayer(this);
    imageLayer->showPixmap(pixmap);

    auto layout = qobject_cast<AbsoluteLayout*>(this->layout());
    layout->addWidget(imageLayer, QPoint(0, 0));
    qDebug() << "Layout " << layout->metaObject()->className() << " for ImageView " << this->objectName();
}

