// configmanager.h
#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QObject>
#include <QString>
#include <QSettings>
#include <QStandardPaths>
#include <QDir>

class ConfigManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString filenameMask READ filenameMask WRITE setFilenameMask NOTIFY filenameMaskChanged)
    Q_PROPERTY(int defaultDelaySeconds READ defaultDelaySeconds WRITE setDefaultDelaySeconds NOTIFY defaultDelaySecondsChanged)

    static constexpr const char* SETTINGS_FILENAME = ".qtscreenshot.settings";

    static constexpr const char* KEY_FILENAME_MASK = "filenameMask";
    static constexpr const char* DEFAULT_FILENAME_MASK = "screenshot_%d.txt";

    static constexpr const char* KEY_DEFAULT_DELAY_SECONDS = "defaultDelaySeconds";
    static constexpr int DEFAULT_DELAY_SECONDS = 0;


public:
    static ConfigManager& instance();

    QString filenameMask() const;
    void setFilenameMask(const QString &filenameMask);

    int defaultDelaySeconds() const;
    void setDefaultDelaySeconds(int seconds);

    void saveSettings();
    void loadSettings();

signals:
    void filenameMaskChanged(const QString &filenameMask);
    void defaultDelaySecondsChanged(int seconds);

private:
    explicit ConfigManager(QObject *parent = nullptr);
    ~ConfigManager() = default;
    
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;
    ConfigManager(ConfigManager&&) = delete;
    ConfigManager& operator=(ConfigManager&&) = delete;

    QString m_filenameMask;
    int m_defaultDelaySeconds;
    QSettings *m_settings;

};

#endif // CONFIGMANAGER_H

