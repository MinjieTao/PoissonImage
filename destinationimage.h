#ifndef DESTINATIONIMAGE_H
#define DESTINATIONIMAGE_H

#include <QWidget>
#include <QPainter>
#include <QFileDialog>
#include <QMouseEvent>
#include <QDebug>

class DestinationImage : public QWidget
{
    Q_OBJECT
public:
    explicit DestinationImage(QWidget *parent = 0);
    QImage destinationImage;
    QImage subImage;
signals:
    void positionChanged();
    
public slots:

protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
public:
    void loadImage();
    QPoint maskPosition;
};

#endif // DESTINATIONIMAGE_H
