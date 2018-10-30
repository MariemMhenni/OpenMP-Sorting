#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#define  K  10//taille du tableau (nb colonne)
#define  N  10//nombre de tableau (nb ligne)

void generator(int **tab);

int main() //int argc, char *argv[]
{
	//(void) argc; (void) argv;
    int **tab=NULL, i,j;
    
    tab=(int**)malloc(sizeof(int*)*N);
    #pragma omp parallel for
		for(i=0;i<N;i++)
		{
			tab[i]=(int*)malloc(sizeof(int)*K);
		}
    
    generator(tab);

    printf("%s\n", "Hello World!");
    return 0;
}

void generator(int **tab)
{
	int i,j;
	srand(time(NULL));
	#pragma omp paralle for
		for(i=0;i<N;i++)
		{
			#pragma omp parallel for
				for(j=0;j<K;j++)
				{
					tab[i][j]=rand();
					printf("tab %d/%d : %d\n",i,j,tab[i][j]);

				}
		}
}
