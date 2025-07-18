#include "imageview.h"
#include "imagelayer.h"
#include "absolutelayout.h"
#include "marker.h"

#include <QPainter>
#include <QLabel>


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
    layout->addWidget(imageLayer, QPoint(0, 0), AbsoluteLayout::SizeMode::FULLSIZE);
}

// handle doubleclick
void ImageView::mouseDoubleClickEvent(QMouseEvent *event)
{
    // add a QLabel at the mouse position
    auto marker = new Marker(this);
    marker->setText("Mark1");
    auto x = event->pos().x();
    auto y = event->pos().y();
    auto layout = qobject_cast<AbsoluteLayout*>(this->layout());
    layout->addWidget(marker, QPoint(x, y), AbsoluteLayout::SizeMode::OWNSIZE);
}

