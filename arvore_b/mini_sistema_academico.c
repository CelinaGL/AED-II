#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHAVES 4
#define MIN_CHAVES ( CHAVES/2 )
#define FILHOS ( CHAVES + 1 )

typedef struct {
    int matricula; //Chave Primária
    char nome[100];
}Aluno;

typedef struct {
    int codigo; //Chave Primária
    char nome[100];
}Disciplinas;

typedef struct {
    int id; //Chave Primária
    int matricula_aluno; //Chave Estrangeira
    int codigo_disciplina; //Chave Estrangeira
    float media;
}Matriculas;

typedef struct {
    int n; //numero de chaves na página atualmente
    int folha; //0 = nó interno || 1 = folha
    int id[CHAVES];
    long long pos[CHAVES]; //posição das chaves
    //Pagina* filho[FILHOS];
    long long filhos[FILHOS]; //posição dos filhos
}Pagina;

void menuPrincipal( FILE* indiceAlunos, FILE* dadosAlunos );
void menuAlunos( FILE* indice, FILE* dados );
void menuDisciplinas();
void menuMatriculas();
Pagina* criaPagina( int folha );
long long abreIndice( FILE* indice );
void dividirPag( long long posiP, long long posiF, int posi, FILE* indice );
void addChave_naoCheio( long long posi, int id, long long dados, FILE* indice );
long long inserir( FILE* indice, long long posiRaiz, int id, long long dados );
void addAluno( FILE* arquivo, FILE* indice, long long* root );
void imprimeAluno(FILE *dados, long long pos);
void imprimePagAluno(FILE *indice, FILE *dados, long long posPag);

int main() {
    printf("DEBUG: sizeof(Aluno)=%zu, sizeof(Pagina)=%zu, sizeof(long long)=%zu\n",
       sizeof(Aluno), sizeof(Pagina), sizeof(long long));

    FILE *indiceAlunos = fopen("alunos.idx", "r+b");
    if (!indiceAlunos) indiceAlunos = fopen("alunos.idx", "w+b");

    FILE *dadosAlunos = fopen("alunos.data", "r+b");
    if (!dadosAlunos) dadosAlunos = fopen("alunos.data", "w+b");
    
    menuPrincipal( indiceAlunos, dadosAlunos );

    fclose(indiceAlunos);
    fclose(dadosAlunos);
    
    return 0;
}

void menuPrincipal( FILE* indiceAlunos, FILE* dadosAlunos ) {
    int m = 0;
    for ( ;; ) {
        do {
            printf( "\t### MINI SITEMA ACADEMICO ###\n" );
            printf( "\t1.Alunos\n" );
            printf( "\t2.Disciplinas\n" );
            printf( "\t3.Matriculas\n" );
            printf( "\t4.Sair\n" );
            printf( "Escolha: " );
            scanf( "%d", &m );
            getchar();

            if ( m <= 0 || m > 4 ) {
                printf( "Numero invalido, tente novamente\n" );
            }
        } while( m <= 0 || m > 4 );
    
        switch (m) {
        case 1:
            menuAlunos( indiceAlunos, dadosAlunos );
            break;
        case 2:
            menuDisciplinas();
            break;
        case 3:
            menuMatriculas();
            break;
        case 4:
            return;
            break;
        }
        
    }
}

void menuAlunos( FILE* indice, FILE* dados ) {
    int m;
    
    do {
        printf( "\t### ALUNOS ###\n" );
        printf( "\t1.Adicionar\n" );
        printf( "\t2.Listar\n" );
        printf( "\t3.Atualizar\n" );
        printf( "\t4.Buscar\n" );
        printf( "\t5.Deletar\n" );
        printf( "\t6.Sair\n" );
        printf( "Escolha: " );
        scanf( "%d", &m );
        getchar();
        
        if ( m <= 0 || m > 6 ) {
            printf( "Numero invalido, tente novamente\n" );
        }
    } while( m <= 0 || m > 5 );

    long long raiz = abreIndice( indice );

    switch (m) {
        case 1:{
            addAluno( dados, indice, &raiz );
            fseek(indice, 0, SEEK_SET);
            fwrite(&raiz, sizeof(long long), 1, indice);
        }
            break;
        case 2:{
            if ( raiz == -1 ) {
                printf("Nenhum aluno cadastrado\n");
                return;
            }

            printf( "---- LISTA DE ALUNOS ----\n" );
            imprimePagAluno(indice, dados, raiz);
        }
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:{
            fseek( indice, 0, SEEK_SET );
            fwrite( &raiz, sizeof(long long), 1, indice );
            return;
            break;    
        }
    }
}

void menuDisciplinas() {
    int m;
    
    do {
        printf( "\t### DISCIPLINAS ###\n" );
        printf( "\t1.Adicionar\n" );
        printf( "\t2.Listar\n" );
        printf( "\t3.Atualizar\n" );
        printf( "\t4.Deletar\n" );
        printf( "\t5.Sair\n" );
        printf( "Escolha: " );
        scanf( "%d", &m );
        getchar();
        
        if ( m <= 0 || m > 5 ) {
            printf( "Numero invalido, tente novamente\n" );
        }
    } while( m <= 0 || m > 5 );

    switch (m) {
        case 1:
            
            break;
    
        case 2:
            break;

        case 5:
            return;
            break;    
    }
}

void menuMatriculas() {
    int m;
    
    do {
        printf( "\t### MATRICULAS ###\n" );
        printf( "\t1.Adicionar\n" );
        printf( "\t2.Listar\n" );
        printf( "\t3.Atualizar\n" );
        printf( "\t4.Deletar\n" );
        printf( "\t5.Sair\n" );
        printf( "Escolha: " );
        scanf( "%d", &m );
        getchar();
        
        if ( m <= 0 || m > 5 ) {
            printf( "Numero invalido, tente novamente\n" );
        }
    } while( m <= 0 || m > 5 );

    switch (m) {
        case 1:
            
            break;
    
        case 2:
            break;

        case 5:
            return;
            break;    
    }
}

Pagina* criaPagina( int folha ) {
    Pagina* p = ( Pagina * )malloc( sizeof(Pagina) );
    p->folha = folha;
    p->n = 0;

    for ( int i = 0; i < CHAVES; i++ ) {
        p->id[i] = 0;
        p->pos[i] = -1;
    }

    for ( int i = 0; i < FILHOS; i++ ) {
        //p->filho[i] = NULL;
        p->filhos[i] = -1;
    }

    return p;
}

void dividirPag( long long posiP, long long posiF, int posi, FILE* indice ) {
    Pagina p, f;
    fseek( indice, posiP, SEEK_SET );
    fread( &p, sizeof(Pagina), 1, indice );
    
    fseek( indice, posiF, SEEK_SET );
    fread( &f, sizeof(Pagina), 1, indice );

    int i, j;
    int tempIds[CHAVES + 1];
    long long tempDados[CHAVES + 1];
    long tempFilhos[FILHOS]; 

    for ( i = 0; i < f.n; ++i ) {
        tempIds[i] = f.id[i];
        tempDados[i] = f.pos[i];
    }

    if ( f.folha == 0 ) {
        for ( i = 0; i <= FILHOS; i++ ) {
            tempFilhos[i] = f.filhos[i];
        }
    }

    /*int k = CHAVES;
    while ( k > 0 && id < tempIds[k - 1] ) {
        tempIds[k] = tempIds[k - 1];
        tempDados[k] = tempDados[k - 1];
        k--;// nao acredito q tinha faltado isso
    }
    tempIds[k] = id;
    tempDados[k] = dados;*/

    int mediana = f.n / 2;
    int sobeId = tempIds[mediana];
    long long sobeDados = tempDados[mediana];

    Pagina* novaP = criaPagina( f.folha );
    novaP->n = mediana;

    f.n = mediana;
    for ( i = 0; i < mediana; i++ ) {
        f.id[i] = tempIds[i];
        f.pos[i] = tempDados[i];
        f.filhos[i] = -1;
        if ( f.folha == 0 ) {
            f.filhos[i] = tempFilhos[i];
        }
    }
    for ( i = mediana; i < CHAVES; i++ ) {
        f.id[i] = -1;
        f.pos[i] = -1;
    }
    fseek( indice, posiF, SEEK_SET );
    fwrite( &f, sizeof(Pagina), 1, indice );

    novaP->n = mediana;
    j = mediana + 1;
    for ( i = 0; i < mediana; i++ ) {
        novaP->id[i] = tempIds[j];
        novaP->pos[i] = tempDados[j];
        novaP->filhos[i] = -1;
        if ( novaP->folha == 0 ) {
            novaP->filhos[i] = tempFilhos[j];
        }
        j++; //essa linha pode ta no lugar errado
    }
    for ( ; j < CHAVES; j++ ) {
        novaP->id[i] = -1;
        novaP->pos[i] = -1;
    }
    fseek( indice, 0, SEEK_END );
    long posiNova = ftell(indice);
    fwrite( novaP, sizeof(Pagina), 1, indice );
    free(novaP);
    
    for ( i = p.n - 1; posi <= i; i-- ) {
        p.filhos[i + 1] = p.filhos[i];
        p.id[i + 1] = p.id[i];
        p.pos[i + 1] = p.pos[i];
    }

    p.filhos[posi + 1] = posiNova;
    p.id[posi] = sobeId;
    p.pos[posi] = sobeDados;
    p.n++;

    fseek( indice, posiP, SEEK_SET );
    fwrite( &p, sizeof(Pagina), 1, indice );

}

void addChave_naoCheio( long long posi, int id, long long dados, FILE* indice ) {
    Pagina p;
    fseek( indice, posi, SEEK_SET );
    fread( &p, sizeof(Pagina), 1, indice );

    int i = p.n - 1;
    if ( p.folha == 1 ) { 
        while ( i >= 0 && id < p.id[i] ) {
            p.id[i + 1] = p.id[i];
            p.pos[i + 1] = p.pos[i];
            i--;
        }

        i++;
        p.id[i] = id;
        p.pos[i] = dados;
        p.n++;
        fseek( indice, posi, SEEK_SET );
        fwrite( &p, sizeof(Pagina), 1, indice );

    } else {
        
        while ( i >= 0 && id < p.id[i] ) {
            i--;
        }
        int indiceFilho = i + 1;

        Pagina f;
        fseek( indice, p.filhos[indiceFilho], SEEK_SET );
        fread( &f, sizeof(Pagina), 1, indice );

        if ( f.n == CHAVES ) {
            dividirPag( posi, p.filhos[indiceFilho], indiceFilho, indice );
            fseek( indice, posi, SEEK_SET ); //CLARO Q N IA FUNCIONAR EU N TAVA ATUALIZANDO O PAI
            fread( &p, sizeof(Pagina), 1, indice );

            if ( id > p.id[indiceFilho] ) {
                indiceFilho++;
            }
        }
        addChave_naoCheio( p.filhos[indiceFilho], id, dados, indice );
    }  
}

long long inserir( FILE* indice, long long posiRaiz, int id, long long dados ) {
    Pagina root;
    fseek( indice, posiRaiz, SEEK_SET );
    fread( &root, sizeof(Pagina), 1, indice );

    if ( root.n == CHAVES ) {
        Pagina* novaR = criaPagina(0);
        novaR->filhos[0] = posiRaiz;

        fseek( indice, 0, SEEK_END );
        long posiNova = ftell(indice);
        fwrite( novaR, sizeof(Pagina), 1, indice );
        free(novaR);

        dividirPag( posiNova, posiRaiz, 0, id, dados, indice );

        Pagina pag;
        fseek( indice, posiNova, SEEK_SET );
        fread( &pag, sizeof(Pagina), 1, indice );
        
        int dest;
        if (id > pag.id[0]) {
            dest = 1;   //direita
        } else {
            dest = 0;   //esquerda
        }
        addChave_naoCheio( pag.filhos[dest], id, dados, indice );
        fseek( indice, 0, SEEK_SET );
        fwrite( &posiNova, sizeof(long long), 1, indice );

        return posiNova;
    }

    addChave_naoCheio( posiRaiz, id, dados, indice );
    fseek( indice, 0, SEEK_SET );
    fwrite( &posiRaiz, sizeof(long long), 1, indice );

    printf("DEBUG: inserir() terminou. root=%lld\n", posiRaiz);
    return posiRaiz;
}

void addAluno( FILE* arquivo, FILE* indice, long long* root ) { 
    if ( !arquivo ) { 
        printf("Nao abriu"); 
        return; 
    }

    Aluno a;
    printf( "Matricula: " );
    scanf( "%d", &a.matricula );
    printf( "Nome: " );
    getchar();
    fgets( a.nome, 100, stdin );
    a.nome[strcspn(a.nome, "\n")] = '\0';

    fseek( arquivo, 0, SEEK_END ); 
    long long pos = ftell( arquivo );
    printf("DEBUG: aluno salvo em pos = %lld (mat=%d)\n", pos, a.matricula);
    fwrite( &a, sizeof(Aluno), 1, arquivo );

    *root = inserir( indice, *root, a.matricula, pos );
    printf("DEBUG: root atualizada para %lld\n", *root);
    fseek(indice, 0, SEEK_SET);
    fwrite(root, sizeof(long long), 1, indice);

    printf( "Aluno inserido\n" );
}

long long abreIndice( FILE* indice ) {
    fseek( indice, 0, SEEK_END );
    long long tam = ftell( indice );
    
    if ( tam >= sizeof(long long) ) {
        long long root;
        fseek(indice, 0, SEEK_SET);
        fread( &root, sizeof(long long), 1, indice );

        if (root < sizeof(long long) || root >= tam) {
            printf("Indice corrompido, recriando...\n");
        } else {
            return root;
        }
    }

    Pagina* raiz = criaPagina(1);
    long long pos;
    fseek( indice, 0, SEEK_SET );
    long long primeiro = -1;
    fwrite( &primeiro, sizeof(long long), 1, indice );

    pos = ftell( indice );
    fwrite( raiz, sizeof(Pagina), 1, indice );
    fseek( indice, 0, SEEK_SET );
    fwrite( &pos, sizeof(long long), 1, indice );// é pra manter a posição da raiz no topo, vamo ver se funciona

    free(raiz);
    return pos;
}

void imprimeAluno( FILE *dados, long long pos ) {
    if (pos < 0) return;

    Aluno a;

    fseek(dados, pos, SEEK_SET);
    fread(&a, sizeof(Aluno), 1, dados);

    printf("Matricula: %d | Nome: %s\n", a.matricula, a.nome);
}

void imprimePagAluno( FILE *indice, FILE *dados, long long posPag ) {
    if (posPag < 0) return;

    Pagina pag;
    fseek(indice, posPag, SEEK_SET);
    fread(&pag, sizeof(Pagina), 1, indice);

    int i;
    for ( i = 0; i < pag.n; i++ ) {

        if (pag.filhos[i] >= 0){
            imprimePagAluno( indice, dados, pag.filhos[i] );
        }
        imprimeAluno( dados, pag.pos[i] );
    }

    if ( pag.filhos[i] >= 0 )
        imprimePagAluno( indice, dados, pag.filhos[i] );
}
