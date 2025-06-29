
#include "ocr.h"

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

QString Ocr::performOcr(const QImage &image) {
    return "";
}

void Ocr::setLanguages(const QString &language) {
    
}
