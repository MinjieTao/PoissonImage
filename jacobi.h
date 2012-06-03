#ifndef JACOBI_H
#define JACOBI_H

#include <QObject>
#include <QMap>
#include <QVector>
class Jacobi : public QObject
{
    Q_OBJECT
public:
    explicit Jacobi(QObject *parent = 0);

    QMap<int, QVector<int> > matrixA;
    QVector<double> b;
    QVector<double> x;
    QVector<double> cov;
    QVector<double> error;
    QVector<double> bMinusAx;
    int n;

    void init(QMap<int, QVector<int> > &mA, QVector<double> &b);
    void compute();
    void cal();
    void cal(int times);
    
signals:
    
public slots:
    
};

#endif // JACOBI_H
