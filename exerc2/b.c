#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#define N 2048
#define NUM_GER 2000
#define NUM_THREADS 8

int getNeighbors(int **grid, int i, int j){

    int cima, baixo, esquerda, direita;

    cima = i - 1;
    baixo = i + 1;
    esquerda = j - 1;
    direita = j + 1;

    if(cima < 0) 
        cima = N - 1;

    if(baixo >= N) 
        baixo = 0;

    if(esquerda < 0) 
        esquerda = N - 1;

    if(direita >= N) 
        direita = 0;
    
    return  grid[cima][esquerda] + grid[cima][j] 
            + grid[cima][direita] + grid[i][esquerda]
            + grid[i][direita] + grid[baixo][esquerda]
            + grid[baixo][j] + grid[baixo][direita];     
}



int main(){

    double tempo_total, tempo_laco;

    tempo_total = omp_get_wtime(); // pega tempo atual

    int **grid = (int **) calloc(N, sizeof(int*));
    int **newgrid = (int **) calloc(N, sizeof(int*));


    for(int i = 0 ; i < N ; i++){
        grid[i] = (int *) calloc(N, sizeof(int));
        newgrid[i] = (int *) calloc(N, sizeof(int));
    }

    // Building Glider
    int lin = 1, col = 1;
    grid[lin  ][col+1] = 1;
    grid[lin+1][col+2] = 1;
    grid[lin+2][col  ] = 1;
    grid[lin+2][col+1] = 1;
    grid[lin+2][col+2] = 1;


    // Building R-pentomino
    lin =10; col = 30;
    grid[lin][col+1] = 1;
    grid[lin][col+2] = 1;
    grid[lin+1][col  ] = 1;
    grid[lin+1][col+1] = 1;
    grid[lin+2][col+1] = 1;


    int vizinhos_vivos = 0;
    int i,j;

    tempo_laco = omp_get_wtime();
    for(int g = 0; g < NUM_GER; g++){

        omp_set_num_threads(NUM_THREADS);
        #pragma omp parallel shared(grid, newgrid)
        {
            
            #pragma omp for collapse(2) private(i, j, vizinhos_vivos) 
            for(i = 0; i < N; i++){
                for(j = 0; j < N; j++){

                    vizinhos_vivos = getNeighbors(grid, i, j);


                    if(grid[i][j] == 1 && (vizinhos_vivos == 2 || vizinhos_vivos == 3))
                        newgrid[i][j] = 1;
                        
                    else if(grid[i][j] == 0 && vizinhos_vivos == 3)
                        newgrid[i][j] = 1;
                    
                    else
                        newgrid[i][j] = grid[i][j] * 0;

                }
            }

            #pragma omp for collapse(2)
            for(i = 0; i < N; i++)
                for(j = 0; j < N; j++)
                    grid[i][j] = newgrid[i][j];

        }
    }
    tempo_laco = omp_get_wtime() - tempo_laco;

    int total = 0;

    // calcula vivos
    #pragma omp parallel for collapse(2) reduction(+ : total)
        for(i = 0; i < N; i++){
            for (j = 0; j < N; j++)
                {
                if(grid[i][j]) total++;
                }
        }
    
    
    tempo_total = omp_get_wtime() - tempo_total;

    printf("%d células vivas\n", total);
    printf("Tempo do somatório: %f\n", tempo_total - tempo_laco);

    
}