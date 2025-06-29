#ifndef IMAGEEDITOR_H
#define IMAGEEDITOR_H

#include "../imagelist.h"
#include <QWidget>


class ImageEditor : public QWidget
{
    Q_OBJECT

    ImageSpec m_image;

public:
    explicit ImageEditor(QWidget *parent = nullptr);

    ImageSpec image() const;
    void setImage(const ImageSpec &image);

};

#endif // IMAGEEDITOR_H