
#include "imageeditor.h"

ImageEditor::ImageEditor(QWidget *parent)
    : QWidget(parent)
{
}

ImageSpec ImageEditor::image() const
{
    return m_image;
}

void ImageEditor::setImage(const ImageSpec &image)
{
    m_image = image;
}