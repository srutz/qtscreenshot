#include "screenshot.h"
#include "capture.h"
#include <QClipboard>
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QScreen>
#include <QGuiApplication>
#include <QMenu>
#include <QMenuBar>
#include <QIcon>
#include <QDialog>
#include <QMessageBox>
#include <QSystemTrayIcon>


Screenshot::Screenshot()
{
    auto mainLayout = new QVBoxLayout(this);

    auto buttons = new QGroupBox("Options", this);
    mainLayout->addWidget(buttons);
    mainLayout->addStretch(1);
    auto buttonLayout = new QHBoxLayout(buttons);
    int buttonWidth = 160;
    {
        auto selectionButton = new QPushButton("Selection", this);
        selectionButton->setFixedWidth(buttonWidth);
        buttonLayout->addWidget(selectionButton);
        connect(selectionButton, &QPushButton::clicked, this, [this]() {
            m_overlay->showForSelection();
        });
    }

    if (false)
    {
        auto windowButton = new QPushButton("Active Window", this);
        windowButton->setFixedWidth(buttonWidth);
        buttonLayout->addWidget(windowButton);
    }

    {
        auto screenButton = new QPushButton("Screen", this);
        screenButton->setFixedWidth(buttonWidth);
        buttonLayout->addWidget(screenButton);
        connect(screenButton, &QPushButton::clicked, this, [this]() {
            Capture::captureScreenshot(this, false, QRect(), [] {});
        });
    }

    buttonLayout->addSpacing(16);
    buttonLayout->addWidget(new QLabel("Delay:", this));
    m_delayBox = new QSpinBox(this);
    m_delayBox->setSuffix(tr(" s"));
    m_delayBox->setMaximum(180);
    buttonLayout->addWidget(m_delayBox);

    m_overlay = new Overlay(this);
    setWindowTitle(tr("Screenshot"));
    adjustSize();
    connect(m_overlay, &Overlay::visibilityChanged, this, [this](OverlayVisiblity visible) {
        if (visible == VISIBLE) {
            this->hide();
        } else if (visible == CAPTURING) {
            this->hide();
        } else {
            this->show();
        }
    });

    // add a menu
    auto menu = new QMenuBar(this);
    auto fileMenu = new QMenu("&File", this);
    menu->addAction(fileMenu->menuAction());
    auto helpMenu = new QMenu("&Help", this);
    menu->addAction(helpMenu->menuAction());

    auto exitAction = new QAction("E&xit", this);
    connect(exitAction, &QAction::triggered, this, [this]() {
        QApplication::quit();
    });
    fileMenu->addAction(exitAction);

    auto aboutAction = new QAction("&About", this);
    connect(aboutAction, &QAction::triggered, this, [this]() {
        auto msg = QString::fromUtf8(u8"🌟 QScreenShot\n\n"
            u8"🖼️ A simple screenshot tool.\n\n"
            u8"👨‍💻 Author: Stepan Rutz / stepan.rutz AT stepanrutz.com\n"
            u8"📦 Version: 1.0.0");
        QString appName = QApplication::applicationName();
        QString appVersion = QApplication::applicationVersion();
        QMessageBox aboutBox(this);
        aboutBox.setWindowTitle("About " + appName);
        //aboutBox.setTextFormat(Qt::RichText);
        aboutBox.setText(msg);
        aboutBox.addButton(QMessageBox::Ok);
        aboutBox.exec();    
    });
    helpMenu->addAction(aboutAction);

    // setup tray icon
    {
        auto showAction = new QAction("Show QtScreenshot", this);
        connect(showAction, &QAction::triggered, this, [this]() {
            this->show();
        });
        auto hideAction = new QAction("Hide", this);
        connect(hideAction, &QAction::triggered, this, [this]() {
            this->hide();
        });
        auto quitAction = new QAction("E&xit", this);
        connect(quitAction, &QAction::triggered, this, [this]() {
            QApplication::quit();
        });

        auto trayIconMenu = new QMenu(this);
        trayIconMenu->addAction(showAction);
        trayIconMenu->addAction(hideAction);
        trayIconMenu->addSeparator();
        trayIconMenu->addAction(quitAction);

        auto trayIcon = new QSystemTrayIcon(this);
        trayIcon->setIcon(QIcon::fromTheme("camera-photo"));
        trayIcon->setContextMenu(trayIconMenu);
        trayIcon->setToolTip("Qt Screenshot");
        trayIcon->show();
    }
}

Screenshot::~Screenshot()
{
}
