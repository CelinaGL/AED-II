//O código não esta completo, não consegui teminar, apenas li o arquivo, fiz uma tabela com as frequencias e
//um merge sort para ordenar a tabela
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define AF 256
#define TAMAP 100

struct Node{
    unsigned char l;
    int freq;
    struct Node *left, *right;//0 = esquerda | 1 = direita
};
typedef struct Node Nodo;

//fazer uma vetor talvez pra ver o q mais se repete e ordenar com um merge

void frequencia( char* word, int freq[AF] );
void verificarFreq( int freq[AF] );
Nodo* criaNodoTabela( unsigned char l, int f );
void mergeSort( Nodo** tabela, int left, int right );
void merge( Nodo** tabela, int left, int meio, int right );
void verificarTabela( Nodo** tabela, int tam );
void freeTree( Nodo* root );

int main() {
    FILE* arquivo;
    char word[TAMAP];
    int freq[AF];

    for( int i = 0; i < AF; i++ ) {
        freq[i] = 0;
    }

    arquivo = fopen("palavras.txt", "r");
    while ( fgets( word, sizeof(word), arquivo ) ) {
        size_t t = strlen(word);
        word[t - 1] = '\0';

        //printf( "Lendo: %s\n", word ); //Usado pra testar se estava inserindo ou nao
        frequencia( word, freq );
    } 

    int tamF = 0;
    Nodo* tabela[AF];
    for ( int i = 0; i < AF; i++ ) {//cria a tabela
        if ( freq[i] > 0 ) {
            tabela[tamF] = criaNodoTabela( (unsigned char)i, freq[i] );
            tamF++;
        }
    }
    mergeSort( tabela, 0, tamF - 1 );

    verificarFreq(freq);
    printf("\n");
    verificarTabela( tabela, tamF );
    fclose(arquivo);
    for ( int i = 0; i < tamF; i++ ) {
        free(tabela[i]);
    }
    //freeTree(root);
    return 0;
}

void frequencia( char* word, int freq[AF] ){
    while (*word) {
        freq[(unsigned char)*word]++;
        word++;
    }
}

void verificarFreq( int freq[AF] ) {
    printf("FREQUENCIA\n");
    for( int i = 0; i < AF; i++ ) {
        if ( freq[i] > 0 ) {
            printf("%d\n", freq[i]);
        }
    }
}

Nodo* criaNodoTabela( unsigned char l, int f ) {
    Nodo *node = (Nodo *)malloc( sizeof(Nodo) );
    node->l = l;
    node->freq = f;
    node->left = NULL;
    node->right = NULL;   
    return node;
}

void mergeSort( Nodo** tabela, int left, int right ) {
    if ( left < right ) {
        int meio = left + ( right - left ) / 2;

        mergeSort( tabela, left, meio );
        mergeSort( tabela, meio + 1, right );

        merge( tabela, left, meio, right );
    }
}

void merge( Nodo** tabela, int left, int meio, int right ) {
    int n1 = meio - left + 1;
    int n2 = right - meio;

    Nodo **l = (Nodo **)malloc( n1 * sizeof(Nodo *) );
    Nodo **r = (Nodo **)malloc( n2 * sizeof(Nodo *) );

    for ( int i = 0; i < n1; i++ ) {
        l[i] = tabela[left + i];
    }
    for ( int j = 0; j < n2; j++ ) {
        r[j] = tabela[meio + 1 + j];
    }

    int i, j, k;
    i = 0; 
    j = 0; 
    k = left;

    while ( i < n1 && j < n2 ) {
        if ( l[i]->freq >= r[j]->freq ) {
            tabela[k] = l[i];
            i++;
        } else {
            tabela[k] = r[j];
            j++;
        }
        k++;
    }

    while ( i < n1 ) {
        tabela[k++] = l[i++];
    }
    while ( j < n2 ) {
        tabela[k++] = r[j++];
    }

    free(l);
    free(r);
}

void verificarTabela( Nodo** tabela, int tam ) {
    printf("VETOR ORDENADO\n");
    for ( int i = 0; i < tam; i++ ) {
        if ( tabela[i]->freq > 0 ) {   
            printf( "%d\n", tabela[i]->freq );
        }
    }
}


void freeTree( Nodo* root ) {
    if (!root) { 
        return;
    }
    freeTree( root->left );
    freeTree( root->right );
    free(root);
}
/*
- Cálculo de bits necessários (Entropia):
        S{x} = -log{2}(P{x}) // vai ter q ser ( -log(P{x}) / log(2) ) a n ser q tenha log de 2 direto
    x = símbolo     
    S{x} = entropia de x
    P{x} = probabilidade de x
    usar pra testar se a quantidade de bits dá um número próximo
*/