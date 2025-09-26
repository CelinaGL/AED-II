#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct{
    int linha, coluna;
    int direcao; // 0 = Horizontal | 1 = Vertical | 2 = Diagonal
}Posisao; // usar pra armazenar a posiçao de cada palavra 

void iniciaTabuleiro( char tabuleiro[100][100], int t );
void imprimeTabuleiro( char tabuleiro[100][100], int t );
void inserePalavra( char palavra[100], char tabuleiro[100][100], int t );

int main() {
    srand(time(NULL));
    FILE *arquivoP;
    char palavras[10][100];
    char tabuleiro[100][100];
    char temp[100];
    int tamT, qP; // tamT = Tamanho do tabuleiro | qp = Quantidade de palavras

    do {
        printf("Informe o tamanho do tabuleiro: ");
        scanf("%d", &tamT);
    } while( 0 >= tamT || tamT > 100 );

    iniciaTabuleiro( tabuleiro, tamT );
    imprimeTabuleiro( tabuleiro, tamT );

    do {
        printf("Informe a quantidade de palavras: ");
        scanf("%d", &qP);
    } while( 0 >= qP || qP > 10 );

    arquivoP = fopen("palavras.txt", "r");
    for ( int i = 0; i < qP; i++ ) {
        fgets(temp, 100, arquivoP); 
        if ( strlen(temp) < tamT ) { // tamT porque a palavra não pode ser maior que o tabueleiro
            strcpy(palavras[i], temp);
            inserePalavra( palavras[i], tabuleiro, tamT ); // insere enquanto le
        } else {
            printf("Palavra extrapola o limite do tabuleiro\n");
        }
    }
    fclose(arquivoP);

}

void iniciaTabuleiro( char tabuleiro[100][100], int t ) {
    for ( int i = 0; i < t; i++) {
        for ( int j = 0; j < t; j++ ) {
            tabuleiro[i][j] = 'a' + ( rand() % 26 ); // escolher letras de a-z aleatórias
        }
    }
}

void imprimeTabuleiro( char tabuleiro[100][100], int t ) {
    for ( int i = 0; i < t; i++) {
        for ( int j = 0; j < t; j++ ) {
            printf("%c ", tabuleiro[i][j]);
        }
        printf("\n");
    }
}

void inserePalavra( char palavra[100], char tabuleiro[100][100], int t ) {
    int linha = rand() % t;
    int coluna = rand() % t;
}

/*
strlen(palavra) + x > tamanho do tabuleiro 

for (;;) {
        switch (entrada) {
            case 1:
            imprimeTabuleiro( tabuleiro, tam_t );
            break;

            case 2:
            break;

            case 3:
            exit;
            break;
        }
    }

int menu() {
    int i;
    do {
        printf("\t### Menu ###\n");
        printf("\t1.Mostrar tabuleiro\n");
        printf("\t2.Buscar palavra\n");
        printf("\t3.Sair\n");
        printf("Escolha: ");
        scanf( "%d", &i);
        getchar();
    } while( i <= 0 || i > 3 );
}
*/