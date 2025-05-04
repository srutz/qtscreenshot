#ifndef GALLERYVIEW_H
#define GALLERYVIEW_H

#include "imagelist.h"
#include "imageview.h"
#include <memory>
#include <vector>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>

using namespace std;



/*
 * Shows an image-gallery. this means one image of the gallery
 * is shown and others are accessible via buttons and can be
 * navigated to
 */
class GalleryView : public QWidget
{
    Q_OBJECT

    shared_ptr<ImageList> m_input;
    vector<unique_ptr<ImageView>> m_imageViews; // 3 image views at a time
    QWidget *m_leftNavigation;
    QWidget *m_rightNavigation;
    QWidget *m_imageViewContainer;
    QLineEdit *m_statusLabel;
    QWidget *m_fileArea;
    bool m_transitioning = false;

    void setImageSpec(const ImageSpec &spec, ImageView *imageView, bool appylTitle = false);
    void layoutImages();
    void rehashImage();

public:
    explicit GalleryView(QWidget *parent = nullptr);
    void resizeEvent(QResizeEvent *event) override;

    void step(int direction);
    void setInput(shared_ptr<ImageList>);


signals:
};

#endif // GALLERYVIEW_H
