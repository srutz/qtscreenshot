
#include "marker.h"
#include <QLabel>
#include <QVBoxLayout>

Marker::Marker(QWidget *parent)
    : QWidget(parent)
{
    //setAttribute(Qt::WA_TransparentForMouseEvents);
    //setAttribute(Qt::WA_NoSystemBackground);
    //setAttribute(Qt::WA_OpaquePaintEvent);

    m_label = new QLabel(this);
    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_label);
    layout->setAlignment(m_label, Qt::AlignCenter);
    setStyleSheet("background-color: red; padding: 8px;");
}

QSize Marker::sizeHint() const
{
    return QWidget::sizeHint();
}

