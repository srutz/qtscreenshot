#include "settings.h"
#include "ui_settings.h"
#include "configmanager.h"
#include <QPushButton>

Settings::Settings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Settings)
{
    ui->setupUi(this);
    auto &cm = ConfigManager::instance();
    ui->m_defaultDelaySeconds->setValue(cm.defaultDelaySeconds());
    ui->m_filenameMask->setText(cm.filenameMask());

    auto okButton = ui->buttonBox->button(QDialogButtonBox::Ok);
    connect(okButton, &QPushButton::clicked, this, [this] {
        auto &cm = ConfigManager::instance();
        cm.setDefaultDelaySeconds(ui->m_defaultDelaySeconds->value());
        cm.setFilenameMask(ui->m_filenameMask->text());
        cm.saveSettings();
    });
}

Settings::~Settings()
{
    delete ui;
}
