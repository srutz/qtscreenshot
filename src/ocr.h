
#ifndef OCR_H
#define OCR_H

#include <QObject>
#include <QString>
#include <QImage>
#include <vector>

using namespace std;

struct OcrResult {
    int exitCode;
    QString text;
};

/* run ocr in the background */
class Ocr : public QObject {
    Q_OBJECT

public:
    Ocr(QObject *parent = nullptr);
    ~Ocr() = default;

    const QImage &image() const { return m_image; }
    void setImage(const QImage &image) { m_image = image; }

    void setLanguages(const QString &language);

public slots:
    void startWork();

signals:
    void completed(OcrResult result);

private:
    QImage m_image;
    vector<QString> m_languages;

};

#endif // OCR_H