#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>

/** Number of tables (a.k.a. blocks). */
static int N;
/** Size of one table (one block). */
static int K;

/** Return the minimum value between a and b. */
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
/** Return the maximum value between a and b. */
#define MAX(a,b) (((a) > (b)) ? (a) : (b))

/**
 * Fill a table with random values.
 * @param tab the table to fill.
 * @param size the size of the table.
 */
void generator(double tab[], int size)
{
    for (int i = 0; i < size; i++)
        tab[i] = rand() / 3.33; /* Generate real numbers. */
}

/**
 * Perform a bubble sort on a table.
 * @param tab the table to sort.
 * @param size the size of the table.
 */
void sort(double *tab, int size)
{
    for (int j = 1, sort = 0; !sort ; j++)
    {
        sort = 1;
        for (int i = 0; i < size - j; i++)
        {
            if (tab[i+1] < tab[i])
            {
                double tmp = tab[i];
                tab[i]     = tab[i+1];
                tab[i+1]   = tmp;
                sort       = 0;
            }
        }
    }
}

/**
 * Merge two locally sorted table in two globally sorted table.
 * @param tab1 a sorted table.
 * @param tab2 a sorted table.
 * @param size the size of the two tables.
 */
void sort_merge(double tab1[], double tab2[], int size)
{
    /* Copy the original two tables. */
    double tmp1[size], tmp2[size], *tab = tab1;
    memcpy(tmp1, tab1, size * sizeof(*tab1));
    memcpy(tmp2, tab2, size * sizeof(*tab2));
    /* Merge the two tables. The next for loop will do only 2 iterations. */
    for (int cnt1 = 0, cnt2 = 0; cnt1 + cnt2 != 2 * size; tab = tab2) {
        for (int i = 0; i < size; i++)
            tab[i] = (tmp1[cnt1] < tmp2[cnt2] && cnt1 < size) || cnt2 >= size ?
                tmp1[cnt1++] : tmp2[cnt2++];
    }
}

/**
 * Sort a bunch of tables in parallel.
 * @param N Number of blocks.
 * @param K Size of one block.
 * @param blocks Tables to sort.
 */
void sort_parallel(int N, int K, double ** blocks)
{
    /* Sort each block individually, in parallel. */
    #pragma omp parallel for
    for (int i = 0; i < N; i++)
        sort(blocks[i],K);

    /* Merge all sorted blocks to have a sorted database, in parallel. */
    for (int j = 0; j < N; j++)
    {
        int k = (j % 2); /* Process blocks two by two. */
        #pragma omp parallel for
        for (int i = 0; i < N/2; i++)
        {
            int b1 = (k + 2*i) % N, b2 = (k + 2*i + 1) % N;
            sort_merge(blocks[MIN(b1, b2)], blocks[MAX(b1, b2)], K);
        }
    }
}

int main(int argc, char *argv[])
{
   
   /* Initialize parameters of the algorithm. */
    if (argc <= 2)
        return fprintf(stderr, "Usage: %s [NUMBER OF BLOCKS] [SIZE OF ONE BLOCK]\n", argv[0]), 1;
    if ((N = atoi(argv[1])) < 1 || (K = atoi(argv[2])) < 1)
        return fprintf(stderr, "Usage: %s [NUMBER OF BLOCKS > 0] [SIZE OF ONE BLOCK > 0]\n", argv[0]), 1;
    srand(time(NULL));
    double t = omp_get_wtime();

    /* Database to sort: N tables of K random elements each. */
    double ** blocks = malloc(N * sizeof(*blocks));
    for (int i = 0; i < N; i++)
        generator((blocks[i] = malloc(K * sizeof(**blocks))), K);
    
    /* Sort the database in parallel. */
    sort_parallel(N, K, blocks);
   
    /* Display the database. */
    printf("Time elapsed: \n%f\nDatabase:\n", omp_get_wtime() - t);
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < K; j++)
            printf("%f\n", blocks[i][j]);
    }
    
    /* Free ressources. */
    for (int i = 0; i < N; i++)
        free(blocks[i]);
    free(blocks);

    return 0;
}
