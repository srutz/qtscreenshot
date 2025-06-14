#include "overlay.h"
#include "capture.h"
#include <QScreen>
#include <QGuiApplication>
#include <QTimer>
#include <QPainter>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QApplication>
#include <QClipboard>
#include <QPoint>

Overlay::Overlay(QWidget *parent)
    : QWidget(parent),
      m_countdownTimer(nullptr),
      m_shownTicks(0),
      m_visibleTickCount(30),
      m_infoLabel(new QLabel(this)),
      m_mouseDown(false),
      m_mouseDownPos(-1, -1),
      m_mousePos(-1, -1),
      m_capturing(false)
{
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setAttribute(Qt::WA_NoSystemBackground);
    this->hide();
    this->setMouseTracking(true); // Enable mouse tracking

    this->m_countdownTimer = new QTimer(this);
    m_infoLabel->setStyleSheet("color: white; background-color: rgba(255, 255, 255, 100);");

    m_countdownTimer->setInterval(1000);
    m_countdownTimer->connect(m_countdownTimer, &QTimer::timeout, this, [this]()
                              {
        m_shownTicks++;
        if (m_shownTicks >= m_visibleTickCount) {
            m_shownTicks = 0;
            m_countdownTimer->stop();
            this->dismiss();
        }
        updateUi(); });
    auto mainLayout = new QVBoxLayout(this);
    m_infoLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    mainLayout->addWidget(m_infoLabel, 0, Qt::AlignTop);
    m_infoLabel->hide();
    auto geo = QGuiApplication::primaryScreen()->geometry();
    this->setGeometry(geo);
}

void Overlay::showForSelection()
{
    auto geo = QGuiApplication::primaryScreen()->geometry();
    this->setGeometry(geo);
    this->show();
    this->raise();
    this->activateWindow();
    m_infoLabel->show();
    m_countdownTimer->start();
    updateUi();

    /* emergency timeout after 5 minutes */
    QTimer::singleShot(5 * 60 * 1000, this, [this] { this->dismiss(); });
}

void Overlay::dismiss()
{
    this->hide();
    m_infoLabel->hide();
    m_countdownTimer->stop();
}

void Overlay::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    auto w = this->width();
    auto h = this->height();

    auto textColor = Qt::lightGray;
    auto lineColor = QColor(0xf0, 0xf0, 0xf0);
    if (m_mouseDown)
    {
        // Handle mouse move event while dragging
        QRect selectionRect(m_mouseDownPos, m_mousePos);
        selectionRect = selectionRect.normalized();

        // Draw everything but the selection rectangle
        painter.save();
        QRegion outsideRegion(this->rect());
        QRegion selectionRegion(selectionRect);
        QRegion maskRegion = outsideRegion.subtracted(selectionRegion);

        painter.setClipRegion(maskRegion);
        painter.setBrush(QColor(0, 0, 0, 150)); // Semi-transparent background
        painter.setPen(Qt::NoPen);
        painter.drawRect(this->rect());
        painter.restore();

        // Draw the selection rectangle
        painter.setPen(lineColor); // Red border with 2px width
        painter.drawRect(selectionRect.adjusted(1, 1, -1, -1));
        painter.setPen(textColor);
        painter.drawText(
            QPoint(m_mouseDownPos.x(), m_mouseDownPos.y() - 4),
            QString::fromUtf8(u8"%1").arg(selectionRect.width()));
        painter.drawText(
            QPoint(m_mouseDownPos.x() - 32, m_mouseDownPos.y() + 12),
            QString::fromUtf8(u8"%1").arg(selectionRect.height()));
    } else if (!m_capturing) {
        // Draw the overlay without selection rectangle
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(0, 0, 0, 150)); // Semi-transparent background
        painter.drawRect(this->rect());
    } else {
        // paint a semi-transparent rectangle over the whole screen
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(0, 0, 0, 80));
        painter.drawRect(this->rect());
        auto x = m_mousePos.x();
        auto y = m_mousePos.y();
        painter.setPen(textColor);
        painter.drawText(
            QPoint(m_mousePos.x(), m_mousePos.y() - 4),
            QString::fromUtf8(u8"%1 x %2").arg(x).arg(y));
        painter.setPen(lineColor);
        painter.drawLine(x, 0, x, h);
        painter.drawLine(0, y, w, y);
    }
}

void Overlay::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        // Your custom ESC key handling here
        event->accept();
        this->dismiss();
        return;
    }
    QWidget::keyPressEvent(event);
}

void Overlay::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    emit visibilityChanged(VISIBLE);
}

void Overlay::hideEvent(QHideEvent *event)
{
    QWidget::hideEvent(event);
    emit visibilityChanged(CAPTURING);
}

void Overlay::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) {
        return;
    }
    m_mouseDown = true;
    m_mouseDownPos = event->pos();
}

void Overlay::mouseReleaseEvent(QMouseEvent *event)
{
    // Handle mouse release event
    if (m_mouseDown) {
        m_mouseDown = false;
        this->repaint();
        this->dismiss();
        QTimer::singleShot(300, this, [this]() { captureScreenshot(); });
    }
}

void Overlay::mouseMoveEvent(QMouseEvent *event)
{
    m_mousePos = event->pos();
    repaint();
}

void Overlay::updateUi() const
{
    m_infoLabel->setText(QString::fromUtf8(
        u8"📸 QScreenShot -> 🖱️ Use mouse to select an area // ⛔ Press ESC to cancel // ⏳ %1 seconds remaining")
        .arg(QString("%1").arg(m_visibleTickCount - m_shownTicks, 2, 10, QChar('0'))));
}

void Overlay::captureScreenshot()
{
    m_capturing = true;
    // Capture the screenshot of the selected area
    QRect selectionRect(m_mouseDownPos, m_mousePos);
    selectionRect = selectionRect.normalized();
    // translate selectionRect to global coordinates
    selectionRect.translate(this->mapToGlobal(QPoint(0, 0)));
    Capture::captureScreenshot(dynamic_cast<Screenshot *>(this->parent()), true, selectionRect,
        [this] { m_capturing = false; emit visibilityChanged(HIDDEN); });
}
