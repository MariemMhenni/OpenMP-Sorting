#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

/* Parameters. */

/** Number of tables (a.k.a. blocks). */
static int N;
/** Size of one table (one block). */
static int K;

int main(int argc, char *argv[])
{
    /* Initialize parameters of the algorithm. */
    if (argc <= 2)
        return fprintf(stderr, "Usage: %s [NUMBER OF BLOCKS] [SIZE OF ONE BLOCK]\n", argv[0]), 1;
    if ((N = atoi(argv[1])) < 1 || (K = atoi(argv[2])) < 1)
        return fprintf(stderr, "Usage: %s [NUMBER OF BLOCKS > 0] [SIZE OF ONE BLOCK > 0]\n", argv[0]), 1;

    /* Database to sort: N tables of K elements each. */
    int blocks[N][K];
    
    return 0;
}
