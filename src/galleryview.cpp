#include "galleryview.h"
#include "toast.h"
#include "util.h"
#include <memory>
#include <QApplication>
#include <QStyle>
#include <QDebug>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileInfo>
#include <QTimer>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>

GalleryView::GalleryView(QWidget *parent)
    : QWidget{parent}
{
    m_imageViewContainer = new QWidget(this);
    for (int i = 0; i < 3; i++) {
        this->m_imageViews.push_back(make_unique<ImageView>(m_imageViewContainer));
    }
    this->m_leftNavigation = new QWidget(this);
    this->m_rightNavigation = new QWidget(this);

    auto setupNavigation = [=,this](QWidget *widget, const QIcon icon, auto onClick) {
        auto layout = new QVBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        widget->setLayout(layout);
        auto button = new QPushButton(widget);
        const int ICON_SIZE = 48;
        button->setIcon(icon);
        button->setFixedSize(ICON_SIZE, ICON_SIZE);
        button->setIconSize(QSize(ICON_SIZE, ICON_SIZE));
        button->setStyleSheet("QPushButton { border: none; }");
        layout->addWidget(button);
        connect(button, &QPushButton::clicked, widget, [=,this]() {
            onClick();
        });
    };
    setupNavigation(m_leftNavigation, QIcon(":/common/resources/images/back_48.png"), [=,this]() { step(-1); });
    setupNavigation(m_rightNavigation, QIcon(":/common/resources/images/forward_48.png"), [=,this]() { step(1); });

    m_statusLabel = new QLineEdit(this);
    m_statusLabel->setMinimumSize(QSize(250, 0));
    m_statusLabel->setPlaceholderText("Image filename");
    //m_statusLabel = new QLabel(this);
    //m_statusLabel->setAlignment(Qt::AlignCenter);
    //m_statusLabel->setTextInteractionFlags(Qt::TextSelectableByKeyboard | Qt::TextSelectableByMouse);
    m_statusLabel->setStyleSheet("QLabel { color : #efefef; padding-bottom: 4px; }");


    auto horizontalWrapper = new QWidget(this);
    auto horizontalLayout = new QHBoxLayout(this);
    horizontalWrapper->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    horizontalWrapper->setLayout(horizontalLayout);
    horizontalLayout->setContentsMargins(0, 0, 0, 0);
    horizontalLayout->addWidget(m_leftNavigation);
    horizontalLayout->addWidget(m_imageViewContainer, 1);
    horizontalLayout->addWidget(m_rightNavigation);

    auto outerLayout = new QVBoxLayout(this);
    this->setLayout(outerLayout);
    horizontalWrapper->setStyleSheet("background-color: #737373");

    m_fileArea = new QWidget(this);
    auto fileLayout = new QHBoxLayout(this);
    m_fileArea->setLayout(fileLayout);
    m_fileArea->setContentsMargins(QMargins(0, 9, 0, 9));
    auto saveButton = new QPushButton("Save", this);
    Util::applyFlatButtonStyle(saveButton);
    auto label = new QLabel("Filename", this);
    fileLayout->addWidget(label);
    fileLayout->addWidget(m_statusLabel);
    fileLayout->addWidget(saveButton);
    fileLayout->addStretch();

    outerLayout->setContentsMargins(0, 4, 0, 4);
    outerLayout->setSpacing(0);
    outerLayout->addWidget(horizontalWrapper);
    outerLayout->addWidget(m_fileArea);
    m_fileArea->setEnabled(false);
    QTimer::singleShot(0, this, [this] { layoutImages(); });

    connect(saveButton, &QPushButton::clicked, this, [=,this]() {
        auto currentImage = m_input->image(m_input->index());
        if (currentImage.m_valid) {
            // save in default pictures directory
            auto picturesDir = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
            auto filename = m_statusLabel->text();
            // combine filename and picturesDir
            auto filePath = QDir(picturesDir).filePath(filename);
            // check if file exists
            QFileInfo fileInfo(filePath);
            if (fileInfo.exists()) {
                auto result = QMessageBox::question(this, "File exists", "File already exists. Overwrite?", QMessageBox::Yes | QMessageBox::No);
                if (result == QMessageBox::No) {
                    return;
                }
            }
            // save the image
            auto result = currentImage.m_pixmap.save(filePath);
            if (result) {
                auto msg = QString::fromUtf8(u8"📸 Image saved to %1").arg(filePath);
                Toast::showToast(this, msg, 3000);
                currentImage.m_filename = filename;
            } else {
                auto msg = QString::fromUtf8(u8"📸 Error saving image to %1").arg(filePath);
                Toast::showToast(this, msg, 3000);
            }   
        }
    }); 
}


void GalleryView::layoutImages()
{
    auto size = m_imageViewContainer->size();
    auto x = -size.width();
    auto y = 0;

    for (auto &view : m_imageViews) {
        view->setFixedSize(size.width(), size.height());
        view->setPosition(QPoint(x, y));
        view->show();
        x += size.width();
    }
}


void GalleryView::resizeEvent(QResizeEvent * event)
{
    layoutImages();
}


void GalleryView::setInput(shared_ptr<ImageList> input)
{
    // disconnect from old input
    if (this->m_input != nullptr) {
        disconnect(this->m_input.get(), &ImageList::statusChanged, this, &GalleryView::rehashImage);
    }
    this->m_input = input;
    // connect to new input
    if (this->m_input != nullptr) {
        connect(this->m_input.get(), &ImageList::statusChanged, this, &GalleryView::rehashImage);
    }
    rehashImage();
}



void GalleryView::step(int direction)
{
    if (m_transitioning) {
        return;
    }
    auto size = m_imageViewContainer->size();
    auto duration = 250;
    if (direction > 0) {
        m_transitioning = true;
        m_imageViews[2]->setPositionA(QPoint(0, 0), duration);
        m_imageViews[1]->setPositionA(QPoint(-size.width(), 0), duration, [=,this] {
            m_input->step(direction);
            rehashImage();
            layoutImages();
            m_transitioning = false;
        });
    } else if (direction < 0) {
        m_transitioning = true;
        m_imageViews[0]->setPositionA(QPoint(0, 0), duration);
        m_imageViews[1]->setPositionA(QPoint(size.width(), 0), duration, [=,this] {
            m_input->step(direction);
            rehashImage();
            layoutImages();
            m_transitioning = false;
        });
    }
}

void GalleryView::rehashImage()
{
    //qDebug() << "rehashImage" << m_input->index() << m_input->size();
    auto currentImage = m_input->image(m_input->index());
    auto prevImage = m_input->image(m_input->index() -1);
    auto nextImage = m_input->image(m_input->index() + 1);
    setImageSpec(prevImage, m_imageViews.at(0).get());
    setImageSpec(currentImage, m_imageViews.at(1).get(), true);
    setImageSpec(nextImage, m_imageViews.at(2).get());

    QPixmap transparent = QPixmap(QSize(48, 48));
    transparent.fill(Qt::transparent);

    bool leftEnabled = m_input->index() > 0;
    bool rightEnabled = m_input->index() < m_input->size() - 1;
    m_leftNavigation->findChild<QPushButton*>()->setIcon(leftEnabled
        ? QIcon(":/common/resources/images/back_48.png") : QIcon(transparent));
    m_leftNavigation->findChild<QPushButton*>()->setEnabled(leftEnabled);
    m_rightNavigation->findChild<QPushButton*>()->setIcon(rightEnabled
        ? QIcon(":/common/resources/images/forward_48.png") : QIcon(transparent));
    m_rightNavigation->findChild<QPushButton*>()->setEnabled(rightEnabled);

    m_fileArea->setEnabled(currentImage.m_valid);
}


void GalleryView::setImageSpec(const ImageSpec &spec, ImageView *imageView, bool applyTitle)
{
    imageView->showPixmap(spec.m_pixmap);
    if (applyTitle) {
        m_statusLabel->setText(spec.m_filename);
    }
}


