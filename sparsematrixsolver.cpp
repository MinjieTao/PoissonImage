#include "sparsematrixsolver.h"

SparseMatrixSolver::SparseMatrixSolver(QObject *parent) :
    QObject(parent)
{
}

void SparseMatrixSolver::init(QMap<int, QVector<int> > &matrixA)
{

    n=matrixA.count();




    for(int i=0;i<n;i++)
    {
        ia<<ja.count();
        a<<4;//a[i][i]=4;
        ja<<i;

        foreach(int j,matrixA[i])
        {
            if(j>i)//upper triangle
            {
                a<<-1;
                ja<<j;
            }
        }
    }
}
int SparseMatrixSolver::presolve()
{
    ia<<ja.count();
    nnz=ia[n];

    x.resize(n);

    error=0;
    solver=1;//multi-recursive iterative solver
    pardisoinit (pt,  &mtype, &solver, iparm, dparm, &error);

    dparm[1]=10000;

    if (error != 0)
    {
        if (error == -10 )
           qDebug("No license file found \n");
        if (error == -11 )
           qDebug("License is expired \n");
        if (error == -12 )
           qDebug("Wrong username or hostname \n");
         return 1;
    }
    else
        qDebug("[PARDISO]: License check was successful ... \n");


    iparm[2]  = 1;
    iparm[8] = 2;
    mtype = -2;        /* Real symmetric matrix */
    nrhs = 1;          /* Number of right hand sides. */

    maxfct = 1;		/* Maximum number of numerical factorizations.  */
    mnum   = 1;         /* Which factorization to use. */

    msglvl = 1;         /* Print statistical information  */
    error  = 0;         /* Initialize error flag */

/* -------------------------------------------------------------------- */
/* ..  Convert matrix from 0-based C-notation to Fortran 1-based        */
/*     notation.                                                        */
/* -------------------------------------------------------------------- */
    for (i = 0; i < n+1; i++) {
        ia[i] += 1;
    }
    for (i = 0; i < nnz; i++) {
        ja[i] += 1;
    }

/* -------------------------------------------------------------------- */
/*  .. pardiso_chk_matrix(...)                                          */
/*     Checks the consistency of the given matrix.                      */
/*     Use this functionality only for debugging purposes               */
/* -------------------------------------------------------------------- */

    pardiso_chkmatrix  (&mtype, &n, &a.first(), &ia.first(), &ja.first(), &error);
    if (error != 0) {
        qDebug("\nERROR in consistency of matrix: %d", error);
        exit(1);
    }



/* -------------------------------------------------------------------- */
/* ..  Reordering and Symbolic Factorization.  This step also allocates */
/*     all memory that is necessary for the factorization.              */
/* -------------------------------------------------------------------- */
    phase = 11;

    pardiso (pt, &maxfct, &mnum, &mtype, &phase,
         &n, &a.first(), &ia.first(), &ja.first(), &idum, &nrhs,
             iparm, &msglvl, &ddum, &ddum, &error, dparm);

    if (error != 0) {
        qDebug("\nERROR during symbolic factorization: %d", error);
        exit(1);
    }
    qDebug("\nReordering completed ... ");
    qDebug("\nNumber of nonzeros in factors  = %d", iparm[17]);
    qDebug("\nNumber of factorization MFLOPS = %d", iparm[18]);

/* -------------------------------------------------------------------- */
/* ..  Numerical factorization.                                         */
/* -------------------------------------------------------------------- */
    phase = 22;
    iparm[32] = 1; /* compute determinant */

    pardiso (pt, &maxfct, &mnum, &mtype, &phase,
             &n, &a.first(), &ia.first(), &ja.first(), &idum, &nrhs,
             iparm, &msglvl, &ddum, &ddum, &error,  dparm);

    if (error != 0) {
        qDebug("\nERROR during numerical factorization: %d", error);
        exit(2);
    }
    qDebug("\nFactorization completed ...\n ");

}

int SparseMatrixSolver::solve(QVector<double> &b)
{
    /* -------------------------------------------------------------------- */
    /* ..  pardiso_chkvec(...)                                              */
    /*     Checks the given vectors for infinite and NaN values             */
    /*     Input parameters (see PARDISO user manual for a description):    */
    /*     Use this functionality only for debugging purposes               */
    /* -------------------------------------------------------------------- */

        pardiso_chkvec (&n, &nrhs, &b.first(), &error);
        if (error != 0) {
            qDebug("\nERROR  in right hand side: %d", error);
            exit(1);
        }

    /* -------------------------------------------------------------------- */
    /* .. pardiso_printstats(...)                                           */
    /*    prints information on the matrix to STDOUT.                       */
    /*    Use this functionality only for debugging purposes                */
    /* -------------------------------------------------------------------- */

        pardiso_printstats (&mtype, &n, &a.first(), &ia.first(), &ja.first(), &nrhs, &b.first(), &error);
        if (error != 0) {
            qDebug("\nERROR right hand side: %d", error);
            exit(1);
        }
    /* -------------------------------------------------------------------- */
    /* ..  Back substitution and iterative refinement.                      */
    /* -------------------------------------------------------------------- */
        phase = 33;

        iparm[32] = 1;       /* Max numbers of iterative refinement steps. */

        pardiso (pt, &maxfct, &mnum, &mtype, &phase,
                 &n, &a.first(), &ia.first(), &ja.first(), &idum, &nrhs,
                 iparm, &msglvl, &b.first(), &x.first(), &error,  dparm);

        qDebug()<<dparm[35];

        if (error != 0) {
            printf("\nERROR during solution: %d", error);
            exit(3);
        }

        printf("\nSolve completed ... ");
        printf("\nThe solution of the system is: ");
//        for (i = 0; i < n; i++) {
//            printf("\n x [%d] = % f", i, x[i] );
//        }
//        printf ("\n");

    /* -------------------------------------------------------------------- */
    /* ..  Convert matrix back to 0-based C-notation.                       */
    /* -------------------------------------------------------------------- */
        for (i = 0; i < n+1; i++) {
            ia[i] -= 1;
        }
        for (i = 0; i < nnz; i++) {
            ja[i] -= 1;
        }
}

int SparseMatrixSolver::postsolve()
{
    /* -------------------------------------------------------------------- */
    /* ..  Termination and release of memory.                               */
    /* -------------------------------------------------------------------- */
        phase = -1;                 /* Release internal memory. */

        pardiso (pt, &maxfct, &mnum, &mtype, &phase,
                 &n, &ddum, &ia.first(), &ja.first(), &idum, &nrhs,
                 iparm, &msglvl, &ddum, &ddum, &error,  dparm);
        return 0;
}
