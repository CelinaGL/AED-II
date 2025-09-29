#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define AF 26
#define TAMANHO_PALAVRA 100

struct Node{
    char letter;
    int feeling; // Bom = 1 || Neutro = 0 || Ruim = -1 
    bool end; 
    struct Node *next[AF];
};

typedef struct Node Nodo;

Nodo* createTree(); 
void insert( Nodo* root, char* word, int s );
bool search( Nodo* root, char* word );
Nodo* keysWithPrefix( Nodo* root, char* pre );
void freeTrie(Nodo* node);

int main() {
    Nodo* root = createTree();

    insert(root, "casa", 2);
    insert(root, "carro", 1);
    insert(root, "cachorro", 0);
    insert(root, "gato", -1);
    insert(root, "cobra", -2);
    insert(root, "casar", -2);

    printf("Busca por 'casa': %s\n", search(root, "casa") ? "Encontrada" : "Nao encontrada");
    printf("Busca por 'co': %s\n", search(root, "co") ? "Encontrada" : "Nao encontrada");

    //printf("\nPalavras com prefixo 'ca':\n");
    //keysWithPrefix(root, "ca");

    //printf("\nPalavras com prefixo 'co':\n");
    //printWordsWithPrefix(root, "co");

    freeTrie(root);
    return 0;

    return 0;
}

Nodo* createTree() {
    Nodo *node = (Nodo *)malloc( sizeof(Nodo) );
    node->letter = '-';
    node->feeling = 0;
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
Nodo* keysWithPrefix( Nodo* root, char* pre ){
    Nodo* puppet = root;

    while ( *pre ) {
        int l = *pre - 'a';

        if ( !puppet->next[l] ) {
            return NULL;
        }
        puppet = puppet->next[l];
        pre++;
    }
   
    return puppet;
}

void freeTrie(Nodo* node) {
    if (!node) return;
    for (int i = 0; i < AF; i++) freeTrie(node->next[i]);
    free(node);
}