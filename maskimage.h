#ifndef MASKIMAGE_H
#define MASKIMAGE_H

#include <QWidget>
#include <QPainter>

class MaskImage : public QWidget
{
    Q_OBJECT
public:
    explicit MaskImage(QWidget *parent = 0);

    QImage maskImage;
    
signals:
    
public slots:

protected:
    void paintEvent(QPaintEvent *);
    
};

#endif // MASKIMAGE_H
