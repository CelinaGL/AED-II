#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define TAM_HASH 60000
#define P 31
#define ASCII 256

struct Node{
    char key[100];
    char inf[50];
    int marcador; //Nunca ocupada = 1 || Ocupada = 0 || Removida = -1
};
typedef struct Node Nodo;

long long primaria( char *word );
void insert( Nodo* hash, long long key, char* word, char* inf );

int main() {
    Nodo hash[TAM_HASH];
    FILE* arquivo;
    char linha[100], word[100], inf[100];
    long long key;

    /*arquivo = fopen("lexico_v3.0.txt", "r");
    while ( fgets( linha, sizeof(linha), arquivo ) ) {
        int t = strlen(linha);
        linha[t - 1] = '\0';

        sscanf(linha, "%99[^,],%s", word, inf);
        key = primaria( word );

        printf( "Inserindo: %s, %s\n", word, inf );
        insert( hash, key, word, inf );
        
    }
    fclose(arquivo);*/

    scanf("%[^,]s,%[^\n]s", word, inf);
    key = primaria( word );
    insert( hash, key, word, inf );

    

    return 0;
}

void criaHash( Nodo hash[TAM_HASH] ){

    for( int i = 0; i < TAM_HASH; i++ ){
        hash[i].marcador = 1;
    }

}

long long primaria( char *word ){
    //somatorio até n - 1 das (letras segundo a tabela ascii * P elevado a i) mod(%) TAM_HASH
    long long k;
    int num, i = 0;
    
    while ( *word ) {
        num = *word;
        k = num * pow(P, i);
        word++;
        i++;
    }

    k = k % TAM_HASH;
    return k;
}

void insert( Nodo* hash, long long key, char* word, char* inf ) {
    hash[key].marcador = 0;
    strcpy( hash[key].key, word );
    strcpy( hash[key].inf, inf );
}

bool search( Nodo* hash ){

}