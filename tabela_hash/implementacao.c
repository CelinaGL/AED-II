//Não consegui terminar a função de reHash a tempo, ela esta comentada no final do código
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 100003
#define P 31

struct Node{
    unsigned char key[100];
    unsigned char inf[50];
    int marcador; //Nunca ocupada = 0 || Ocupada = 1 || Removida = -1
};
typedef struct Node Nodo;

long long primaria( unsigned char *word );
long long secundaria( unsigned char* word );
void insert( Nodo* hash, unsigned char* word, unsigned char* inf, long long* coli );
void criaHash( Nodo* hash );
void search( Nodo* hash, unsigned char *word );
long long colisao( long long k1, unsigned char* word, Nodo* hash, long long* coli );
void remover( Nodo* hash, unsigned char *word );
//Nodo* reHash( Nodo* hash, long long novoT );
int menu();

int main() {
    Nodo* hash = (Nodo*)malloc( sizeof(Nodo) * TAM_HASH );
    FILE* arquivo;
    unsigned char linha[200], word[200], inf[200];
    long long coli = 0;

    criaHash( hash );
    if (hash == NULL) {
        printf("Erro ao alocar memoria\n");
        return 1;
    }

    arquivo = fopen("lexico_v3.0.txt", "r");
    while ( fgets( (char *)linha, sizeof(linha), arquivo ) ) {
        size_t t = strlen((char *)linha);
        linha[t - 1] = '\0';

        sscanf((char *)linha, "%99[^,],%s", word, inf);

        //printf( "Inserindo: %s, %s\n", word, inf );
        insert( hash, word, inf, &coli );
        
    }
    fclose(arquivo);

    printf("Colisoes: %lld\n", coli);

    for ( ; ; ) {
        int m = menu();
        switch ( m ) {
            case 1: {
                printf("Digite a palavra e as informacoes entre virgulas: ");
                scanf("%[^,],%[^\n]", word, inf);
                insert( hash, word, inf, &coli );
                break;
            }

            case 2: {
                printf("Digite a palavra: ");
                scanf("%[^\n]", word);
                search( hash, word );
                break;
            }

            case 3: {
                printf("Digite a palavra: ");
                scanf("%[^\n]", word);
                remover( hash, word );
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
    for( long long i = 0; i < TAM_HASH; i++ ) {
        puppet->marcador = 0;
        puppet++;
    }
}

long long primaria( unsigned char* word ){
    //somatorio até n - 1 das (letras segundo a tabela ascii * P elevado a i) mod(%) TAM_HASH
    long long num, pot = 1, k = 0;

    while ( *word ) {
        num = *word;

        k = (k + ((num * pot) % TAM_HASH )) % TAM_HASH;
        pot = (pot * P) % TAM_HASH;

        word++;
    }

    return k;
}

long long secundaria( unsigned char* word ) {
    long long k = 0, num;

    while ( *word ) {
        num = *word;

        k = (k + num) % TAM_HASH;

        word++;
    }

    return k;
}

long long colisao( long long k1, unsigned char* word, Nodo* hash, long long* coli ) {
    long long t = 1;
    Nodo* puppet = (Nodo *)(hash) + k1;
    long long indice = k1, k2 = secundaria(word);
    if (k2 == 0) {
        k2 = 1;
    }

    while ( puppet->marcador == 1 && t < TAM_HASH  ) {
        indice = ( k1 + ( ( t * k2 ) % TAM_HASH ) ) % TAM_HASH;
        puppet = (Nodo *)(hash) + indice;
        t++;
        (*coli)++;
    }

    /*if ( t == TAM_HASH ) {
        rehash++;
        hash = reHash(hash, TAM_HASH * rehash );
    }*/

    return indice;
}

void insert( Nodo* hash, unsigned char* word, unsigned char* inf, long long* coli ) {
    long long key = primaria( word );
    Nodo* puppet = (Nodo *)(hash) + key;

    if ( puppet->marcador != 1 ) { 
        puppet->marcador = 1;
        strcpy( (char *)puppet->key, (char *)word );
        strcpy( (char *)puppet->inf, (char *)inf );
        return;
    } else {
        key = colisao( key, word, hash, coli );
        puppet = (Nodo *)(hash) + key;

        puppet->marcador = 1;
        strcpy( (char *)puppet->key, (char *)word );
        strcpy( (char *)puppet->inf, (char *)inf );
        return;
    }

}

void search( Nodo* hash, unsigned char *word ){
    long long k1 = primaria(word), k2 = secundaria(word);
    long long i = 0, indice = 0;
    if (k2 == 0) {
        k2 = 1;
    }

    while (i < TAM_HASH) {
        indice = ( k1 + (i * k2)% TAM_HASH ) % TAM_HASH;
        Nodo* puppet = hash + indice;

        if ( puppet->marcador == 0 ) {
            printf("Nao ta na hash\n");
            return;
        }

        if ( puppet->marcador == 1 && strcmp((char *)puppet->key, (char *)word) == 0 ) {
            printf("Word = %s\n", puppet->key);
            printf("Inf = %s\n", puppet->inf);
            return;
        }

        i++;
    }

    printf("Nao ta na hash\n");
}

void remover( Nodo* hash, unsigned char *word ){
    long long k1 = primaria(word), k2 = secundaria(word);
    long long i = 0, indice;
    if (k2 == 0) {
        k2 = 1;
    }

    while (i < TAM_HASH) {
        indice = (k1 + i * k2) % TAM_HASH;
        Nodo* puppet = hash + indice;

        if ( puppet->marcador == 0 ) {
            printf("Nao ta na hash\n");
            return;
        }

        if ( puppet->marcador == 1 && strcmp((char *)puppet->key, (char *)word) == 0 ) {
            puppet->marcador = -1;
            printf("Palavra removida\n");

            return;
        }

        i++;
    }

    printf("Nao ta na hash\n");
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
Nodo* reHash( Nodo* hash, long long novoT ) {
    Nodo* novaHash = (Nodo*)malloc( sizeof(Nodo) * (size_t)novoT );
    long long coli = 0, t = 0;
    if (!novaHash) {
        printf("Erro de alocacao de memoria\n");
        return hash; 
    }
    criaHash( novaHash );

    Nodo* puppet = hash;
    while ( t  < (novoT - TAM_HASH) ) {
        insert(novaHash, puppet->key, puppet->inf, &coli);
        t++;
        puppet++;
    }
    printf("Colisoes: %lld\n", coli);
    free(hash);
        
    return novaHash;
}
*/    