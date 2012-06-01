#include "maskimage.h"

MaskImage::MaskImage(QWidget *parent) :
    QWidget(parent)
{
}

void MaskImage::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawImage(0,0,maskImage);
}
