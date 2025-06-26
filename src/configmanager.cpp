#include "configmanager.h"

ConfigManager& ConfigManager::instance()
{
    static ConfigManager instance;
    return instance;
}

ConfigManager::ConfigManager(QObject *parent)
    : QObject(parent)
    , m_filenameMask(DEFAULT_FILENAME_MASK)
    , m_defaultDelaySeconds(DEFAULT_DELAY_SECONDS)
{
    const auto configPath = QDir(QStandardPaths::writableLocation(QStandardPaths::HomeLocation)).filePath(SETTINGS_FILENAME);
    m_settings = new QSettings(configPath, QSettings::IniFormat, this);
    loadSettings();
}

QString ConfigManager::filenameMask() const
{
    return m_filenameMask;
}

void ConfigManager::setFilenameMask(const QString &filenameMask)
{
    if (m_filenameMask != filenameMask) {
        m_filenameMask = filenameMask;
        emit filenameMaskChanged(m_filenameMask);
    }
}

int ConfigManager::defaultDelaySeconds() const
{
    return m_defaultDelaySeconds;
}

void ConfigManager::setDefaultDelaySeconds(int seconds)
{
    if (m_defaultDelaySeconds != seconds) {
        m_defaultDelaySeconds = seconds;
        emit defaultDelaySecondsChanged(m_defaultDelaySeconds);
    }
}

void ConfigManager::saveSettings()
{
    m_settings->setValue(KEY_FILENAME_MASK, m_filenameMask);
    m_settings->setValue(KEY_DEFAULT_DELAY_SECONDS, m_defaultDelaySeconds);
    m_settings->sync();
}

void ConfigManager::loadSettings()
{
    setFilenameMask(m_settings->value(KEY_FILENAME_MASK, DEFAULT_FILENAME_MASK).toString());
    setDefaultDelaySeconds(m_settings->value(KEY_DEFAULT_DELAY_SECONDS, DEFAULT_DELAY_SECONDS).toInt());
}
