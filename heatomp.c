/*
 * heat transfer simulation using Jacobi iteration
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define max(a, b) ((a) >= (b) ? (a) : (b))

/*
 * printMap - spit out the values as CSV
 *
 * prints m elements per row, n-many rows
 */
int k = 0;
void printMap(double **A, int n, int m)
{
    int i, j;
    char filebuf[32];
    snprintf(filebuf, sizeof(char) * 32, "heatput%i", k);
    FILE *f = fopen(filebuf, "w");
    for (j = 0; j < n; j++)
    {
        for (i = 0; i < m - 1; i++)
        {
            fprintf(f, "%d ", (int)A[j][i]);
        }
        // last one in the row:
        // w/o comma but w/ newline
        fprintf(f, "%d\n", (int)A[j][i]);
    }
    k++;
    fclose(f);
}

/*
 * set the boundary to hot edges but cool top
 * n is row size
 * m is column size
 */
void setBoundary(double **A, int n, int m)
{
    int i, j;
    // set top and bottom to 20 degrees
    for (j = 0; j < m; j++)
    {
        A[0][j] = 20.0;
        A[n - 1][j] = 20.0;
    }

    for (i = 0; i < n; i++)
    {
        A[i][0] = 90.0;
        A[i][m - 1] = 90.0;
    }
    // set sides to 90 degrees

    // DONE:  set the top, bottom, and the sides

} // setBoundary

/*
 * implement the cooling posts
 * assume grid is big enough that 10x10 isn't a problem
 */
void coolingPosts(double **A, int n, int m)
{
    int i, j;
    int leftX = (1 / 3.0) * n;
    int rightX = (2 / 3.0) * n;
    int topY = (1 / 3.0) * m;
    int bottomY = (2 / 3.0) * m;
    // printf("n: %d, m: %d\nlX: %d, rX: %d, tY: %d, bY: %d\n",n,m,leftX,rightX,topY,bottomY);
    for (i = 0; i < 10; i++)
    {
        for (j = 0; j < 10; j++)
        {
            A[leftX - 5 + i][topY - 5 + j] = 10;
            A[rightX - 5 + i][topY - 5 + j] = 10;
            A[leftX - 5 + i][bottomY - 5 + j] = 10;
            A[rightX - 5 + i][bottomY - 5 + j] = 10;
        }
    }
} // coolingPosts

/*
 * heat transfer simulation using Jacobi iteration
 *
 * memory is allocated for two complete lattices,
 * to keep new values separate from current values
 *
 * example usage:  a.out 500 800
 * will run with a 500x800 lattice size.
 * default is 2000x2000.
 */
int main(int argc, char **argv)
{
    int i, j;
    int n, m;
    double **A, **Anew;
    double tol = 0.001; // tolerance
    double err = 2 * tol;
    int iter = 0;
    int iter_max = 100000; // 25000;
    omp_set_num_threads(12);

    /* size of the area nxm; default 2000x2000 */
    if (argc < 3)
    {
        n = m = 1000;
    }
    else
    {
        n = atoi(argv[1]);
        m = atoi(argv[2]);
    }

    // allocate memory for the nxm lattice; to hold the current values
    A = (double **)calloc(n, sizeof(double *));
    for (j = 0; j < n; j++)
    {
        A[j] = (double *)calloc(m, sizeof(double));
    }
    // allocate memory for the 2nd nxm lattice; to hold the new values
    Anew = (double **)calloc(n, sizeof(double *));
    for (j = 0; j < n; j++)
    {
        Anew[j] = (double *)calloc(m, sizeof(double));
    }

    /*
     * initialize it to something room-temperature (Centigrade)
     */
    for (j = 0; j < n - 1; j++)
    {
        for (i = 0; i < m - 1; i++)
        {
            A[j][i] = 20;
        }
    }

    setBoundary(A, n, m);
    coolingPosts(A, n, m);
    printMap(A, n, m);

    /*
     * jacobi iteration - until we're within tolerance
     */
    double neighborAverage;
    double neighborSum;
    int id;
    double idmax;
    // double start = omp_get_wtime();
#pragma omp parallel private(neighborAverage, neighborSum, id, idmax) shared(err)
    {
        int i, j;
        int numthreads = omp_get_num_threads();
        id = omp_get_thread_num();
        while (/*err > tol &&*/ iter < iter_max)
        {
            err = 0.0;
            neighborAverage = 0.0;
            neighborSum = 0.0;

            for (j = id + 1; j < n - 1; j += numthreads)
            {
                for (i = 1; i < m - 1; i++)
                {
                    if (A[j + 1][i] != 0)
                        neighborSum += A[j + 1][i];
                    if (A[j - 1][i] != 0)
                        neighborSum += A[j - 1][i];
                    if (A[j][i + 1] != 0)
                        neighborSum += A[j][i + 1];
                    if (A[j][i - 1] != 0)
                        neighborSum += A[j][i - 1];
                    neighborAverage = neighborSum * 0.25;
                    neighborSum = 0.0;
                    Anew[j][i] = neighborAverage;
                    neighborAverage = 0.0;
                    // fprintf(stderr, "%f \t", Anew[i][j]);
                }

                // Hint: when parallel, not thread-safe!
                idmax = max(idmax, fabs(Anew[j][i] - A[j][i]));

            } // next i    // next j
#pragma omp critical
            {
                err = max(err, idmax);
            }
            // fprintf(stderr, "err is %f", err);
            // fprintf(stderr, " iter is %d", iter);
            /* copy new map to old */

#pragma omp single
            {
                setBoundary(Anew, n, m);
                coolingPosts(Anew, n, m);
                iter++;
            }

            for (j = 1; j < n - 1; j++)
            {
                for (i = 1; i < m - 1; i++)
                {
                    A[j][i] = Anew[j][i];
                }
            }

            /* re-enforce boundary */
            // DEBUG or for periodic visualization:
#pragma omp single
            {
                if (iter % 100 == 0)
                {
                    printf("%d\n", iter);
                }
                if (iter % 10000 == 0)
                {
                    printMap(Anew, n, m);
                }
            }
        } // endwhile
    }

/*
 * Write this to stderr to avoid mixing in with the values;
 * it can serve as a "checksum" to see if a new version still
 * terminates at the same iteration count, implying the same
 * values were computed.
 */
#pragma omp single
    {
        if (iter == iter_max)
        {
            setBoundary(Anew, n, m);
            printMap(Anew, n, m);
        }
        // double end = omp_get_wtime();
        // if (iter % 100 == 0)
        {
            fprintf(stderr, "%d iterations.\n", iter);
        }
    }
    /*
     * display final results
     */
} // main
