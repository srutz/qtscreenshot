
#ifndef MARKER_H
#define MARKER_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>

class Marker : public QWidget
{
    Q_OBJECT

    QLabel *m_label;
    bool m_dragging;
    QPoint m_dragStartPosition;

public:
    explicit Marker(QWidget *parent = nullptr);

    QSize sizeHint() const override;
    
    void setText(const QString &text) { m_label->setText(text); }

signals:
    void positionChanged(const QPoint &newPosition);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // MARKER_H
