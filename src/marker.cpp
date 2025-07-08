
#include "marker.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QApplication>

Marker::Marker(QWidget *parent)
    : QWidget(parent), m_dragging(false)
{
    //setAttribute(Qt::WA_TransparentForMouseEvents);
    //setAttribute(Qt::WA_NoSystemBackground);
    //setAttribute(Qt::WA_OpaquePaintEvent);

    m_label = new QLabel(this);
    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_label);
    layout->setAlignment(m_label, Qt::AlignCenter);
    setStyleSheet("background-color: red; padding: 8px; border: 1px solid darkred;");
    
    // Enable mouse tracking and make sure the widget can receive mouse events
    setMouseTracking(true);
    setAttribute(Qt::WA_Hover);
}

QSize Marker::sizeHint() const
{
    return QWidget::sizeHint();
}

void Marker::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = true;
        m_dragStartPosition = event->pos();
        setCursor(Qt::ClosedHandCursor);
        // Change appearance to show it's being dragged
        setStyleSheet("background-color: darkred; padding: 8px; border: 2px solid red;");
    }
    QWidget::mousePressEvent(event);
}

void Marker::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging && (event->buttons() & Qt::LeftButton)) {
        // Calculate the new position relative to the parent
        QPoint newPos = mapToParent(event->pos() - m_dragStartPosition);
        
        // Make sure the marker stays within the parent bounds
        if (parentWidget()) {
            QRect parentRect = parentWidget()->rect();
            QSize markerSize = size();
            
            newPos.setX(qMax(0, qMin(newPos.x(), parentRect.width() - markerSize.width())));
            newPos.setY(qMax(0, qMin(newPos.y(), parentRect.height() - markerSize.height())));
        }
        
        // Move the widget to the new position
        //move(newPos);
        
        // Emit signal to notify the layout about position change
        emit positionChanged(newPos);
    }
    QWidget::mouseMoveEvent(event);
}

void Marker::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_dragging) {
        m_dragging = false;
        setCursor(Qt::OpenHandCursor);
        // Restore normal appearance
        setStyleSheet("background-color: red; padding: 8px; border: 1px solid darkred;");
    }
    QWidget::mouseReleaseEvent(event);
}

