#ifndef ABSOLUTELAYOUT_H
#define ABSOLUTELAYOUT_H

#include <QLayout>
#include <QWidget>

class AbsoluteLayout : public QLayout
{
    Q_OBJECT

public:

    enum SizeMode { FULLSIZE, OWNSIZE };

    AbsoluteLayout(QWidget* parent = nullptr) : QLayout(parent) {}
    
    ~AbsoluteLayout() {
        QLayoutItem* item;
        while ((item = takeAt(0))) {
            delete item;
        }
    }

    // Add widget with absolute position
    void addWidget(QWidget* widget, int x, int y, const SizeMode &sizeMode);
    void addWidget(QWidget* widget, const QPoint& position, const SizeMode &sizeMode);
    // QLayout interface
    void addItem(QLayoutItem* item) override;
    QSize sizeHint() const override;
    QSize minimumSize() const override;
    QLayoutItem* itemAt(int index) const override;
    QLayoutItem* takeAt(int index) override;
    int count() const override;
    void setGeometry(const QRect& rect) override;

    // Helper methods
    void setItemPosition(int index, const QPoint& position);
    void setItemPosition(QWidget* widget, const QPoint& position);
    
    // Method to connect marker signals for automatic position updates
    void connectMarker(QWidget* marker);

private:
    class AbsoluteLayoutItem {
    public:
        AbsoluteLayoutItem(QLayoutItem* item, const QPoint& pos, const SizeMode &sizeMode) 
            : m_item(item), m_position(pos), m_sizeMode(sizeMode) {}
        
        AbsoluteLayoutItem(QWidget* widget, const QPoint& pos, const SizeMode &sizeMode) 
            : m_item(new QWidgetItem(widget)), m_position(pos), m_sizeMode(sizeMode) {}
        
        ~AbsoluteLayoutItem() { delete m_item; }
        
        QLayoutItem* layoutItem() const { return m_item; }
        QPoint position() const { return m_position; }
        void setPosition(const QPoint& pos) { m_position = pos; }
        SizeMode sizeMode() const { return m_sizeMode; }
        
        // Release ownership of the layout item
        QLayoutItem* releaseLayoutItem() { 
            QLayoutItem* item = m_item; 
            m_item = nullptr; 
            return item; 
        }
        
    private:
        QLayoutItem* m_item;
        QPoint m_position;
        SizeMode m_sizeMode;
    };
    
    QList<AbsoluteLayoutItem*> itemList;
};

#endif // ABSOLUTELAYOUT_H

