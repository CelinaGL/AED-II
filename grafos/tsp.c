#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
    int id;
    long long x, y;
}Cidades;

int main() {
    FILE* arquivo;
    Cidades* vetor = (Cidades *)malloc( sizeof(Cidades) );
    char linha[100], node[] = "NODE_COORD_SECTION";
    int id;
    long long x, y;

    arquivo = fopem( "dj38.tsp", "r" );

    while( fscanf( "%[^\n]s", linha ) ) {
        if ( strcmp( linha, node ) == 0 ) {
            while ( fscanf( "%d %lld %lld", &id, &x, &y ) == 3 ) {
                inserirVetor( vetor, id, x, y );
            }
            break;
        } 
    }
    fclose(arquivo);
    return 0;
}

void inserirVetor( Cidades* vetor, int id, long long x, long long y ) {
    Cidades* puppet = (Cidades*)(vetor) + id;

}

/*
Distancia entre as cidades
*/