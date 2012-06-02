#ifndef SPARSEMATRIXSOLVER_H
#define SPARSEMATRIXSOLVER_H

#include <QObject>
#include <QMap>
#include <QVector>
#include <QDebug>

extern "C" void pardisoinit (void   *, int    *,   int *, int *, double *, int *);
extern "C" void pardiso     (void   *, int    *,   int *, int *,    int *, int *,
                  double *, int    *,    int *, int *,   int *, int *,
                     int *, double *, double *, int *, double *);
extern "C" void pardiso_chkmatrix  (int *, int *, double *, int *, int *, int *);
extern "C" void pardiso_chkvec     (int *, int *, double *, int *);
extern "C" void pardiso_printstats (int *, int *, double *, int *, int *, int *,
                           double *, int *);


class SparseMatrixSolver : public QObject
{
    Q_OBJECT
public:
    explicit SparseMatrixSolver(QObject *parent = 0);


    QVector<double> x;

    void init(QMap<int, QVector<int> > &mA);
    int solve(QVector<double> &b);
    int postsolve();
    int presolve();


signals:
    
public slots:
private:

    QVector<double> a;
    QVector<int> ia;
    QVector<int> ja;
    int n;
    int nnz;
    int mtype;
    int nrhs;


    void *pt[64];
    int iparm[64];
    double dparm[64];

    int maxfct, mnum, phase;
    int error,msglvl,solver;


    int num_procs;

    char *var;
    int i;
    double ddum;
    int idum;


    
};

#endif // SPARSEMATRIXSOLVER_H
