#ifndef IMAGELIST_H
#define IMAGELIST_H

#include <QObject>
#include <QPixmap>
#include <QString>
#include <memory>

using namespace std;

struct ImageSpec {
    bool m_valid = false;
    QPixmap m_pixmap = QPixmap();
    QString m_filename = "";
};

class ImageList : public QObject
{
    Q_OBJECT
    vector<ImageSpec> m_images;
    int m_index = -1;


public:
    static shared_ptr<ImageList> instance();

    explicit ImageList(QObject *parent = nullptr);

    ImageSpec image(int index) const;
    int index() const;
    void setIndex(int index);
    ImageSpec currentImage() const;

    void addImage(const ImageSpec &image);
    void removeImage(int index);
    int size() const;
    void clear();
    void step(int direction);

signals:
    void statusChanged();

};    


#endif // IMAGELIST_H
