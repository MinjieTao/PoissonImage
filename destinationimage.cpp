#include "destinationimage.h"

DestinationImage::DestinationImage(QWidget *parent) :
    QWidget(parent)
{
}

void DestinationImage::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    if(!destinationImage.isNull())
        painter.drawImage(0,0,destinationImage);
    if(!subImage.isNull())
        painter.drawImage(maskPosition,subImage);
}

void DestinationImage::loadImage()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("select source"));

    if(!fileName.isEmpty())
    {
        destinationImage.load(fileName);
    }

}

void DestinationImage::mouseMoveEvent(QMouseEvent *event)
{

    if(event->buttons()&Qt::LeftButton)
    {
        QPoint halfMask(subImage.width()/2,subImage.height()/2);
        maskPosition=event->pos();
        maskPosition-=halfMask;
    }
    this->update();
}
void DestinationImage::mousePressEvent(QMouseEvent *event)
{

    if(event->button()==Qt::LeftButton)
    {
        QPoint halfMask(subImage.width()/2,subImage.height()/2);
        maskPosition=event->pos();
        maskPosition-=halfMask;
    }
    qDebug()<<maskPosition;
    this->update();
}
