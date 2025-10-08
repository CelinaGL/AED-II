#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define AF 10
#define TAMAP 100
#define QUANP 10000

struct ListaPalavras { //Lista encadeada para guardar as palavras
    char word[100];
    struct ListaPalavras* next;
};
typedef struct ListaPalavras Palavras;

struct Node {
    bool end; 
    Palavras* words;
    struct Node *next[AF];
};
typedef struct Node Nodo;

Nodo* createTree(); 
Palavras* createLista( char* word );
void freeTrie( Nodo* node );
void insert( Nodo* root, char* word );
Nodo* search( Nodo* root, char* word );
char digitosEspeciais( unsigned char *p, int *bytes_consumidos );
int cDigito( char l );
void listar( Nodo* root );

int main() {
    FILE* arquivo;
    Nodo* root = createTree();
    char word[TAMAP];

    arquivo = fopen("dicionario.txt", "r");
    while ( fgets( word, sizeof(word), arquivo ) ) {
        size_t t = strlen(word);
        word[t - 1] = '\0';

        //printf( "Inserindo: %s\n", word ); //Usado pra testar se estava inserindo ou nao
        insert( root, word );
    } 
    fclose(arquivo);

    char num[TAMAP];
    printf( "Digite um prefixo: " );
    scanf( "%[^\n]s", num );

    Nodo* found = search( root, num );
    if (found) {
        printf( "Palavras com o prefixo %s:\n", num );
        listar( found );
    } else {
        printf( "Nenhuma palavra encontrada\n" );
        }

    freeTrie(root);

    return 0;
}

Nodo* createTree() {
    Nodo *node = (Nodo *)malloc( sizeof(Nodo) );
    node->end = false;
    node->words = NULL;
    for ( int i = 0; i < AF; i++ ) {
        node->next[i] = NULL;
    }
    return node;
}

Palavras* createLista( char* word ) {
    Palavras *p = (Palavras *)malloc( sizeof(Palavras) );
    strcpy( p->word, word );
    p->next = NULL;
    return p;
}

void freeTrie( Nodo* node ) {
    if (!node) {
        return;
    }
    for (int i = 0; i < AF; i++) {
        freeTrie(node->next[i]);
    }

    Palavras* temp = node->words;
    while (temp) {
        Palavras* aux = temp;
        temp = temp->next;
        free(aux);
    }
    free(node);
}

char digitosEspeciais( unsigned char *p, int *bytes_consumidos ) {
    unsigned char l = p[0];
    *bytes_consumidos = 1;
    
    if ( l == 0xC3 ) {
        unsigned char l2 = p[1];
        *bytes_consumidos = 2;
        
    switch (l2) {

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

    }
}
    
    if ( l >= 'A' && l <= 'Z' ) {
        return l + 32;
    }
    if ( l >= 'a' && l <= 'z' ) {
        return l;
    }
    return 0;
}

int cDigito( char l ) {
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
    return 9;
}

void insert( Nodo* root, char* word ) {
    Nodo *puppet = root;
    unsigned char* p = (unsigned char*)word;
    int bytes;
    char letra;

    while ( *p ) {
        letra = digitosEspeciais( p, &bytes );
        p += bytes;
        if ( !letra ) {
            continue;
        } 

        int dig = cDigito(letra);
        if (dig == 0) {
            continue;
        }

        if (!puppet->next[dig]) {
            puppet->next[dig] = createTree();
        }
        puppet = puppet->next[dig];
    }

    puppet->end = true;

    Palavras* nova = createLista( word );
    nova->next = puppet->words;
    puppet->words = nova;
}

Nodo* search( Nodo* root, char* word ) {
    Nodo* puppet = root;

    while (*word) {
        int d = *word - '0';
        if ( d < 0 || d >= AF || !puppet->next[d] ){
            return NULL;
        }
        puppet = puppet->next[d];
        word++;
    }
    return puppet;
}

void listar( Nodo* root ) {
    if (!root) {
        return;
    }
    
    Palavras* p;
    for ( p = root->words; p != NULL; p = p->next ) {
        printf( "%s\n", p->word );
    }

    for ( int i = 0; i < AF; i++ ) {
        if (root->next[i]) {
            listar( root->next[i] );
        }
    }
}