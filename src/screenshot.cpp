#include "screenshot.h"
#include "capture.h"
#include "settings.h"
#include "configmanager.h"
#include "imagelist.h"
#include "util.h"
#include "titlebar.h"
#include <memory>
#include <QTimer>
#include <QStyle>
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
#include <QPushButton>
#include <QDialog>
#include <QMessageBox>
#include <QSystemTrayIcon>
#include <QCoreApplication>

// Then use:
QString version = QT_VERSION_STR;
QString runtimeVersion = qVersion();

Screenshot::Screenshot() : QMainWindow()
{
    auto centralWidget = new QWidget(this);
    auto centralLayout = new QVBoxLayout(centralWidget);
    centralLayout->setSpacing(0);
    centralLayout->setContentsMargins(0, 0, 0, 0);
    centralWidget->setStyleSheet("QWidget { background-color: #ffffff; }");
    setCentralWidget(centralWidget);
    setWindowFlags(Qt::FramelessWindowHint);
    this->setupSheet();
        
    // Add custom title bar
    auto titleBar = new TitleBar(this);
    connect(titleBar->menuButton(), &QPushButton::clicked, this, [this]() {
        m_sheet->showSheet(this->centralWidget(), Sheet::Side::Left);
    });
    centralLayout->addWidget(titleBar);
 
    m_galleryView = new GalleryView(this);
    m_galleryView->setInput(shared_ptr<ImageList>(ImageList::instance()));

    auto outerWidget = new QWidget(centralWidget);
    auto outerLayout = new QVBoxLayout(outerWidget);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    centralLayout->addWidget(outerWidget);

    auto mainWidget = new QWidget(outerWidget);
    auto mainLayout = new QHBoxLayout(mainWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->addWidget(mainWidget);

    auto sideBar = new QWidget(centralWidget);
    sideBar->setVisible(false);
    auto sideBarLayout = new QVBoxLayout(sideBar);
    sideBar->setStyleSheet("QWidget { border-right: 1px solid #e7e7e7; margin-top: 8px;}");
    sideBarLayout->setContentsMargins(0, 0, 0, 0);
    sideBar->setFixedWidth(48);
    auto toggleSheetButton = new QPushButton(this);
    connect(toggleSheetButton, &QPushButton::clicked, this, [this] {});
    toggleSheetButton->setFlat(true);
    Util::setLucideIcon(toggleSheetButton, QString::fromUtf8(u8"\uea3a"));

    sideBarLayout->addWidget(toggleSheetButton, 0, Qt::AlignTop | Qt::AlignHCenter);
    mainLayout->addWidget(sideBar);

    auto contentWidget = new QWidget(mainWidget);
    auto contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentWidget->setStyleSheet("QWidget { background-color: #ffffff; }");
    mainLayout->addWidget(contentWidget);

    auto buttons = new QWidget(this->centralWidget());
    contentLayout->addWidget(buttons);
    contentLayout->addWidget(m_galleryView);

    auto buttonLayout = new QHBoxLayout(buttons);
    int buttonWidth = 160;
    {
        auto selectionButton = new QPushButton("Select region", this);
        Util::applyFlatButtonStyle(selectionButton);
        selectionButton->setFixedWidth(buttonWidth);
        buttonLayout->addWidget(selectionButton);
        connect(selectionButton, &QPushButton::clicked, this, [this]()
                { m_overlay->showForSelection(); });
    }

    if (false)
    {
        auto windowButton = new QPushButton("Active Window", this);
        windowButton->setFixedWidth(buttonWidth);
        buttonLayout->addWidget(windowButton);
    }

    {
        auto screenButton = new QPushButton("Screen", this);
        Util::applyFlatButtonStyle(screenButton);
        screenButton->setFixedWidth(buttonWidth);
        buttonLayout->addWidget(screenButton);
        connect(screenButton, &QPushButton::clicked, this, [this]()
                { Capture::captureScreenshot(this, false, QRect(), [] {}); });
    }

    buttonLayout->addSpacing(16);
    buttonLayout->addStretch();
    buttonLayout->addWidget(new QLabel("Delay:", this));
    m_delayBox = new QSpinBox(this);
    m_delayBox->setSuffix(tr(" s"));
    m_delayBox->setMaximum(180);
    const auto &cm = ConfigManager::instance();
    m_delayBox->setValue(cm.defaultDelaySeconds());
    connect(&cm, &ConfigManager::defaultDelaySecondsChanged, this, [this, &cm]
            { m_delayBox->setValue(cm.defaultDelaySeconds()); });
    buttonLayout->addWidget(m_delayBox);

    m_overlay = new Overlay(this);
    setWindowTitle(tr("Screenshot"));
    adjustSize();
    // setMinimumHeight(500);
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
    menuBar->hide();
    setFocusPolicy(Qt::StrongFocus);

    m_settingsAction = new QAction("&Settings", this);
    connect(m_settingsAction, &QAction::triggered, this, [this]() {
        Settings dialog(this);
        dialog.exec(); 
    });
    actionsMenu->addAction(m_settingsAction);

    auto exitAction = new QAction("E&xit", this);
    connect(exitAction, &QAction::triggered, this, [this]() { QApplication::quit(); });
    actionsMenu->addAction(exitAction);

    m_aboutAction = new QAction("&About", this);
    connect(m_aboutAction, &QAction::triggered, this, [this]() {
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
    helpMenu->addAction(m_aboutAction);

    // setup tray icon
    {
        auto showAction = new QAction("Show QtScreenshot", this);
        connect(showAction, &QAction::triggered, this, [this]()
                { this->show(); });
        auto hideAction = new QAction("Hide", this);
        connect(hideAction, &QAction::triggered, this, [this]()
                { this->hide(); });
        auto quitAction = new QAction("E&xit", this);
        connect(quitAction, &QAction::triggered, this, [this]()
                { QApplication::quit(); });

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
        connect(trayIcon, &QSystemTrayIcon::activated, this, [this](auto reason) {
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

void Screenshot::keyPressEvent(QKeyEvent *event) 
{
    if (event->key() == Qt::Key_Alt) {
        // Toggle menu bar visibility
        if (menuBar()->isVisible()) {
            menuBar()->hide();
        } else {
            menuBar()->show();
        }
    }
    QMainWindow::keyPressEvent(event);
}


/* Sets up the sheet and its content
 * the sheet is a sliding sidepanel and the content can be any QWidget
 * To close the sheet from your code invoke hideSheet(true);
 * To open the sheet from your code invoke showSheet(QWidget *destination, QWidget *content);
 */
void Screenshot::setupSheet() {
    // setup the sheet's content
    m_sheetContent = new QWidget(this);
    auto sheetLayout = new QVBoxLayout(m_sheetContent);
    sheetLayout->setContentsMargins(9, 0, 13, 0);

    auto actions = new QWidget(this);
    auto actionsLayout = new QVBoxLayout(actions);
    actionsLayout->setContentsMargins(0, 0, 0, 0);
    actionsLayout->setSpacing(8);

    auto settingsButton = new QPushButton("Settings", this);
    Util::applyButtonStyle(settingsButton);
    connect(settingsButton, &QPushButton::clicked, this, [this]() {       
        QTimer::singleShot(50, this, [this]() { m_settingsAction->triggered(); });
        m_sheet->hideSheet();
    });
    actionsLayout->addWidget(settingsButton);

    auto aboutButton = new QPushButton("About Qt-Screenshot", this);
    Util::applyButtonStyle(aboutButton);
    connect(aboutButton, &QPushButton::clicked, this, [this]() {
        QTimer::singleShot(50, this, [this]() { m_aboutAction->triggered(); });
        m_sheet->hideSheet();
    });
    actionsLayout->addWidget(aboutButton);

    auto header = new QLabel(this);
    header->setText(R"(<html>
        <h3>Qt-Screenshot</h3>
        <p>Written by Stepan Rutz</p>
        )");
    header->setWordWrap(true);

    sheetLayout->addWidget(header);
    sheetLayout->addSpacing(16);
    sheetLayout->addWidget(actions, 0, Qt::AlignHCenter);
    sheetLayout->addStretch();

    // add another close button to the sheet
    auto sheetButton = new QPushButton("Close", this);
    Util::applyFlatButtonStyle(sheetButton);
    connect(sheetButton, &QPushButton::clicked, this, [this] {
        this->m_sheet->hideSheet();
    });
    sheetLayout->addWidget(sheetButton, 0, Qt::AlignRight);
    sheetLayout->addSpacing(8);

    // create the sheet panel
    m_sheet = new Sheet(m_sheetContent, this);
    m_sheet->setStyleSheet("QWidget { background-color: #ffffff; }");
}

void Screenshot::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    /* invoke the layout method on the sheet it you want it
     * to adjust its size during resizing of its parent */
    m_sheet->layout(false);
}
