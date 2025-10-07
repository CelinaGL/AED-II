#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define AF 8
#define TAMAP 100
#define QUANP 10000

struct Node{
    int numero;
    bool end; 
    char word[TAMAP][QUANP];
    struct Node *next[AF];
};

typedef struct Node Nodo;

int menu();
Nodo* createTree(); 
void freeTrie( Nodo* node );
void insert( Nodo* root, char* word );
Nodo* search( Nodo* root, char* word );
char digitosEspecias( char l );
int cDigito( char l );


int main() {
    FILE* arquivo;
    Nodo* root = createTree();
    int m, f;
    char word[TAMAP];

    arquivo = fopen("dicionario.txt", "r");
    while ( fgets( word, sizeof(word), arquivo ) ) {
        int t = strlen(word);
        word[t - 1] = '\0';

        printf( "Inserindo: %s\n", word ); //Usado pra testar se estava inserindo ou nao
        insert( root, word );
    } 

    fclose(arquivo);

    for ( ; ; ) {
        m = menu();
        switch ( m ) {
            case 1: {
                printf( "Digite a palavra: " );
                fgets( word, TAMAP, stdin );
                int t = strlen(word);
                word[t - 1] = '\0';

                Nodo* r = search( root, word );

                if ( r && r->end ){
                    printf( "Palavra: '%s'\n", word );
                } else {
                   printf( "Palavra nao encontrada\n" );
                }
                break;
            }

            case 2: {
                printf( "Digite a palavra: " );
                scanf( "%s", word );
                Nodo* ra = search( root, word );

                if (ra && ra->end) {
                    printf("Nova polaridade: ");
                    scanf("%d", &f);
                    printf( "Polaridade de '%s' atualizada para \n", word );
                } else {
                    printf( "Palavra nao encontrada\n" );
                }
                break;
            }

            case 3: {
                freeTrie( root );
                exit(0);
            }
        
            case 4: {
                freeTrie( root );
                exit(0);
                break;
            }
        }
    }

    return 0;
}

int menu() {
    int m;
    do {
        printf( "\t### Menu ###\n" );
        printf( "\t1.Busca de polaridade\n" );
        printf( "\t2.Edicao de polaridade\n" );
        printf( "\t3.Salvamento de arquivo\n" );
        printf( "\t4.Sair\n" );
        printf( "Escolha: " );
        scanf( "%d", &m );
        getchar();
    } while( m <= 0 || m > 4 );
    return m;
}

Nodo* createTree() {
    Nodo *node = (Nodo *)malloc( sizeof(Nodo) );
    node->numero = 1;
    node->end = false;
    for ( int i = 0; i < AF; i++ ) {
        node->next[i] = NULL;
    }
    return node;
}

void freeTrie( Nodo* node ) {
    if (!node) { 
        return;
    }
    for (int i = 0; i < AF; i++) {
        freeTrie(node->next[i]);
    }
    free(node);
}

char digitosEspecias( char l ) {
    switch (l) {

    case 0x81: return 'a'; // Á
    case 0xA1: return 'a'; // á
    case 0x80: return 'a'; // À
    case 0xA0: return 'a'; // à
    case 0x82: return 'a'; // Â
    case 0xA2: return 'a'; // â
    case 0x83: return 'a'; // Ã
    case 0xA3: return 'a'; // ã
    case 0x84: return 'a'; // Ä
    case 0xA4: return 'a'; // ä

    case 0x89: return 'e'; // É
    case 0xA9: return 'e'; // é
    case 0x88: return 'e'; // È
    case 0xA8: return 'e'; // è
    case 0x8A: return 'e'; // Ê
    case 0xAA: return 'e'; // ê

    case 0x8D: return 'i'; // Í
    case 0xAD: return 'i'; // í
    case 0x8C: return 'i'; // Ì
    case 0xAC: return 'i'; // ì
    case 0x8E: return 'i'; // Î
    case 0xAE: return 'i'; // î

    case 0x93: return 'o'; // Ó
    case 0xB3: return 'o'; // ó
    case 0x92: return 'o'; // Ò
    case 0xB2: return 'o'; // ò
    case 0x94: return 'o'; // Ô
    case 0xB4: return 'o'; // ô
    case 0x95: return 'o'; // Õ
    case 0xB5: return 'o'; // õ
    case 0x96: return 'o'; // Ö
    case 0xB6: return 'o'; // ö

    case 0x9A: return 'u'; // Ú
    case 0xBA: return 'u'; // ú
    case 0x99: return 'u'; // Ù
    case 0xB9: return 'u'; // ù
    case 0x9B: return 'u'; // Û
    case 0xBB: return 'u'; // û
    case 0x9C: return 'u'; // Ü
    case 0xBC: return 'u'; // ü

    case 0xA7: return 'c'; // ç
    case 0x87: return 'c'; // Ç

    case 0xB1: return 'n'; // ñ
    case 0x91: return 'n'; // Ñ

    case 0x41: return 'a'; // A
    case 0x42: return 'b'; // B
    case 0x43: return 'c'; // C
    case 0x44: return 'd'; // D
    case 0x45: return 'e'; // E
    case 0x46: return 'f'; // F
    case 0x47: return 'g'; // G

    }

    return l;
}

int cDigito( char ori ) {
    char l = digitosEspecias( ori );

    if ( l == 'a' || l == 'b' || l == 'c' ) {
            return 2;
        } else {
            if ( l == 'd' || l == 'e' || l == 'f' ) {
                return 3;
            } else {
                if ( l == 'g' || l == 'h' || l == 'i' ) {
                    return 4;
                } else {
                    if ( l == 'j' || l == 'k' || l == 'l' ) {
                        return 5;
                    } else {
                        if ( l == 'm' || l == 'n' || l == 'o' ) {
                            return 6;
                        } else {
                            if ( l == 'p' || l == 'q' || l == 'r' || l == 's') {
                                return 7;
                            } else {
                                if ( l == 't' || l == 'u' || l == 'v' ) {
                                    return 8;
                                }
                            }
                        }
                    }
                }
            }
        }
}

void insert( Nodo* root, char* word ) {
    Nodo *puppet = root;
    int digito;
    char *palavra;
    strcmp(palavra, word); 

    while ( *word ) {
        digito = cDigito( *word );

        if ( !puppet->next[digito] ) {
            puppet->next[digito] = (Nodo *)malloc( sizeof(Nodo) );
            puppet->next[digito]->numero = digito;
            puppet->next[digito]->end = false;

            for ( int i = 0; i < AF; i++ ) {
                puppet->next[digito]->next[i] = NULL;
            }
        }
        puppet = puppet->next[digito];
        word++;
    }
    strcmp(puppet->word, palavra); 
    puppet->end = true;
}

Nodo* search( Nodo* root, char* word ) {
    Nodo* puppet = root;

    while ( *word ) {
        unsigned char l = (unsigned char)*word;
        if ( !puppet->next[l] ) {
            return NULL;
        }
        puppet = puppet->next[l];
        word++;
    }
   
    return puppet;    
}

