#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 500000
#define P 31

struct Node{
    char key[100];
    char inf[50];
    int marcador; //Nunca ocupada = 0 || Ocupada = 1 || Removida = -1
};
typedef struct Node Nodo;

long long primaria( char *word );
long long secundaria( char* word );
void insert( Nodo* hash, char* word, char* inf );
void criaHash( Nodo* hash );
void search( Nodo* hash, char *word );
long long colisao( long long k1, char* word, Nodo* hash );
int menu();

int main() {
    Nodo* hash = (Nodo*)malloc( sizeof(Nodo) * TAM_HASH );
    FILE* arquivo;
    char linha[100], word[100], inf[100];

    criaHash( hash );

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

    for ( ; ; ) {
        int m = menu();
        switch ( m ) {
            case 1: {
                printf("Insira a palavra e as informacoes entre virgulas: ");
                scanf("%[^,],%[^\n]", word, inf);
                insert( hash, word, inf );
                break;
            }

            case 2: {
                printf("Insira a palavra e as informacoes entre virgulas: ");
                scanf("%[^,],%[^\n]", word, inf);
                search( hash, word );
                break;
            }

            case 3: {
                
                break;
            }
        
            case 4: {
                free( hash );
                exit(0);
                break;
            }
        }
    }

    return 0;
}

void criaHash( Nodo* hash ){
    Nodo* puppet = hash;
    for( double i = 0; i < TAM_HASH; i++ ) {
        puppet->marcador = 0;
        puppet++;
    }
}

long long primaria( char* word ){
    //somatorio até n - 1 das (letras segundo a tabela ascii * P elevado a i) mod(%) TAM_HASH
    long long num, pot = 1, k = 0;

    while ( *word ) {
        num = *word;

        k = (k + ((num * pot) % TAM_HASH )) % TAM_HASH;
        pot *= P;

        word++;
    }

    return k;
}

long long secundaria( char* word ) {
    long long k = 0, num;

    while ( *word ) {
        num = *word;

        k = (k + num) % TAM_HASH;

        word++;
    }

    return k;
}

long long colisao( long long k1, char* word, Nodo* hash ) {
    long long t = 1;
    Nodo* puppet = (Nodo *)(hash) + k1;
    long long indice, k2 = secundaria(word);

    printf( "Colisoes:\n" );
    while ( puppet->marcador == 1 && t < TAM_HASH ) {
        printf("%I64d\n", t);//tava dando "erro" se fosse %lld
        indice = ( k1 + ( ( t * k2 ) % TAM_HASH ) ) % TAM_HASH;
        puppet = (Nodo *)(hash) + indice;
        t++;
    }
    /*if ( t == TAM_HASH ) {
        
    }*/

    return indice;
}

void insert( Nodo* hash, char* word, char* inf ) {
    long long key = primaria( word );
    Nodo* puppet = (Nodo *)(hash) + key;

    if ( puppet->marcador != 1 ) { 
        puppet->marcador = 1;
        //printf( "Marcador = %d\n", puppet->marcador );
        strcpy( puppet->key, word );
        //printf( "Word = %s\n", puppet->key );
        strcpy( puppet->inf, inf );
        //printf( "Inf = %s", puppet->inf );
        return;
    } else {
        key = colisao( key, word, hash );
        puppet = (Nodo *)(hash) + key;

        puppet->marcador = 1;
        //printf( "Marcador = %d\n", puppet->marcador );
        strcpy( puppet->key, word );
        //printf( "Word = %s\n", puppet->key );
        strcpy( puppet->inf, inf );
        //printf( "Inf = %s", puppet->inf );
        return;
    }

}

void search( Nodo* hash, char *word ){
    long long key = primaria( word );
    Nodo* puppet = hash + key;
    
    if ( puppet->marcador == 1 ) { 
        printf( "Word = %s\n", puppet->key );
        printf( "Inf = %s", puppet->inf );
        return;
    } else {
        key = colisao( key, word, hash );
        puppet = hash + key;
        printf( "Word = %s\n", puppet->key );
        printf( "Inf = %s", puppet->inf );
        return;
    }

    printf( "Nao ta na hash" );
}

void remover( Nodo* hash, char* word ) {
    long long key = primaria( word );
}

Nodo* realocHash( Nodo* hash ) {
    
}

int menu() {
    int m;
    do {
        printf( "\t### Menu ###\n" );
        printf( "\t1.Inserir\n" );
        printf( "\t2.Buscar\n" );
        printf( "\t3.Remover\n" );
        printf( "\t4.Sair\n" );
        printf( "Escolha: " );
        scanf( "%d", &m );
        getchar();
    } while( m <= 0 || m > 4 );
    return m;
}

/*
    scanf("%[^,],%[^\n]", word, inf);
    insert( hash, word, inf );
    printf( "Inserindo: %s, %s\n", word, inf );

    search( hash, word );

    printf("\n");
    getchar();
    scanf("%[^,],%[^\n]", word, inf);
    insert( hash, word, inf );
    printf( "Inserindo: %s, %s\n", word, inf );

    search( hash, word );
    */ 