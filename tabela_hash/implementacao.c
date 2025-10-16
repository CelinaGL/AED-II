#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define TAM_HASH 5000000
#define P 31

struct Node{
    char key[100];
    char inf[50];
    int marcador; //Nunca ocupada = 0 || Ocupada = 1 || Removida = -1
};
typedef struct Node Nodo;

long long primaria( char *word );
void insert( Nodo* hash, long long key, char* word, char* inf );
Nodo* criaHash();
bool search( Nodo* hash, char *word );

int main() {
    Nodo* hash = (Nodo*)malloc( sizeof(Nodo) * TAM_HASH );
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
    criaHash( hash );
    scanf("%[^,],%[^\n]", word, inf);
    key = primaria( word );
    insert( hash, key, word, inf );
    printf( "Inserindo: %s, %s\n", word, inf );

    if ( search( hash, word ) ) {
        printf("Ta na hash");
    } else {
        printf("Nao ta na hash");
    }

    free(hash);
    return 0;
}

Nodo* criaHash( Nodo* hash ){
    Nodo* puppet = hash;
    for( double i = 0; i < TAM_HASH; i++ ) {
        puppet->marcador = 0;
        puppet++;
    }
}

long long primaria( char *word ){
    //somatorio até n - 1 das (letras segundo a tabela ascii * P elevado a i) mod(%) TAM_HASH
    long long num, pot = 1, k = 0;
    
    while ( *word ) {
       num = *word;

        k = (k + (num * pot)) % TAM_HASH;  
        pot *= P;        // atualiza a potência de P

        word++;
    }

    return k;
}

void insert( Nodo* hash, long long key, char* word, char* inf ) {
    Nodo* puppet = (Nodo *)(hash) + key;
    if ( puppet->marcador != 1 ) { 
    puppet->marcador = 1;
    strcmp( puppet->key, word );
    strcmp( puppet->inf, inf );
}
}

bool search( Nodo* hash, char *word ){
    long long key = primaria( word );
    Nodo* puppet = (Nodo *)(hash) + key;
    
    if ( puppet->marcador == 1 ) {
        return true;
    }
    return false;
}