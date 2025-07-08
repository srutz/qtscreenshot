

#include "absolutelayout.h"

// Add widget with absolute position
void AbsoluteLayout::addWidget(QWidget *widget, int x, int y)
{
    addChildWidget(widget);
    itemList.append(new AbsoluteLayoutItem(widget, QPoint(x, y)));
}

void AbsoluteLayout::addWidget(QWidget *widget, const QPoint &position)
{
    addWidget(widget, position.x(), position.y());
}

// Required QLayout interface
void AbsoluteLayout::addItem(QLayoutItem *item)
{
    itemList.append(new AbsoluteLayoutItem(item, QPoint(0, 0)));
}

QSize AbsoluteLayout::sizeHint() const
{
    return minimumSize();
}

QSize AbsoluteLayout::minimumSize() const
{
    QSize size;
    for (const auto *item : itemList)
    {
        QPoint pos = item->position();
        QSize itemSize = item->layoutItem()->minimumSize();
        size = size.expandedTo(QSize(pos.x() + itemSize.width(),
                                     pos.y() + itemSize.height()));
    }
    return size;
}

QLayoutItem* AbsoluteLayout::itemAt(int index) const
{
    if (index >= 0 && index < itemList.size())
    {
        return itemList.at(index)->layoutItem();
    }
    return nullptr;
}

QLayoutItem* AbsoluteLayout::takeAt(int index)
{
    if (index >= 0 && index < itemList.size())
    {
        AbsoluteLayoutItem *layoutItem = itemList.takeAt(index);
        QLayoutItem *item = layoutItem->layoutItem();
        delete layoutItem;
        return item;
    }
    return nullptr;
}

int AbsoluteLayout::count() const 
{
    return itemList.size();
}

void AbsoluteLayout::setGeometry(const QRect &rect)
{
    QLayout::setGeometry(rect);

    // Position all items
    for (auto *item : itemList)
    {
        QPoint pos = item->position();
        QSize size = item->layoutItem()->sizeHint();

        // You can add logic here for relative positioning, scaling, etc.
        item->layoutItem()->setGeometry(QRect(pos, size));
    }
}

// Helper methods
void AbsoluteLayout::setItemPosition(int index, const QPoint &position)
{
    if (index >= 0 && index < itemList.size())
    {
        itemList[index]->setPosition(position);
        update();
    }
}

void AbsoluteLayout::setItemPosition(QWidget *widget, const QPoint &position)
{
    for (auto *item : itemList)
    {
        if (item->layoutItem()->widget() == widget)
        {
            item->setPosition(position);
            update();
            break;
        }
    }
}
