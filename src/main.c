#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

/* Parameters. */

/** Number of tables (a.k.a. blocks). */
static int N;
/** Size of one table (one block). */
static int K;


void tri(double *tab, int size);
void generator(double tab[], int size);


int main(int argc, char *argv[])
{
    /* Initialize parameters of the algorithm. */
    if (argc <= 2)
        return fprintf(stderr, "Usage: %s [NUMBER OF BLOCKS] [SIZE OF ONE BLOCK]\n", argv[0]), 1;
    if ((N = atoi(argv[1])) < 1 || (K = atoi(argv[2])) < 1)
        return fprintf(stderr, "Usage: %s [NUMBER OF BLOCKS > 0] [SIZE OF ONE BLOCK > 0]\n", argv[0]), 1;
    srand(time(NULL));

    /* Database to sort: N tables of K random elements each. */
    double blocks[N][K];
    for (int i = 0; i < N; i++)
        generator(blocks[i], K);
	
    tri(blocks[0],K);
	
   
	
    return 0;
}

/**
 * Fill a table with random values.
 * @param tab the table to fill.
 * @param size the size of the table.
 */
void generator(double tab[], int size)
{
    for (int i = 0; i < size; i++)
        tab[i] = rand()/3.33; /* Generate real numbers. */
}
/**
 * Bubble sort
 */

void tri(double *tab, int size)
{
    int i,j=0, test=1, tmp;
    
    while(test)
    {
        test=0;
        for(i=0;i<size-j;i++)
        {
            if(tab[i+1]<tab[i])
            {
                tmp=tab[i];
                tab[i]=tab[i+1];
                tab[i+1]=tmp;
                test=1;
            }
         }
     }
}


