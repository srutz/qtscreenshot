#include "screenshot.h"
#include "capture.h"
#include "settings.h"
#include "configmanager.h"
#include "imagelist.h"
#include <memory>
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
#include <QCoreApplication>

// Then use:
QString version = QT_VERSION_STR;
QString runtimeVersion = qVersion();

Screenshot::Screenshot() : QMainWindow()
{
    auto centralwidget = new QWidget(this);
    setCentralWidget(centralwidget);

    m_galleryView = new GalleryView(this);
    m_galleryView->setInput(shared_ptr<ImageList>(ImageList::instance()));
    auto mainLayout = new QVBoxLayout(this->centralWidget());

    auto buttons = new QGroupBox(this->centralWidget());
    buttons->setTitle(QString());
    mainLayout->addWidget(buttons);
    mainLayout->addWidget(m_galleryView);

    auto buttonLayout = new QHBoxLayout(buttons);
    int buttonWidth = 160;
    {
        auto selectionButton = new QPushButton("Select region", this);
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
    buttonLayout->addStretch();
    buttonLayout->addWidget(new QLabel("Delay:", this));
    m_delayBox = new QSpinBox(this);
    m_delayBox->setSuffix(tr(" s"));
    m_delayBox->setMaximum(180);
    const auto &cm = ConfigManager::instance();
    m_delayBox->setValue(cm.defaultDelaySeconds());
    connect(&cm, &ConfigManager::defaultDelaySecondsChanged, this, [this,&cm] {
        m_delayBox->setValue(cm.defaultDelaySeconds());
    });
    buttonLayout->addWidget(m_delayBox);

    m_overlay = new Overlay(this);
    setWindowTitle(tr("Screenshot"));
    adjustSize();
    //setMinimumHeight(500);
    resize(800, 600);

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
    auto menuBar = new QMenuBar(this);
    setMenuBar(menuBar);
    auto actionsMenu = new QMenu("&Actions", this);
    menuBar->addAction(actionsMenu->menuAction());
    auto helpMenu = new QMenu("&Help", this);
    menuBar->addAction(helpMenu->menuAction());


    auto settingsAction = new QAction("&Settings", this);
    connect(settingsAction, &QAction::triggered, this, [this]() {
        Settings dialog(this);
        dialog.exec();
    });
    actionsMenu->addAction(settingsAction);

    auto exitAction = new QAction("E&xit", this);
    connect(exitAction, &QAction::triggered, this, [this]() {
        QApplication::quit();
    });
    actionsMenu->addAction(exitAction);

    auto aboutAction = new QAction("&About", this);
    connect(aboutAction, &QAction::triggered, this, [this]() {
        auto msg = QString::fromUtf8(u8"🌟 QScreenShot\n\n"
            u8"🖼️ A simple screenshot tool.\n\n"
            u8"🖥️ Compile-Version: %1\n"
            u8"🕒 Runtime-Version: %2\n\n"
            u8"👨‍💻 Author: Stepan Rutz / stepan.rutz AT stepanrutz.com\n"
            u8"📦 Version: 1.0.0")
            .arg(QString::fromUtf8(QT_VERSION_STR))
            .arg(QString::fromUtf8(qVersion()))
            ;
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
        connect(trayIcon, &QSystemTrayIcon::activated, this, [this] (auto reason) {
            if (reason == QSystemTrayIcon::ActivationReason::Trigger || reason == QSystemTrayIcon::ActivationReason::DoubleClick) {
                this->activateWindow();
                this->showNormal();
                this->raise();
            }
        });
    }
}

Screenshot::~Screenshot()
{
}
