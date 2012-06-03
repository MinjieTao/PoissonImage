#ifndef DATA_H
#define DATA_H

#include <QObject>
#include <QMap>
#include <QVector>
#include <QVector3D>
#include <QImage>
#include <QDebug>
#include "sparsematrixsolver.h"
#include "jacobi.h"


typedef struct PixData{
    QPoint p;
    QRgb d;
}pixData;

typedef struct Int3D{
    int r;
    int g;
    int b;
    inline Int3D &operator+=(const Int3D &vector)
    {
        r += vector.r;
        g += vector.g;
        b += vector.b;
        return *this;
    }
}Int3D;

class Data : public QObject
{
    Q_OBJECT
public:
    explicit Data(QObject *parent = 0);

    QMap<int,int> index;
    QVector<pixData> fp;
    QMap<int,int> indexOut;
    QVector<pixData> fstar;


    QImage subImage;
    QImage maskImage;
    QImage srcImage;
    QImage testImage;
    QImage desImage;

    QMap<int,QVector<int> > neighIn;
    QMap<int,QVector<int> > neighOut;

    QPoint maskPosition;
    QPoint srcPosition;
    QVector<Int3D> vpq;
    QVector<pixData> gOmega;
    QVector<pixData> fstarOmega;
    QVector<Int3D> right;




    void init(QImage &sub, QImage &mask,
              QImage &des, QPoint maskPos,
              QImage &src, QPoint srcPos)
    {
        //subImage=sub.convertToFormat(QImage::Format_RGB32);
        subImage=sub;
        maskImage=mask;
        desImage=des;
        maskPosition=maskPos;
        srcImage=src;
        srcPosition=srcPos;
        resetData();
    }

    void checkNeigh(int i, QPoint &q);

    void crackImage(int m);
    void getVpqNaive();
    void getVpq();
    void calculate();
    void solveJacobi(int n);
    void test();
    
signals:
    
public slots:
    void resetData();

    
};

#endif // DATA_H
