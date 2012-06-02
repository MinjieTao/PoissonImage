#include "sourceimage.h"

SourceImage::SourceImage(QWidget *parent) :
    QWidget(parent)
{
}

void SourceImage::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    if(!sourceImage.isNull())
        painter.drawImage(0,0,sourceImage);

    painter.setPen(Qt::red);
    painter.drawPolygon(boundary);
}

void SourceImage::loadImage()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("select source"));

    if(!fileName.isEmpty())
    {
        sourceImage.load(fileName);
        qDebug()<<sourceImage.format();
    }

}
void SourceImage::linkPoint(QPoint a, QPoint b)
{
    QPoint del=b;
    del-=a;
    if(del.x()<=1&&del.x()>=-1
            &&del.y()<=1&&del.y()>=-1)
        return;
    if(del.rx()>1)
        del.rx()=1;
    if(del.rx()<-1)
        del.rx()=-1;
    if(del.ry()>1)
        del.ry()=1;
    if(del.ry()<-1)
        del.ry()=-1;
    a+=del;
    boundary<<a;
    linkPoint(a,b);
}

void SourceImage::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() &Qt::LeftButton)
    {
        if(boundary.isEmpty())
        {
            boundary<<event->pos();
            return;
        }

        QPoint oldpos = boundary.last();
        QPoint newpos = event->pos();
        qDebug()<<event->pos();
        if(((oldpos.rx()-newpos.rx())<=1)
                &&((oldpos.rx()-newpos.rx())>=-1)
                &&((oldpos.ry()-newpos.ry())<=1)
                &&((oldpos.ry()-newpos.ry())>=-1))
            boundary<<newpos;
        else
        {
            linkPoint(oldpos, newpos);
            boundary<<newpos;
        }
        this->update();
    }
}


void SourceImage::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        boundary.clear();
        emit resetSub();
        max_x=0;
        max_y=0;
        min_x=sourceImage.width();
        min_y=sourceImage.height();
    }
}
void SourceImage::mouseReleaseEvent(QMouseEvent *)
{
    linkPoint(boundary.last(),boundary.first());

    for(int i=0; i<boundary.count();i++)
    {
        if(boundary[i].rx()<min_x)
            min_x=boundary[i].rx();
        if(boundary[i].rx()>max_x)
            max_x=boundary[i].rx();
        if(boundary[i].ry()<min_y)
            min_y=boundary[i].ry();
        if(boundary[i].ry()>max_y)
            max_y=boundary[i].ry();
    }
    srcPosition.rx()=min_x;
    srcPosition.ry()=min_y;
    QVector<QPoint> inner;
    inner.resize(max_y-min_y+1);
    for(int i=0;i<(max_y-min_y+1);i++ )
    {
        inner[i].rx()=-1;
        inner[i].ry()=-1;
    }

    for(int i=0;i<boundary.count();i++)
    {
        if(inner[boundary[i].y()-min_y].x()<0)
            inner[boundary[i].y()-min_y].rx()=inner[boundary[i].y()-min_y].ry()=boundary[i].x()-min_x;
        else if(inner[boundary[i].y()-min_y].x()>(boundary[i].x()-min_x))
        {
            inner[boundary[i].y()-min_y].rx()=(boundary[i].x()-min_x);
        }
        else if(inner[boundary[i].y()-min_y].y()<(boundary[i].x()-min_x))
            inner[boundary[i].y()-min_y].ry()=(boundary[i].x()-min_x);
    }



    QImage mask((max_x-min_x+1),(max_y-min_y+1),QImage::Format_Mono);
    mask.fill(0);

    for(int i=0; i<boundary.count();i++)
    {
        mask.setPixel(boundary[i].rx()-min_x,
                      boundary[i].ry()-min_y,1);
    }

    for(int i=0;i<=(max_y-min_y);i++)
    {
        int left=inner[i].x();
        int right=inner[i].y();

        while((++left)<right)
        {
            mask.setPixel(left,i,1);
        }
    }


    maskImage = mask;
    subImage = sourceImage.copy(min_x, min_y,
                                (max_x-min_x+1),
                                (max_y-min_y+1));

    subImage = subImage.convertToFormat(QImage::Format_ARGB32);

    subImage.setAlphaChannel(mask);

}
