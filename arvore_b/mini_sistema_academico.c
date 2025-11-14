#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHAVES 4
#define MIN_CHAVES ( CHAVES/2 )
#define FILHOS ( CHAVES + 1 )

typedef struct {
    int id;
    long pos;
}Index;

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
    Pagina* filho[FILHOS];
    //long long filhos[FILHOS]; //posição dos filhos
}Pagina;

typedef struct {
    long long raiz;
    long long prox;
}Cabecalho;

void menuPrincipal();
void menuAlunos();
void menuDisciplinas();
void menuMatriculas();
void addChave_naoCheio( Pagina* p, int id, long long dados );
void addAluno();


int main() {
    menuPrincipal();
    
    return 0;
}

void menuPrincipal() {
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
            menuAlunos();
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

void menuAlunos() {
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

    switch (m) {
        case 1:
            addAluno();
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            return;
            break;    
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
        p->filho[i] = NULL;
    }

    return p;
}

void dividirPag( Pagina* p, Pagina* f, int posi ){
    int j = 0;
    Pagina* novaP = criaPagina( f->folha );
    novaP->n = MIN_CHAVES; 

    for ( int i = MIN_CHAVES; i < CHAVES; i++ ) {
        novaP->id[j] = f->id[i];
        novaP->pos[j] = f->pos[i];
        remover(i);

        if ( f->folha == 0 ) {
            novaP->filho[j] = f->filho[i];
        }
        j++;
    }
    f->n = MIN_CHAVES;
    
    for( int i = p->n - 1; posi <= i; i-- ) {
        p->filho[i + 1] = p->filho[i];
        p->id[i + 1] = p->id[i];
        p->pos[i + 1] = p->pos[i];
    }
    p->filho[posi + 1] = novaP;
    
    p->id[]



}

void addChave_naoCheio( Pagina* p, int id, long long dados ) {
    int i = p->n - 1;
    if ( p->folha == 1 ) { 
        while ( i != -1 && id < p->id[i] ) {
            p->id[i + 1] = p->id[i];
            p->pos[i + 1] = p->pos[i];
            i--;
        }

        i++;
        p->id[i] = id;
        p->pos[i] = dados;
        p->n++;

    } else {
        while ( i != -1 && id < p->id[i] ) {
            i--;
        }

        if ( p->filho[i]->n == CHAVES ) {
            dividirPag( p, p->filho[i], i );
        }

        
    }

}

void inserir() {

}

void addAluno() { 
    FILE *arquivo = fopen("alunos.data", "ab");
    if ( !arquivo ) { 
        printf("erro"); 
        return; 
    }

    Aluno a;
    printf( "Matricula: " );
    scanf( "%d", &a.matricula );
    printf( "Nome: " );
    getchar();
    fgets( a.nome, 100, stdin );
    a.nome[strcspn(a.nome, "\n")] = '\0';

    long long pos = ftell(arquivo);
    fwrite( &a, sizeof(Aluno), 1, arquivo );
    fclose(arquivo);

    addChave( a.matricula, pos );

    printf( "Aluno inserido\n" );
}

/*

//if ( id > p->id[i] ) {
           // i++;
        //}

        //addChave_naoCheio( p->filho[i], id, dados );


if ( f->id[MIN_CHAVES] < id && id < novaP->id[0] ) {
        p->n++;
        p->id[p->n - 1] = id;
        p->pos[p->n - 1] = dados;

    } else if ( f->id[MIN_CHAVES] < novaP->id[0] && novaP->id[0] < id ) {       
        p->n++;
        p->id[p->n - 1] = novaP->id[0];
        p->pos[p->n - 1] = novaP->pos[0];

    } else if ( id < f->id[MIN_CHAVES] && f->id[0] < novaP->id[0] ) {
        p->n++;
        p->id[p->n - 1] = f->id[0];
        p->pos[p->n - 1] = f->pos[0];
    }

*/