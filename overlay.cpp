
#include "overlay.h"
#include "toast.h"
#include "capture.h"
#include <QScreen>
#include <QGuiApplication>
#include <QTimer>
#include <QPainter>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QApplication>
#include <QClipboard>

Overlay::Overlay(QWidget *parent)
    : QWidget(parent),
      m_countdownTimer(nullptr),
      m_shownTicks(0),
      m_visibleTickCount(10),
      m_infoLabel(new QLabel(this)),
      m_mouseDown(false),
      m_mouseDownPos(-1, -1),
      m_mousePos(-1, -1)
{
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setAttribute(Qt::WA_NoSystemBackground);
    // this->setStyleSheet("background-color: rgba(100, 200, 100, 200);");
    // this->setStyleSheet("background-color: red;");
    this->hide();
    this->m_countdownTimer = new QTimer(this);
    m_infoLabel->setStyleSheet("color: white; background-color: rgba(255, 255, 255, 100);");

    m_countdownTimer->setInterval(1000);
    m_countdownTimer->connect(m_countdownTimer, &QTimer::timeout, this, [this]() {
        m_shownTicks++;
        if (m_shownTicks >= m_visibleTickCount) {
            m_shownTicks = 0;
            m_countdownTimer->stop();
            this->dismiss();
        }
        updateUi();
    });
    auto mainLayout = new QVBoxLayout(this);
    m_infoLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    mainLayout->addWidget(m_infoLabel, 0, Qt::AlignTop);
    m_infoLabel->hide();
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
    // paint a semi-transparent rectangle
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, m_mouseDown ? 50 : 100));
    painter.drawRect(this->rect());

    if (m_mouseDown) {
        // Handle mouse move event while dragging
        QRect selectionRect(m_mouseDownPos, m_mousePos);
        selectionRect = selectionRect.normalized();

        painter.setPen(Qt::red);
        painter.drawRect(selectionRect);
        painter.setBrush(QColor(255, 0, 0, 50));
        painter.drawRect(selectionRect.adjusted(1, 1, -1, -1));
    }
}

void Overlay::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        // Your custom ESC key handling here
        event->accept();
        this->dismiss();
        return;
    }
    QWidget::keyPressEvent(event);
}

void Overlay::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    emit visibilityChanged(true);
}

void Overlay::hideEvent(QHideEvent *event) {
    QWidget::hideEvent(event);
    emit visibilityChanged(false);
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
        QTimer::singleShot(300, this, [this]() {
            captureScreenshot();
        });
    }
}

void Overlay::mouseMoveEvent(QMouseEvent *event)
{
    m_mousePos = event->pos();
    if (m_mouseDown) {
        repaint();
    }
}

void Overlay::updateUi() const
{
    m_infoLabel->setText(QString::fromUtf8(
        u8"📸 QScreenShot -> 🖱️ Use mouse to select an area // ⛔ Press ESC to cancel // ⏳ %1 seconds remaining")
        .arg(QString("%1").arg(m_visibleTickCount - m_shownTicks, 2, 10, QChar('0'))));
}   

void Overlay::captureScreenshot() const
{
    // Capture the screenshot of the selected area
    QRect selectionRect(m_mouseDownPos, m_mousePos);
    selectionRect = selectionRect.normalized();
    // translate selectionRect to global coordinates
    selectionRect.translate(this->mapToGlobal(QPoint(0, 0)));
    Capture::captureScreenshot(this, &selectionRect);
}
