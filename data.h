#ifndef DATA_H
#define DATA_H

#include <QObject>
#include <QMap>
#include <QVector>
#include <QImage>
#include <QDebug>


typedef struct PixData{
    QPoint p;
    QRgb d;
}pixData;


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
    QImage testImage;
    QImage desImage;

    QMap<int,QVector<int> > neighIn;
    QMap<int,QVector<int> > neighOut;

    QPoint maskPosition;




    void init(QImage &sub, QImage &mask,
              QImage &des, QPoint maskPos)
    {
        //subImage=sub.convertToFormat(QImage::Format_RGB32);
        subImage=sub;
        maskImage=mask;
        desImage=des;
        maskPosition=maskPos;
        resetData();
    }

    void checkNeigh(int i, QPoint &q);

    void crackImage();
    void test();
    
signals:
    
public slots:
    void resetData();
    void resetBoundary()
    {
        resetData();
        crackImage();

    }
    
};

#endif // DATA_H
