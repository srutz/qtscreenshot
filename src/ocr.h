
#ifndef OCR_H
#define OCR_H

#include <QObject>
#include <QString>
#include <vector>

using namespace std;

/* run ocr in the background */
class Ocr : QObject {
    Q_OBJECT

public:
    Ocr(QObject *parent = nullptr);
    ~Ocr() = default;

    QString performOcr(const QImage &image);
    void setLanguages(const QString &language);

private:
    vector<QString> m_languages;

};

#endif // OCR_H