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
            globalP=srcPosition;
            globalP+=q;
            QRgb val2=srcImage.pixel(globalP);
            pixData newP;
            newP.p=q;
            newP.d=val2;
            gOmega<<newP;
            indexOut[getId(q)]=fstar.count()-1;

            neighOut[i]<<indexOut[getId(q)];
        }
    }
}

void Data::getVpq()
{
    for(int i=0;i<fp.count();i++)
    {
        int sumR,sumG,sumB;
        sumR=sumG=sumB=0;
        foreach(int j,neighIn[i])
        {
            pixData col = fp[j];
            sumR += qRed(col.d);
            sumG += qGreen(col.d);
            sumB += qBlue(col.d);
        }

        foreach(int j,neighOut[i])
        {
            pixData col = gOmega[j];
            sumR += qRed(col.d);
            sumG += qGreen(col.d);
            sumB += qBlue(col.d);
        }

        pixData val = fp[i];
        sumR -= 4*qRed(val.d);
        sumG -= 4*qGreen(val.d);
        sumB -= 4*qBlue(val.d);

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
        newNeigh=p+QPoint(0,-1);
        checkNeigh(i,newNeigh);
        newNeigh=p+QPoint(-1,0);
        checkNeigh(i,newNeigh);
        newNeigh=p+QPoint(1,0);
        checkNeigh(i,newNeigh);
        newNeigh=p+QPoint(0,1);
        checkNeigh(i,newNeigh);
    }

    getVpq();

    for(int i=0;i<fp.count();i++)
    {
        Int3D sumOutNeigh;
        sumOutNeigh.r=sumOutNeigh.g=sumOutNeigh.b=0;
        foreach(int j, neighOut[i])
        {
            QRgb col=fstar[j].d;
            sumOutNeigh.r += qRed(col);
            sumOutNeigh.g += qGreen(col);
            sumOutNeigh.b += qBlue(col);

        }

        sumOutNeigh+=vpq[i];
        right<<sumOutNeigh;
    }
}
void Data::calculate()
{

    SparseMatrixSolver solver;
    QVector<double> r,g,b;
    QVector<int> resultR, resultG, resultB;

    solver.init(neighIn);
    solver.presolve();
    for(int i=0;i<right.count();i++)
    {
        r<<right[i].r;
        g<<right[i].g;
        b<<right[i].b;
    }

    solver.solve(r);
    foreach(double j,solver.x)
    {
        resultR<<j;
    }

    solver.postsolve();
    SparseMatrixSolver solver1;
    solver1.init(neighIn);

    solver1.presolve();
    solver1.solve(g);
    foreach(double j,solver1.x)
    {
        resultG<<j;
    }
    solver1.postsolve();
    SparseMatrixSolver solver2;
    solver2.init(neighIn);
    solver2.presolve();
    solver2.solve(b);
    foreach(double j,solver2.x)
    {
        resultB<<j;
    }
    solver2.postsolve();
//    int max=0, min=255;

    foreach(int j,resultR)
    {
        j=j>255?255:(j<0?0:j);
    }
//    foreach(int j,resultR)
//    {
//        j=((j-min)*1.0)*255/(max-min);
//    }
//    max=0;
//    min=255;
    foreach(int j, resultG)
    {
        j=j>255?255:(j<0?0:j);
    }
//    foreach(int j, resultG)
//    {
//         j=((j-min)*1.0)*255/(max-min);
//    }

    foreach(int j, resultB)
    {
        j=j>255?255:(j<0?0:j);
    }





//    foreach(int j, resultB)
//    {
//         j=((j-min)*1.0)*255/(max-min);
//    }

    for(int i=0;i<fp.count();i++)
    {
        QRgb newVal=qRgb(resultR[i],resultG[i],resultB[i]);
        fp[i].d=newVal;
    }

}

void Data::test()
{
    QImage t(maskImage.width(),maskImage.height(),QImage::Format_ARGB32);

    for(int y=0;y<t.height();y++)
    {
        for(int x=0;x<t.width();x++)
        {
            QPoint p(x,y);

            if(index.count(getId(p)))
            {
                int ind=index[getId(p)];
                t.setPixel(x,y,fp[ind].d);

            }
            else
                t.setPixel(x,y,qRgba(0,0,0,0));
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
    neighIn.clear();
    neighOut.clear();
    gOmega.clear();
}
