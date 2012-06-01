#include "data.h"

Data::Data(QObject *parent) :
    QObject(parent)
{
}
int getId(QPoint &p)
{
    return(p.x()+p.y()*1000);
}
void Data::checkNeigh(int i, QPoint &q)
{
    if(index.count(getId(q)))
    {
        neighIn[i]<<index[getId(q)];
    }
    else
    {
        if(indexOut.count(getId(q)))
            neighOut[i]<<indexOut[getId(q)];
        else
        {
            QPoint globalP;
            globalP+=maskPosition;
            globalP+=q;
            QRgb val=desImage.pixel(globalP);
            pixData newPix;
            newPix.p=q;
            newPix.d=val;
            fstar<<newPix;
            indexOut[getId(q)]=fstar.count()-1;

            neighOut[i]<<indexOut[getId(q)];
        }
    }
}
void Data::getVpq()
{
    for(int i=0;i<fp.count();i++)
    {
        int sumR=sumG=sumB=0;
        foreach(int j,neighIn[i])
        {
            QRgb col = fp[j];
            sumR += qRed(col.d);
            sumG += qGreen(col.d);
            sumB += qBlue(col.d);
        }

        foreach(int j,neighOut[i])
        {
            QRgb col = fstar[j];
            sumR += qRed(col.d);
            sumG += qGreen(col.d);
            sumB += qBlue(col.d);
        }

        QRgb val = fp[i];
        sumR -= 4*qRed(val);
        sumG -= 4*qGreen(val);
        sumB -= 4*qBlue(val);

        Int3D NewVpq;
        NewVpq.r=sumR;
        NewVpq.g=sumG;
        NewVpq.b=sumB;

        vpq<<NewVpq;
    }
}

void Data::crackImage()
{
    if(subImage.isNull()||maskImage.isNull())
        return;

    int w, h;

    w=maskImage.width();
    h=maskImage.height();



    for(int y=0;y<h;y++)
    {
        for(int x=0;x<w;x++)
        {
            QRgb val=subImage.pixel(x,y);
           if(qAlpha(val)==255)
           {
               QPoint p(x,y);
               pixData newPix;
               newPix.p=p;
               newPix.d=val;
                fp<<newPix;
                index[getId(newPix.p)]=fp.count()-1;

           }

        }
    }

    for(int i=0;i<fp.count();i++)
    {
        QPoint newNeigh;
        QPoint p;
        p=fp[i].p;
        newNeigh=p+QPoint(1,0);
        checkNeigh(i,newNeigh);
        newNeigh=p+QPoint(0,1);
        checkNeigh(i,newNeigh);
        newNeigh=p+QPoint(-1,0);
        checkNeigh(i,newNeigh);
        newNeigh=p+QPoint(0,-1);
        checkNeigh(i,newNeigh);
    }

    getVpq();

    for(int i=0;i<fp.count();i++)
    {
        Int3D sumOutNeigh;
        sumOutNeigh.r=sumOutNeigh.g=sumOutNeigh.b=0;
        foreach(int j, neighOut[i])
        {
            qRgb col=fstar[j].d;
            sumOutNeigh.r += qRed(col);
            sumOutNeigh.g += qGreen(col);
            sumOutNeigh.b += qBlue(col);

        }

        sumOutNeigh+=vpq[i];
        right<<sumOutNeigh;
    }
}
void Data::test()
{
    QImage t(maskImage.width()+2,maskImage.height()+2,QImage::Format_ARGB32);

    for(int y=-1;y<t.height()-1;y++)
    {
        for(int x=-1;x<t.width()-1;x++)
        {
            QPoint p(x,y);
            if(indexOut.count(getId(p)))
                t.setPixel(x+1,y+1,fstar[indexOut[getId(p)]].d);
            else if(index.count(getId(p)))
            {
                int ind=index[getId(p)];
                int n=neighOut[ind].count();
                qDebug()<<n;
                for(int i=0;i<n;i++)
                {
                    qDebug()<<neighOut[ind][i];

                }
                qDebug();
                if(n!=0)
                    t.setPixel(x+1,y+1,fstar[neighOut[ind][0]].d);

            }
            else
                t.setPixel(x+1,y+1,qRgb(255,0,0));
        }
    }

    testImage = t;
}
void Data::resetData()
{
    fp.clear();
    index.clear();
    fstar.clear();
    indexOut.clear();
    vpq.clear();
    right.clear();
}
