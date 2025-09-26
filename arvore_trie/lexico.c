#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define AF 26
#define TAMAP 100

struct Node{
    char letter, m_a;
    int feeling; // Bom = 1 || Neutro = 0 || Ruim = -1
    char tipo[10];
    bool end; 
    struct Nodo *next[AF];
};

typedef struct Node Nodo;

int menu();
Nodo* createTree(); 
void insert( Nodo* root, char* word, int f, char m_a, char* tipo );
void search( Nodo* root, char* word );

int main() {
    FILE* arquivo;
    Nodo* root = NULL;
    int m, f;
    char word[TAMAP], tipo[10], m_a;

    root = createTree;

    arquivo = fopen("lexico_v3.0.txt", "r");
    for ( int i = 0; i < 32191; i++ ) {

        fscanf( "%[^\n]s,", word );
        fscanf( "%[^\n]s,", tipo );
        fscanf("%d,", &f );
        fscanf( "%c", m_a );
        getchar();
    }

    insert( root, word, f, m_a, tipo );

    fclose(arquivo);

    for ( ; ; ) {
        m = menu();
        switch ( m )
        {
        case 1:
            printf( "Digite a palavra: " );
            fgets( word, TAMAP, stdin );
            search( root );
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

void insert( Nodo* root, char* word, int f, char m_a, char* tipo ) {
    Nodo *puppet = root;

    while ( *word ) {
        int l = *word - 'a';

        if ( !puppet->next[l] ) {
            puppet->next[l] = (Nodo *)malloc(sizeof(Nodo));
            puppet->next[l]->letter = *word;
            puppet->next[l]->feeling = 0;
            puppet->next[l]->end = false;
            puppet->next[l]->m_a = m_a;
            strcpy(tipo, puppet->next[l]->tipo);

            for ( int i = 0; i < AF; i++ ) {
                puppet->next[l]->next[i] = NULL;
            }
        }
        puppet = puppet->next[l];
        word++;
    }
    puppet->end = true;
    puppet->feeling = f;
}

void search( Nodo* root, char* word ) {
    Nodo* puppet = root;

    while ( *word ) {
        int l = *word - 'a';

        if ( !puppet->next[l] ) {
            return;
        }
        puppet = puppet->next[l];
        word++;
    }
   
    printf( "Polaridade da palavra: %d", puppet->feeling );
    
}
