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
void Data::getVpqNaive()
{
    vpq.clear();
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
        sumR = 4*qRed(val.d)-sumR;
        sumG = 4*qGreen(val.d)-sumG;
        sumB = 4*qBlue(val.d)-sumB;

        Int3D NewVpq;
        NewVpq.r=sumR;
        NewVpq.g=sumG;
        NewVpq.b=sumB;
        vpq<<NewVpq;
    }

}

void Data::getVpq()
{
    vpq.clear();
    for(int i=0;i<fp.count();i++)
    {


        int r=0, g=0, b=0;
        foreach(int j,neighIn[i])
        {
            pixData col=fp[j];
            pixData col2=fstarOmega[j];
            int del=qRed(fp[i].d)-qRed(col.d);
            int del2=qRed(fstarOmega[i].d)-qRed(col2.d);

            if((del>0?del:-del)>(del2>0?del2:-del2))
                r+=del;
            else
                r+=del2;

            del=qGreen(fp[i].d)-qGreen(col.d);
            del2=qGreen(fstarOmega[i].d)-qGreen(col2.d);

            if((del>0?del:-del)>(del2>0?del2:-del2))
                g+=del;
            else
                g+=del2;

            del=qBlue(fp[i].d)-qBlue(col.d);
            del2=qBlue(fstarOmega[i].d)-qBlue(col2.d);

            if((del>0?del:-del)>(del2>0?del2:-del2))
                b+=del;
            else
                b+=del2;
        }
        foreach(int j,neighOut[i])
        {
            pixData col=gOmega[j];
            pixData col2=fstar[j];
            int del=qRed(fp[i].d)-qRed(col.d);
            int del2=qRed(fstarOmega[i].d)-qRed(col2.d);

            if((del>0?del:-del)>(del2>0?del2:-del2))
                r+=del;
            else
                r+=del2;

            del=qGreen(fp[i].d)-qGreen(col.d);
            del2=qGreen(fstarOmega[i].d)-qGreen(col2.d);

            if((del>0?del:-del)>(del2>0?del2:-del2))
                g+=del;
            else
                g+=del2;

            del=qBlue(fp[i].d)-qBlue(col.d);
            del2=qBlue(fstarOmega[i].d)-qBlue(col2.d);

            if((del>0?del:-del)>(del2>0?del2:-del2))
                b+=del;
            else
                b+=del2;
        }
        Int3D newVpq;
        newVpq.r=r;
        newVpq.g=g;
        newVpq.b=b;

        vpq<<newVpq;
    }
}

void Data::crackImage(int m)
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
                QRgb val1=desImage.pixel(maskPosition+p);
                pixData newP;
                newP.p=p;
                newP.d=val1;
                fstarOmega<<newP;

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

    if(m==1)
        getVpq();
    else
        getVpqNaive();

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
    int maxOfOrigin=0;int minOfOrigin=255;
    int maxOfNow=0;int minOfNow=255;

    for(int j=0;j<fp.count();j++)
    {
        if(resultR[j]>maxOfNow)
            maxOfNow=resultR[j];
        if(resultR[j]<minOfNow)
            minOfNow=resultR[j];
        if(qRed(fp[j].d)>maxOfOrigin)
            maxOfOrigin=qRed(fp[j].d);
        if(qRed(fp[j].d)<minOfOrigin)
            minOfOrigin=qRed(fp[j].d);
    }
    foreach(int j,resultR)
    {
        j=((j-minOfNow)*1.0)*(maxOfOrigin-minOfOrigin)/(maxOfNow-minOfNow)+minOfOrigin;
    }

    maxOfOrigin=0;
    minOfOrigin=255;
    maxOfNow=0;
    minOfNow=255;

    for(int j=0;j<fp.count();j++)
    {
        if(resultG[j]>maxOfNow)
            maxOfNow=resultG[j];
        if(resultG[j]<minOfNow)
            minOfNow=resultG[j];
        if(qGreen(fp[j].d)>maxOfOrigin)
            maxOfOrigin=qGreen(fp[j].d);
        if(qGreen(fp[j].d)<minOfOrigin)
            minOfOrigin=qGreen(fp[j].d);
    }

    foreach(int j, resultG)
    {
        j=((j-minOfNow)*1.0)*(maxOfOrigin-minOfOrigin)/(maxOfNow-minOfNow)+minOfOrigin;

    }


    maxOfOrigin=0, minOfOrigin=255;
    maxOfNow=0, minOfNow=255;

    for(int j=0;j<fp.count();j++)
    {
        if(resultB[j]>maxOfNow)
            maxOfNow=resultB[j];
        if(resultB[j]<minOfNow)
            minOfNow=resultB[j];
        if(qBlue(fp[j].d)>maxOfOrigin)
            maxOfOrigin=qBlue(fp[j].d);
        if(qBlue(fp[j].d)<minOfOrigin)
            minOfOrigin=qBlue(fp[j].d);
    }
//    foreach(int j, resultG)
//    {
//        if(j>max)
//            max=j;
//        if(j<min)
//            min=j;
//    }
//    foreach(int j, resultB)
//    {
//        if(j>max)
//            max=j;
//        if(j<min)
//            min=j;
//    }




    foreach(int j, resultB)
    {
        j=((j-minOfNow)*1.0)*(maxOfOrigin-minOfOrigin)/(maxOfNow-minOfNow)+minOfOrigin;

    }
    for(int i=0;i<fp.count();i++)
    {
        QRgb newVal=qRgb(resultR[i],resultG[i],resultB[i]);
        fp[i].d=newVal;
    }

}

void Data::solveJacobi(int n)
{
    Jacobi s1,s2,s3;
    QVector<double> r,g,b;
    QVector<int> resultR, resultG, resultB;


    for(int i=0;i<right.count();i++)
    {
        r<<right[i].r;
        g<<right[i].g;
        b<<right[i].b;
    }

    s1.init(neighIn,r);
    s2.init(neighIn,g);
    s3.init(neighIn,b);

    s1.cal(n);
    s2.cal(n);
    s3.cal(n);

    for(int i=0;i<right.count();i++)
    {
        resultR<<s1.x[i];
        resultG<<s2.x[i];
        resultB<<s3.x[i];
    }

    int maxOfOrigin=0;int minOfOrigin=255;
    int maxOfNow=0;int minOfNow=255;

    for(int j=0;j<fp.count();j++)
    {
        if(resultR[j]>maxOfNow)
            maxOfNow=resultR[j];
        if(resultR[j]<minOfNow)
            minOfNow=resultR[j];
        if(qRed(fp[j].d)>maxOfOrigin)
            maxOfOrigin=qRed(fp[j].d);
        if(qRed(fp[j].d)<minOfOrigin)
            minOfOrigin=qRed(fp[j].d);
    }
    foreach(int j,resultR)
    {
        j=((j-minOfNow)*1.0)*(maxOfOrigin-minOfOrigin)/(maxOfNow-minOfNow)+minOfOrigin;
    }

    maxOfOrigin=0;
    minOfOrigin=255;
    maxOfNow=0;
    minOfNow=255;

    for(int j=0;j<fp.count();j++)
    {
        if(resultG[j]>maxOfNow)
            maxOfNow=resultG[j];
        if(resultG[j]<minOfNow)
            minOfNow=resultG[j];
        if(qGreen(fp[j].d)>maxOfOrigin)
            maxOfOrigin=qGreen(fp[j].d);
        if(qGreen(fp[j].d)<minOfOrigin)
            minOfOrigin=qGreen(fp[j].d);
    }

    foreach(int j, resultG)
    {
        j=((j-minOfNow)*1.0)*(maxOfOrigin-minOfOrigin)/(maxOfNow-minOfNow)+minOfOrigin;

    }


    maxOfOrigin=0, minOfOrigin=255;
    maxOfNow=0, minOfNow=255;

    for(int j=0;j<fp.count();j++)
    {
        if(resultB[j]>maxOfNow)
            maxOfNow=resultB[j];
        if(resultB[j]<minOfNow)
            minOfNow=resultB[j];
        if(qBlue(fp[j].d)>maxOfOrigin)
            maxOfOrigin=qBlue(fp[j].d);
        if(qBlue(fp[j].d)<minOfOrigin)
            minOfOrigin=qBlue(fp[j].d);
    }
//    foreach(int j, resultG)
//    {
//        if(j>max)
//            max=j;
//        if(j<min)
//            min=j;
//    }
//    foreach(int j, resultB)
//    {
//        if(j>max)
//            max=j;
//        if(j<min)
//            min=j;
//    }




    foreach(int j, resultB)
    {
        j=((j-minOfNow)*1.0)*(maxOfOrigin-minOfOrigin)/(maxOfNow-minOfNow)+minOfOrigin;

    }

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
                /*t.setPixel(p,qRgb(vpq[ind].r+128,
                                  vpq[ind].g+128,
                                  vpq[ind].b+128));
*/
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
    fstar.clear();
    fstarOmega.clear();
}
