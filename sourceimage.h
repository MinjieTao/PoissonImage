#ifndef SOURCEIMAGE_H
#define SOURCEIMAGE_H

#include <QPainter>
#include <QWidget>
#include <QFileDialog>
#include <QMouseEvent>
#include <QVector>
#include <QDebug>

class SourceImage : public QWidget
{
    Q_OBJECT
public:
    explicit SourceImage(QWidget *parent = 0);


    QImage sourceImage;
    QImage maskImage;
    QImage subImage;
    QVector<QPoint> boundary;

signals:
    void resetSub();
    
public slots:

protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

private:
    void linkPoint(QPoint a, QPoint b);
    int min_x;
    int min_y;
    int max_x;
    int max_y;

public:
    void loadImage();
};

#endif // SOURCEIMAGE_H
