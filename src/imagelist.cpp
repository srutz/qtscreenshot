
#include "imagelist.h"

shared_ptr<ImageList> ImageList::instance() {
    static std::shared_ptr<ImageList> instance(new ImageList());
    return instance;
}

ImageList::ImageList(QObject *parent) : QObject(parent) {

}

ImageSpec ImageList::image(int index) const {
    if (index < 0) {
        index += m_images.size();
    }
    if (index >= 0 && index < m_images.size()) {
        return m_images[index];
    }
    return ImageSpec();
}

ImageSpec ImageList::currentImage() const {
    if (m_index >= 0 && m_index < m_images.size()) {
        return m_images[m_index];
    }
    return ImageSpec();
}

int ImageList::index() const {
    return m_index;
}

void ImageList::setIndex(int index) {
    if (index >= 0 && index < m_images.size()) {
        m_index = index;
        emit statusChanged();
    }
}

void ImageList::addImage(const ImageSpec &image) {
    m_images.push_back(image);
    if (m_images.size() == 1) {
        m_index = 0;
    }
    emit statusChanged();
}

void ImageList::removeImage(int index) {
    if (index >= 0 && index < m_images.size()) {
        m_images.erase(m_images.begin() + index);
        emit statusChanged();
    }
}

int ImageList::size() const {
    return m_images.size();
}

void ImageList::clear() {
    m_images.clear();
    emit statusChanged();
}

void ImageList::step(int direction) {
    if (m_images.empty()) {
        return;
    }
    
    if (direction > 0) {
        m_index = (m_index + 1) % m_images.size();
    } else if (direction < 0) {
        m_index = (m_index - 1 + m_images.size()) % m_images.size();
    }
    emit statusChanged();
}