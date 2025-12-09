#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
    int id;
    double x, y;
}Cidades;

void inserirVetor( Cidades** vetor, int id, double x, double y );
void lerVetor( Cidades* v, int tam );
double distanciaE( double x1, double y1, double x2, double y2 );
void inserirMatrizLinha( Cidades* v, double** matriz, int n );
void lerMatriz( double** m, int tam );

int main() {
    FILE* arquivo;
    Cidades* vetor = (Cidades *)malloc( sizeof(Cidades) );
    char linha[100], node[] = "NODE_COORD_SECTION";
    int id, n = 0;  //tamanho
    double x, y;

    arquivo = fopen( "dj38.tsp", "r" );
    if ( !arquivo ) {
        printf("Erro");
        return 1;
    }

    while( fscanf( arquivo, "%s", linha ) == 1 ) {
        if ( strcmp( linha, node ) == 0 ) {
            break;
        } 
    }

    while ( fscanf( arquivo, "%d %lf %lf", &id, &x, &y ) == 3 ) {
        inserirVetor( &vetor, id, x, y );
        n++;
    }

    //lerVetor( vetor, n ); //testar se ta inserindo certo do arquivo

    double** matriz = (double **)malloc( sizeof(double *) * n );
    for( int i = 0; i < n; i++ ) {
        matriz[i] = (double *)malloc( sizeof(double) * n );//se o conteudo for -1 ent ta removido
    }

    inserirMatrizLinha( vetor, matriz, n );
    //lerMatriz( matriz, n ); //testar se ta funcionando

    for ( ; ; ) {
        int m = menu();
        switch ( m ) {
            case 1: {
                
                break;
            }

            case 2: {
                
                break;
            }

            case 3: {
                
                break;
            }
        
            case 4: {
                break;
            }
            case 5: {
                exit(0);
                break;
            }
        }
    }

    for( int i = 0; i < n; i++ ) {
        free( matriz[i] );
    }
    free(matriz);
    free(vetor);
    fclose(arquivo);
    return 0;
}

void inserirVetor( Cidades** v, int id, double x, double y ) {
    *v = realloc( *v, ((long long unsigned)id) * sizeof(Cidades) );
    Cidades* puppet = (Cidades*)(*v) +( id - 1 );

    puppet->id = id;
    puppet->x = x;
    puppet->y = y;
}

void lerVetor( Cidades* v, int tam ) {
    Cidades* puppet = v;

    for( int i = 0; i < tam; i++ ){
        printf( "ID: %d    X: %lf    Y: %lf\n", puppet->id, puppet->x, puppet->y );
        puppet++;
    }
}

double distanciaE( double x1, double y1, double x2, double y2 ) {
    //distancia entre as cidades
    return sqrt( pow( x2 - x1, 2 ) + pow( y2 - y1, 2 ) );
}

void inserirMatrizLinha( Cidades* v, double** matriz, int n ) {//( vetor, matriz, tam do vetor/matriz ) 
    int m = 1;

    for ( int i = 0; i < n; i++ ) {
        Cidades* puppet1 = v + i;

        for ( int j = 0; j < m; j++ ) {
            Cidades* puppet2 = v + j;
            if ( i == j ) {
                matriz[i][j] = 0;
            } else {
                matriz[i][j] = matriz[j][i] = distanciaE( puppet1->x, puppet1->y, puppet2->x, puppet2->y );
            }
        }

        if ( m < n ) {
            m++;
        }
    }
}

void lerMatriz( double** m, int tam ) {

    for( int i = 0; i < tam; i++ ){
        for( int j = 0; j < tam; j++ ) {
        printf( "[%d][%d]: %lf\n", i, j, m[i][j] );
        }
        printf("\n");
    }
}

int menuP() {
    int m;
    do {
        printf( "\t### Menu ###\n" );
        printf( "\t1.Adicionar Cidade\n" );
        printf( "\t2.Remover Cidade\n" );
        printf( "\t3.Busca de Arestas\n" );
        printf( "\t4.Relatorio\n" );
        printf( "\t5.Sair\n" );
        printf( "Escolha: " );
        scanf( "%d", &m );
        getchar();
    } while( m <= 0 || m > 4 );
    return m;
}

int buscarVertice() {//0 = nao existe | 1 = existe

}

int buscarAresta() {//0 = nao existe | 1 = existe

}