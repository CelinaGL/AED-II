#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define AF 
#define TAMANHO_PALAVRA 100

struct Node{
    char letter;
    int feeling; // Bom = 1 || Neutro = 0 || Ruim = -1 
    bool end; 
    struct Nodo[AF] *next;
};

typedef struct Node Nodo;

int menu();
Nodo* createTree(); 
void insert( Nodo* root, char* word, int s );
bool search( Nodo* root, char* word );

int main() {
    int m;

     for ( ; ; ) {
        m = menu();
        switch ( m )
        {
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            exit(0);
            break;
        }
    }

    return 0;
}

int menu() {
    int m;
    do {
        printf("\t### Menu ###\n");
        printf("\t1.Busca de polaridade\n");
        printf("\t2.Edicao de polaridade\n");
        printf("\t3.Salvamento de arquivo\n");
        printf("\t4.Sair\n");
        printf("Escolha: ");
        scanf( "%d",m);
        getchar();
    } while( m <= 0 || m> 5 );
}

Nodo* createTree() {
    Nodo *node = (Nodo *)malloc( sizeof(Nodo) );
    node->letter = NULL;
    node->feeling = NULL;
    node->end = false;
    for ( int i = 0; i < AF; i++ ) {
        node->next[i] = NULL;
    }
    return node;
}

void insert( Nodo* root, char* word, int s ) {
    Nodo* puppet = root;

    while ( *word ) {
        int l = *word - 'a';

        if ( !puppet->next[l] ) {
            puppet->next[l] = (Nodo *)malloc(sizeof(Nodo));
            puppet->next[l]->letter = *word;
            puppet->next[l]->feeling = 0;
            puppet->next[l]->end = false;
            for ( int i = 0; i < AF; i++ ) {
                puppet->next[l]->next[i] = NULL;
            }
        }
        puppet = puppet->next[l];
        word++;
    }
    puppet->end = true;
    puppet->feeling = s;
}

bool search( Nodo* root, char* word ) {
    Nodo* puppet = root;

    while ( *word ) {
        int l = *word - 'a';

        if ( !puppet->next[l] ) {
            return false;
        }
        puppet = puppet->next[l];
        word++;
    }
   
    return puppet->end;
}
