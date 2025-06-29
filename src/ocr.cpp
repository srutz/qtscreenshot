
#include "ocr.h"
#include <QProcess>
#include <QDebug>

Ocr::Ocr(QObject *parent)
    : QObject(parent)
{
    m_languages.push_back("eng");
    m_languages.push_back("deu");
    m_languages.push_back("fra");
    m_languages.push_back("ita");
    m_languages.push_back("spa");
    m_languages.push_back("rus");
}

void Ocr::setLanguages(const QString &language) {
    
}

void Ocr::startWork() {
    // run ocr process in the shell
    //QString command = QString("tesseract -l %1 %2 stdout")
    //    .arg(m_languages.join("+"))
    //    .arg(image);
    QString command = QString("/bin/cat /etc/hosts");
    QProcess process;
    process.start("/bin/sh", QStringList() << "-c" << command);
    if (!process.waitForFinished()) {
        qWarning() << "OCR process failed to start or finish.";
        return;
    }
    OcrResult result =  {
        .exitCode = process.exitCode(),
        .text = QString::fromUtf8(process.readAllStandardOutput())
    };
    emit completed(result);
}
