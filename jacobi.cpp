#include "jacobi.h"

Jacobi::Jacobi(QObject *parent) :
    QObject(parent)
{
}

void Jacobi::init(QMap<int, QVector<int> > &mA, QVector<double> &b)
{
    matrixA=mA;
    this->b=b;
    x.resize(b.size());
    x.fill(0);

    n=b.size();
    error.resize(n);
    cov.resize(n);
    error.fill(0);
    bMinusAx.resize(n);
}

void Jacobi::compute()
{
    for(int i=0;i<n;i++)
    {
        double left;
        left=0;
        foreach(int j, matrixA[i])
        {

            left-=x[j];
        }

        x[i]=0.5*x[i]+0.5*(b[i]-left)*1.0/4;
        error[i]=b[i]-left-4*x[i];

    }

//    for(int i=0;i<n;i++)
//    {
//        xpre[i]=x[i];
//    }
}

void Jacobi::cal()
{
    while(1)
    {
        compute();
        for(int i=0;i<error.size();i++)
        {
            cov[i]=error[i]*error[i];
        }
        double sum;
        foreach(double j, cov)
        {
            sum+=j;
        }
        sum/=error.size();
        if(sum<=0.1)
        {
            break;
        }
    }
}
void Jacobi::cal(int t)
{
    for(int i=0;i<t*10;i++)
    {
        compute();
    }
}
