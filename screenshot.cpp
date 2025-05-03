#include "screenshot.h"
#include "toast.h"
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


Screenshot::Screenshot()
{
    auto mainLayout = new QVBoxLayout(this);

    auto buttons = new QGroupBox("Options", this);
    mainLayout->addWidget(buttons);
    mainLayout->addStretch(1);
    auto buttonLayout = new QHBoxLayout(buttons);
    int buttonWidth = 200;
    {
        auto selectionButton = new QPushButton("Selection", this);
        selectionButton->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::ViewRestore));
        selectionButton->setFixedWidth(buttonWidth);
        buttonLayout->addWidget(selectionButton);
        connect(selectionButton, &QPushButton::clicked, this, [this]() {
            m_overlay->showForSelection();
        });
    }

    if (false)
    {
        auto windowButton = new QPushButton("Active Window", this);
        windowButton->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::ViewFullscreen));
        windowButton->setFixedWidth(buttonWidth);
        buttonLayout->addWidget(windowButton);
    }

    {
        auto screenButton = new QPushButton("Screen", this);
        screenButton->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::ViewFullscreen));
        screenButton->setFixedWidth(buttonWidth);
        buttonLayout->addWidget(screenButton);
        connect(screenButton, &QPushButton::clicked, this, [this]() {
            auto screen = QGuiApplication::primaryScreen();
            if (screen) {
                auto screenshot = screen->grabWindow(0);
                QApplication::clipboard()->setPixmap(screenshot);
                auto msg = QString::fromUtf8(u8"📸 Capture complete. Screenshot saved to clipboard.");
                Toast::showToast(this->m_overlay, msg, 3000);
            }
        });
    }

    m_overlay = new Overlay(this);
    setWindowTitle(tr("Screenshot"));
    adjustSize();
    connect(m_overlay, &Overlay::visibilityChanged, this, [this](bool visible) {
        if (visible) {
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

}

Screenshot::~Screenshot()
{
}
