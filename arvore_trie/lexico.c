//Explicação de complexidade no final do código
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define AF 256
#define TAMAP 100

struct Node{
    char letter, m_a;
    int feeling; // Bom = 1 || Neutro = 0 || Ruim = -1
    char tipo[10];
    bool end; 
    struct Node *next[AF];
};

typedef struct Node Nodo;

int menu();
Nodo* createTree(); 
void freeTrie( Nodo* node );
void insert( Nodo* root, char* word, int f, char m_a, char* tipo );
Nodo* search( Nodo* root, char* word );
void save( FILE* out, Nodo* root, char* buffer, int depth );

int main() {
    FILE* arquivo;
    Nodo* root = createTree();
    int m, f;
    char word[TAMAP], tipo[10], linha[AF], m_a;

    arquivo = fopen("lexico_v3.0.txt", "r");
    while ( fgets( linha, sizeof(linha), arquivo ) ) {
        int t = strlen(linha);
        linha[t - 1] = '\0';

        sscanf(linha, "%99[^,],%9[^,],%d,%c", word, tipo, &f, &m_a);
        printf( "Inserindo: %s, %s, %d, %c\n", word, tipo, f, m_a ); //Usado pra testar se estava inserindo ou nao
        
        insert( root, word, f, m_a, tipo );
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
                    printf( "Polaridade de '%s' = %d\n", word, r->feeling );
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
                    ra->feeling = f;
                    printf( "Polaridade de '%s' atualizada para %d\n", word, f );
                } else {
                    printf( "Palavra nao encontrada\n" );
                }
                break;
            }

            case 3: {
                FILE* temp = fopen( "lexico_editado.txt", "w" );

                char buffer[TAMAP];
                save( temp, root, buffer, 0 );
                fclose(temp);

                printf( "Arquivo salvo em lexico_editado.txt\n" );
                break;
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
    node->letter = '\0';
    node->feeling = 0;
    node->end = false;
    node->tipo[0] = '\0';
    node->m_a = '\0';
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

void insert( Nodo* root, char* word, int f, char m_a, char* tipo ) {
    Nodo *puppet = root;

    while ( *word ) {
        unsigned char l = (unsigned char)*word;

        if ( !puppet->next[l] ) {
            puppet->next[l] = (Nodo *)malloc( sizeof(Nodo) );
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
    puppet->feeling = f;
    puppet->m_a = m_a;
    strcpy(puppet->tipo, tipo);
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

void save( FILE* out, Nodo* root, char* buffer, int depth ) {
    if ( !root ) {
        return;
    }

    if ( root->end ) {
        buffer[depth] = '\0';
        fprintf( out, "%s,%s,%d,%c\n", buffer, root->tipo, root->feeling, root->m_a );
    }

    for ( int i = 0; i < AF; i++ ) {
        if ( root->next[i] ) {
            buffer[depth] = root->next[i]->letter;
            save( out, root->next[i], buffer, depth + 1 );
        }
    }
}

/*
    Inserir é O(n) porque depende da quantidade de palavras no arquivo e do tamanho delas
    Procurar é O(n) porque também depende do tamanho da palavra
    Editar também é O(n) por causa do tamanho das palavras 
*/