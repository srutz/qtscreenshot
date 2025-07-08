
#ifndef MARKER_H
#define MARKER_H

#include <QWidget>
#include <QLabel>

class Marker : public QWidget
{
    Q_OBJECT

    QLabel *m_label;

public:
    explicit Marker(QWidget *parent = nullptr);

    QSize sizeHint() const override;
    
    void setText(const QString &text) { m_label->setText(text); }
};

#endif // MARKER_H
