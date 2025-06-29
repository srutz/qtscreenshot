

#include "titlebar.h"
#include "util.h"
#include <QPushButton>

TitleBar::TitleBar(QWidget *parent) : QWidget(parent), m_dragging(false)
{
    setFixedHeight(30);
    auto layout = new QHBoxLayout(this);
    layout->setContentsMargins(8, 0, 4, 0);
    layout->setSpacing(0);

    // App title
    auto titleLabel = new QLabel("Qt-Screenshot", this);
    m_menuButton = new QPushButton(this);
    m_menuButton->setFlat(true);

    Util::setLucideIcon(m_menuButton, QString::fromUtf8(u8"\uea3a"));
    m_menuButton->setStyleSheet("margin-top: 1px; margin-right: 12px;");
    layout->addWidget(m_menuButton);
    layout->addWidget(titleLabel);
    layout->addStretch();

    auto buttons = new QWidget(this);
    auto buttonLayout = new QHBoxLayout(buttons);
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    buttonLayout->setSpacing(1);
    // Window control buttons
    auto minimizeBtn = new QPushButton(this);
    Util::setLucideIcon(minimizeBtn, QString::fromUtf8(u8"\uea40"));
    auto maximizeBtn = new QPushButton(this);
    Util::setLucideIcon(maximizeBtn, QString::fromUtf8(u8"\uea37"));
    auto closeBtn = new QPushButton(this);
    Util::setLucideIcon(closeBtn, QString::fromUtf8(u8"\ueb15"));

    // Style the buttons
    QString buttonStyle = R"(
        QPushButton {
            border: none;
            color: #2f2f2f;
            font-size: 14px;
            font-weight: bold;
            padding: 4px 4px;
        }
        QPushButton:hover {
            background-color: #cfcfcf;
        }
    )";

    QString closeButtonStyle = buttonStyle + R"(
        QPushButton:hover { background-color: #e74c3c; }
    )";

    minimizeBtn->setStyleSheet(buttonStyle);
    maximizeBtn->setStyleSheet(buttonStyle);
    closeBtn->setStyleSheet(closeButtonStyle);

    buttonLayout->addWidget(minimizeBtn);
    buttonLayout->addWidget(maximizeBtn);
    buttonLayout->addWidget(closeBtn);

    layout->addWidget(buttons);

    connect(minimizeBtn, &QPushButton::clicked, [this]() { window()->showMinimized(); });
    connect(maximizeBtn, &QPushButton::clicked, [this]() {
        if (window()->isMaximized()) {
            window()->showNormal();
        } else {
            window()->showMaximized();
        } 
    });
    connect(closeBtn, &QPushButton::clicked, [this]() { window()->close(); });
}

void TitleBar::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_dragging = true;
        m_dragPosition = (event->globalPosition() - window()->frameGeometry().topLeft()).toPoint();
        event->accept();
    }
}

void TitleBar::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton && m_dragging)
    {
        auto position = event->globalPosition() - m_dragPosition;
        window()->move(position.toPoint());
        event->accept();
    }
}

void TitleBar::mouseReleaseEvent(QMouseEvent *event)
{
    m_dragging = false;
    event->accept();
}

QPushButton* TitleBar::menuButton() const
{
    return m_menuButton;
}
