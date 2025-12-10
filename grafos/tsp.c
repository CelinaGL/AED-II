#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
    int id; //se for -1, entao foi removido
    double x, y;
}Cidades;

struct Nodo{
    int id;
    double dist;
    struct Nodo* next;
};

typedef struct Nodo Node;

typedef struct {
    int arestas;
    Node* head;
}Lista;

int menu();
void menuDB( Cidades* v, double** ma, Lista* l, int tam );
void removerVertice( Cidades* v, double** m, Lista* l, int tam);
void inserirVetor( Cidades** vetor, int id, double x, double y, int* n );
void removerVerticeVetor( Cidades* v, int id );
void lerVetor( Cidades* v, int tam );
double distanciaE( double x1, double y1, double x2, double y2 );
void inserirMatrizLinha( Cidades* v, double** matriz, int n );
void lerMatriz( double** m, int tam );
void removerVerticeMatriz( double** m, int id, int tam );
int verificarVertice( Cidades* v, int ver, int tam );
void addVertice( Cidades** v, int* tam, double*** matriz, Lista** lista );
void inserirVerticeMatriz( Cidades* v, double*** m, int novoTam, int id, int tamAntigo );
void addNodo( Lista* lista, int idOrigem, int idDestino, double dist );
Lista* criarListaAdj( Cidades* v, int n );
void lerLista( Lista* lista, int tam );
void inserirVerticeLista( Cidades* v, Lista** l, int novoTam, int id, int tamAntigo );
void removerVerticeLista( Lista* lista, int id, int tam );
void verificarAresta( Cidades* v, double** m, Lista* l, int tam );
int verificarArestaM( double** m, int id1, int id2 );
int verificarArestaL( Lista* l, int id1, int id2 );
void editarAresta( Cidades* v, double** m, Lista* l, int tam );
void editarArestaM( double** m, int id1, int id2, double dist );
void editarArestaL( Lista* l, int id1, int id2, double dist );
void relatorio();

int main() {
    FILE* arquivo;
    Cidades* vetor = (Cidades *)malloc( sizeof(Cidades) );
    char linha[100], node[] = "NODE_COORD_SECTION";
    int id, n = 0;  //tamanho
    double x, y;

    printf("Para este codigo foi usado o arquivo dj38.tsp\n");

    arquivo = fopen( "dj38.tsp", "r" );
    if ( !arquivo ) {
        printf("Erro ou arquivo nao encontrado");
        return 1;
    }

    while( fscanf( arquivo, "%s", linha ) == 1 ) {
        if ( strcmp( linha, node ) == 0 ) {
            break;
        } 
    }

    while ( fscanf( arquivo, "%d %lf %lf", &id, &x, &y ) == 3 ) {
        inserirVetor( &vetor, id, x, y, &n );
    }
    fclose(arquivo);

    //lerVetor( vetor, n ); //testar se ta inserindo certo do arquivo

    double** matriz = NULL;
    Lista* lista = NULL;

    if ( n > 0)  {
        matriz = malloc( sizeof(double*) * (long long unsigned)(n) );
        for( int i = 0; i < n; i++ ) {
            matriz[i] = malloc( sizeof(double) * (long long unsigned)(n) );
        }
        inserirMatrizLinha( vetor, matriz, n );
        lista = criarListaAdj( vetor, n );
    }
    //lerMatriz( matriz, n ); //testar se ta funcionando

    for ( ; ; ) {
        int m = menu();
        switch ( m ) {
            case 1: {
                addVertice( &vetor, &n, &matriz, &lista );
                break;
            }

            case 2: {
                removerVertice( vetor, matriz, lista, n );
                break;
            }

            case 3: {
                verificarAresta( vetor, matriz, lista, n );
                break;
            }
        
            case 4: {
                editarAresta( vetor, matriz, lista, n );
                break;
            }

            case 5: {
                relatorio();
                break;
            }

            case 6: {
                menuDB( vetor, matriz, lista, n ); 
                break;
            }
            case 7: {
                if (matriz) {
                    for( int i = 0; i < n; i++ ) {
                        free( matriz[i] );
                    }
                    free(matriz);
                }

                if (lista) {
                    for ( int i = 0; i < n; i++ ) {
                        Node* atual = lista[i].head;

                        while ( atual != NULL ) {
                            Node* temp = atual;
                            atual = atual->next;
                            free(temp); // Libera cada nó da lista encadeada
                        }
                    }
                    free(lista); 
                }
                free(vetor);
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
        printf( "\t1.Adicionar Cidade\n" );
        printf( "\t2.Remover Cidade\n" );
        printf( "\t3.Busca de Arestas\n" );
        printf( "\t4.Editar Aresta\n" );
        printf( "\t5.Relatorio(e provavel que esteja dando problema com os acentos, esta na ultima funcao do codigo)\n" );
        printf( "\t6.Ler\n" );
        printf( "\t7.Sair\n" );
        printf( "Escolha: " );
        scanf( "%d", &m );
        getchar();
    } while( m <= 0 || m > 7 );
    return m;
}

void menuDB( Cidades* v, double** ma, Lista* l, int tam ) {
    int m;
    do {
        printf( "\t### Ler oq? ###\n" );
        printf( "\t1.Ler Vetor\n" );
        printf( "\t2.Ler Matriz\n" );
        printf( "\t3.Ler Lista\n" );
        printf( "\t4.Sair\n" );
        printf( "Escolha: " );
        scanf( "%d", &m );
        getchar();
    } while( m <= 0 || m > 4 );

    switch (m) {
        case 1:{
            lerVetor( v, tam );
            break;
        }
        case 2:{
            lerMatriz( ma, tam );
            break;
        }
        case 3:{
            lerLista( l, tam );
            break;
        }
        case 4:{
            return;
            break;
        }
    }

}

void removerVertice( Cidades* v, double** m, Lista* l, int tam) {
    int id, validacao1, validacao2;
    
    do {
        printf( "ID do vertice: " );
        validacao1 = scanf( "%d", &id );

        if ( id <= 0 || validacao1 != 1 ) {
            printf( "ID invalido, tente novamente\n" );
            while (getchar() != '\n');
        }

        validacao2 = verificarVertice( v, id, tam );
        if ( id > 0 && validacao2 == 0 ) {
            printf( "ID nao existe, tente novamente\n" );
            while (getchar() != '\n');
        }

    }while( id <= 0 || validacao1 != 1 || validacao2 == 0 );

    removerVerticeVetor( v, id );
    removerVerticeMatriz( m, id, tam );
    removerVerticeLista( l, id, tam );

    printf( "Vertice removido\n");
}

void inserirVetor( Cidades** v, int id, double x, double y, int* n ) {
    if ( id > *n ) {
        Cidades* temp = realloc(*v, (long long unsigned)id * sizeof(Cidades));
        *v = temp;

        for ( int i = *n; i < id; i++ ) {
            (*v)[i].id = -1;   
            (*v)[i].x = 0.0;
            (*v)[i].y = 0.0;
        }

        *n = id;
    }
    Cidades* puppet = (Cidades*)(*v) + ( id - 1 );

    puppet->id = id;
    puppet->x = x;
    puppet->y = y;
}

void removerVerticeVetor( Cidades* v, int id ) {
    v[id - 1].id = -1;
    v[id - 1].x = 0;
    v[id - 1].y = 0;
}   

void lerVetor( Cidades* v, int tam ) {
    Cidades* puppet = v;

    for( int i = 0; i < tam; i++ ) {
        if ( puppet->id != -1 ) {
            printf( "ID: %d    X: %lf    Y: %lf\n", puppet->id, puppet->x, puppet->y );
        } else {
            printf( "ID Removido\n" );
        }
        puppet++;
    }
}

double distanciaE( double x1, double y1, double x2, double y2 ) {
    //distancia entre as cidades
    return sqrt( pow( x2 - x1, 2 ) + pow( y2 - y1, 2 ) );
}

void inserirMatrizLinha( Cidades* v, double** matriz, int n ) {//( vetor, matriz, tam do vetor/matriz ) 
    int m = 1;

    for ( int i = 0; i < n; i++ ) {
        Cidades* puppet1 = v + i;

        for ( int j = 0; j < m; j++ ) {
            Cidades* puppet2 = v + j;
            if ( i == j ) {
                matriz[i][j] = 0;
            } else {
                matriz[i][j] = matriz[j][i] = distanciaE( puppet1->x, puppet1->y, puppet2->x, puppet2->y );
            }
        }

        if ( m < n ) {
            m++;
        }
    }
}

void lerMatriz( double** m, int tam ) {

    for( int i = 0; i < tam; i++ ){
        for( int j = 0; j < tam; j++ ) {
            printf( "[%d][%d]: %lf ", i, j, m[i][j] );
        }
        printf("\n"); 
    }
}

void removerVerticeMatriz( double** m, int id, int tam ) {
    for ( int i = 0; i < tam; i++) {
        m[i][id - 1] = m[id - 1][i] = 0;
    }
}

int verificarVertice( Cidades* v, int ver, int tam ) {//0 = nao existe ou foi removido | 1 = existe
    if ( ver > tam ) {
        return 0;
    }

    Cidades* puppet = v + (ver - 1); 
    if ( puppet->id == -1 ) {
        return 0;
    }

    return 1;
}

void addVertice( Cidades** v, int* tam, double*** matriz, Lista** lista ) {
    int id, validacao1, validacao2;

    do {
        printf( "ID do vertice: " );
        validacao1 = scanf( "%d", &id );

        if ( id <= 0 || validacao1 != 1 ) {
            printf( "ID invalido, tente novamente\n" );
            while (getchar() != '\n');
        }

        validacao2 = verificarVertice( *v, id, *tam );
        if ( id > 0 && validacao2 == 1 ) {
            printf( "ID ja existente, tente novamente\n" );
            while (getchar() != '\n');
        }

    }while( id <= 0 || validacao1 != 1 || validacao2 == 1 );

    double x, y;
    do {
        printf( "X: " );
        validacao1 = scanf("%lf", &x);

        if ( y <= 0 || validacao1 != 1 ) {
            printf( "X invalido, tente novamente\n" );
            while (getchar() != '\n');
        }

    }while( x <= 0 || validacao1 != 1 );

    do {
        printf( "Y: " );
        validacao1 = scanf("%lf", &y);

        if ( y <= 0 || validacao1 != 1 ) {
            printf( "Y invalido, tente novamente\n" );
            while (getchar() != '\n');
        }

    }while( y <= 0 || validacao1 != 1 );

    int tamAntigo = *tam;
    int novoTam;
    
    if ( id > tamAntigo ) {
        novoTam = id;
    } else {
        novoTam = tamAntigo;
    }

    inserirVetor( v, id, x, y, tam );

    if ( novoTam > *tam ) {
        *tam = novoTam;
    }

    inserirVerticeMatriz( *v, matriz, *tam, id, tamAntigo );
    inserirVerticeLista( *v, lista, *tam, id, tamAntigo );
    printf( "Vertice adicionado\n");
    
}

void inserirVerticeMatriz( Cidades* v, double*** m, int novoTam, int id, int tamAntigo ) {
    int novoID = id - 1;
    Cidades* puppet1 = v + novoID;

    if ( novoTam > tamAntigo ) {
        double** temp = realloc( *m, (long long unsigned)novoTam * sizeof(double*) );
        *m = temp;

        for ( int i = 0; i < tamAntigo; i++ ) {
            (*m)[i] = realloc((*m)[i], (long long unsigned)novoTam * sizeof(double) );
            for (int j = tamAntigo; j < novoTam; j++) {
                 (*m)[i][j] = 0; 
            }
        }

        for (int i = tamAntigo; i < novoTam; i++) {
            (*m)[i] = malloc( (long long unsigned)novoTam * sizeof(double) );
             for ( int j = 0; j < novoTam; j++ ) {
                (*m)[i][j] = 0; 
            }
        }
    }

    for ( int i = 0; i < novoTam; i++ ) {
        Cidades* puppet2 = v + i;
        if ( puppet2->id != -1 ) { 
            if ( i == novoID ) {
                (*m)[novoID][i] = 0.0;
            } else {
                double dist = distanciaE( puppet2->x, puppet2->y, puppet1->x, puppet1->y );
                (*m)[i][novoID] = (*m)[novoID][i] = dist;
            }
        }
    }
}

void addNodo( Lista* lista, int idOrigem, int idDestino, double dist ) {
    Node* novo = (Node *)malloc( sizeof(Node));
    novo->id = idDestino;
    novo->dist = dist;
    novo->next = NULL;

    if ( lista[idOrigem].head == NULL ) {
        lista[idOrigem].head = novo;
        lista[idOrigem].arestas = 1;
    } else {
        Node* puppet = lista[idOrigem].head;
        
        while ( puppet->next != NULL ) {
            puppet = puppet->next;
        }
        puppet->next = novo;
        lista[idOrigem].arestas++;
    }
}

Lista* criarListaAdj( Cidades* v, int n ) {
    Lista* lista = (Lista *)malloc( (long long unsigned)n * sizeof(Lista) );
    
    for( int i = 0; i < n; i++ ) {
        lista[i].head = NULL;
        lista[i].arestas = 0;
    }
    
    for ( int i = 0; i < n; i++ ) {
        Cidades* puppet1 = v + i;

        if ( puppet1->id == -1 ){ 
            continue; 
        }

        for ( int j = 0; j < n; j++ ) {
            Cidades* puppet2 = v + j;  

            if ( i != j && puppet2->id != -1 ) {
                double dist = distanciaE( puppet1->x, puppet1->y, puppet2->x, puppet2->y );
                addNodo( lista, i, puppet2->id, dist );
            }
        }
    }
    return lista;
}

void lerLista( Lista* lista, int tam ) {

    for ( int i = 0; i < tam; i++ ) {
        Node* puppetN = lista[i].head;
        printf( "Vertice = %d\n", i + 1 ); 
        printf( "Quantidade de Arestas = %d\n", lista[i].arestas ); 
        for ( int j = 0; j < lista[i].arestas; j++ ) {
            printf("Distancia ate o ID %d: %lf\n", puppetN->id, puppetN->dist );
            puppetN = puppetN->next;
        }
        printf("\n");
    }
}

void inserirVerticeLista( Cidades* v, Lista** l, int novoTam, int id, int tamAntigo ) {
    int novoID = id - 1;
    Cidades* puppet1 = v + novoID;

    if (novoTam > tamAntigo) {
        Lista* temp = realloc(*l, (long long unsigned)novoTam * sizeof(Lista));
        *l = temp;

        for (int i = tamAntigo; i < novoTam; i++) {
            (*l)[i].head = NULL;
            (*l)[i].arestas = 0;
        }
    }

    for (int i = 0; i < novoTam; i++) {

        if (i != novoID && v[i].id != -1) {
            Cidades* puppet2 = v + i;
            double dist = distanciaE( puppet2->x, puppet2->y, puppet1->x, puppet1->y );

            addNodo( *l, novoID, puppet2->id, dist );
            addNodo( *l, i, id, dist );
        }
    }
}

void removerVerticeLista( Lista* lista, int id, int tam ) {
    int idRemover = id - 1;

    Node* atual = lista[idRemover].head;
    while ( atual != NULL ) {
        Node* temp = atual;
        atual = atual->next;
        free(temp); 
    }
    lista[idRemover].head = NULL;
    lista[idRemover].arestas = 0;

    
    for ( int i = 0; i < tam; i++ ) {
        if ( i == idRemover ) {
            continue;
        }

        Node* r = lista[i].head;
        Node* prox = NULL;

        while ( r != NULL ) {
            if ( r->id == id ) {
                if ( prox == NULL ) {
                    lista[i].head = r->next;
                } else {
                    prox->next = r->next;
                }

                free(r);
                lista[i].arestas--;
                break; 
            }
            

            prox = r;
            r = r->next;
        }
    }
}

void verificarAresta( Cidades* v, double** m, Lista* l, int tam ) {
    int id1, id2, validacao1, validacao2;
    
    do {
        printf( "Primeiro ID: " );
        validacao1 = scanf( "%d", &id1 );

        if ( id1 <= 0 || validacao1 != 1 ) {
            printf( "ID invalido, tente novamente\n" );
            while (getchar() != '\n');
        }

        validacao2 = verificarVertice( v, id1, tam );
        if ( id1 > 0 && validacao2 == 0 ) {
            printf( "ID nao existe, tente novamente\n" );
            while (getchar() != '\n');
        }

    }while( id1 <= 0 || validacao1 != 1 || validacao2 == 0 );

    do {
        printf( "Segundo ID: " );
        validacao1 = scanf( "%d", &id2 );

        if ( id2 <= 0 || validacao1 != 1 ) {
            printf( "ID invalido, tente novamente\n" );
            while (getchar() != '\n');
        }

        validacao2 = verificarVertice( v, id2, tam );
        if ( id2 > 0 && validacao2 == 0 ) {
            printf( "ID nao existe, tente novamente\n" );
            while (getchar() != '\n');
        }

    }while( id2 <= 0 || validacao1 != 1 || validacao2 == 0 );

    int vm = verificarArestaM( m, id1, id2 );
    int vl = verificarArestaL( l, id1, id2 );

    if ( vm == vl ) {
        if ( vm == 1 ) {
            printf( "Existe\n" );
            } else if ( vm == 0 ) {
                printf( "Nao existe\n" );
            }   
    } else {
        printf( "Deu problema\n" );
    }
}

int verificarArestaM( double** m, int id1, int id2 ) {//0 = nao existe ou foi removido | 1 = existe
    if ( id1 == id2 ) {
        return 0;
    }
    if ( m[id1 - 1][id2 - 1] != 0 ) {
        return 1;
    }
    return 0;
}

int verificarArestaL( Lista* l, int id1, int id2 ) {//0 = nao existe ou foi removido | 1 = existe
    if ( id1 == id2 ) {
        return 0;
    }
    Node* puppet = l[id1 - 1].head;

    while ( puppet != NULL ) {
        if ( puppet->id == id2 ) {
            if ( puppet->dist != 0 ) {
                return 1;
            }
            break;
        }
        puppet = puppet->next;
    }
    return 0;
}

void editarAresta( Cidades* v, double** m, Lista* l, int tam ) {
    int id1, id2, validacao1, validacao2;
    double novaDist;

    do {
        printf( "ID da Origem: " );
        validacao1 = scanf( "%d", &id1 );

        if ( id1 <= 0 || validacao1 != 1 ) {
            printf( "ID invalido, tente novamente\n" );
            while (getchar() != '\n');
        } else {
            validacao2 = verificarVertice( v, id1, tam ); 
            if ( validacao2 == 0 ) {
                printf( "ID nao existe, tente novamente\n" );
            }
        }
    } while( id1 <= 0 || validacao1 != 1 || validacao2 == 0 );

    do {
        printf( "ID do Destino: " );
        validacao1 = scanf( "%d", &id2 );
        if ( id1 <= 0 || validacao1 != 1 ) {
            printf( "ID invalido, tente novamente\n" );
            while (getchar() != '\n');
        } else {
            validacao2 = verificarVertice( v, id2, tam ); 
            if ( validacao2 == 0 ) {
                printf( "ID nao existe, tente novamente\n" );
            }
        }
    } while( id2 <= 0 || validacao1 != 1 || validacao2 == 0 );

    if ( verificarArestaM( m, id1, id2 ) == 0 ) {
        printf( "Aresta nao existe\n" );
        return;
    }

    do {
        printf( "Digite a nova distancia: " );
        validacao1 = scanf( "%lf", &novaDist );

        if ( novaDist <= 0 || validacao1 != 1 ) {
            printf( "Distancia invalida, tente novamente\n" );
            while (getchar() != '\n');
        }

    }while( novaDist <= 0 || validacao1 != 1 );

    editarArestaM( m, id1, id2, novaDist );
    editarArestaL( l, id1, id2, novaDist );

    printf( "Aresta alterada\n" );
}

void editarArestaM( double** m, int id1, int id2, double dist ) {
    m[id1 - 1][id2 - 1] = dist;
    m[id2 - 1][id1 - 1] = dist;
}

void editarArestaL( Lista* l, int id1, int id2, double dist ) {
    Node* puppet = l[id1 - 1].head;
    
    while ( puppet != NULL ) {
        if ( puppet->id == id2 ) {
            puppet->dist = dist;
            break; 
        }
        puppet = puppet->next;
    }

    puppet = l[id2 - 1].head;
    while ( puppet != NULL ) {
        if ( puppet->id == id1 ) {
            puppet->dist = dist;
            break; 
        }
        puppet = puppet->next;
    }
}

void relatorio() {
    printf( "N = número de vértices \n" );

    printf( "\t\t### Matriz de Adjacencias ###\n" );
    printf( "* Espaco: O(N^2) \n" );
    printf( "\n" );

    printf( "* Ler do arquivo \n" );
    printf( "\t- Tempo: O(N^2)\n" );
    printf( "Apesar da funçãoo de leitura inicial da matriz ter sido feita para que a mesma distância não fosse calculada duas vezes, ainda continua sendo O(N^2), ignorando a constante 1/2\n" );
    printf( "Porém é mais répida do que calcular distância por distância  \n" );
    printf( "\n" );

    printf( "* Adicionar \n" );
    printf( "\t- Tempo: O(N^2)\n" );
    printf( "\n" );

    printf( "* Remover:\n" );
    printf( "\t- Tempo: O(N)\n" );
    printf( "\n" );

    printf( "* Buscar: \n" );
    printf( "\t- Tempo: O(1)\n" );
    printf( "Apenas pega a localização do vértice, não precisa percorrer toda a matriz\n" );
    printf( "\n" );

    printf( "* Editar: \n" );
    printf( "\t- Tempo: O(1)\n" );
    printf( "Apenas pega a localização do vértice, não precisa percorrer toda a matriz\n" );
    printf( "\n" );


    printf( "\t\t### Lista de Adjacencias ###\n" );
    printf( "* Espaco: O(N^2) \n" );
    printf( "\n" );

    printf( "* Ler do arquivo \n" );
    printf( "\t- Tempo: O(N^2)\n" );
    printf( "\n" );

    printf( "* Adicionar \n" );
    printf( "\t- Tempo: O(N^2)\n" );
    printf( "Foi feito inserindo no final da lista, por isso O(N^2)\n" );
    printf( "Após excluir um vértice do meio da lista e adicionar outro vértice com o mesmo ID do excluído, ele é colocado no final da lista como mencionado antes, não tive tempo de fazer ele ficar no 'lugar certo'\n" );
    printf( "\n" );

    printf( "* Remover:\n" );
    printf( "\t- Tempo: O(N^2)\n" );
    printf( "\n" );

    printf( "* Buscar: \n" );
    printf( "\t- Tempo: O(N)\n" );
    printf( "Apesar de pegar a posição da lista, precisa percorrer nos nodos até achar a aresta desejada\n" );
    printf( "\n" );

    printf( "* Editar: \n" );
    printf( "\t- Tempo: O(N)\n" );
    printf( "Apesar de pegar a posição da lista, precisa percorrer nos nodos até achar a aresta desejada\n" );
    printf( "\n" );


    printf( "\t\t### Analise Comparativa ###\n" );
    printf( "Para grafos densos os dois jeitos tem o mesmo desempenho em relação ao espaço ocupado, mas a Matriz é mais vantajosa pos causa das operações que tem complexidade O(1),\n" );
    printf( "enquanto para as mesmas operações a Lista tem O(N)\n" );
    printf( "Já para grafos esparços, a Lista se sobressai em quesito de espaço ocupado, já que ela é dinâmica e só armazena os vértices necessário nos nodos\n" );

}