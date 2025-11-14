#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHAVES 4
#define MIN_CHAVES ( CHAVES/2 )
#define FILHOS ( CHAVES + 1 )

typedef struct {
    int matricula; //Chave Primária
    char nome[100];
    int removido;
}Aluno;

typedef struct {
    int id;
    long pos;
    char removido;
}IndexAluno;

typedef struct {
    int codigo; //Chave Primária
    char nome[100];
    int removido;
}Disciplinas;

typedef struct {
    int id; //Chave Primária
    int matricula_aluno; //Chave Estrangeira
    int codigo_disciplina; //Chave Estrangeira
    float media;
    int removido;
}Matriculas;

typedef struct {
    int n; //numero de chaves
    int folha; //0 = nó interno || 1 = folha
    int ids[CHAVES];
    int removido[CHAVES]; //0 = não removido || 1 = removido
    long long p[CHAVES]; //posição das chaves
    long long filhos[FILHOS]; //posição dos filhos
}Pagina;

typedef struct {
    long long raiz;
    long long prox;
}Cabecalho;

void menuPrincipal();
void menuAlunos();
void menuDisciplinas();
void menuMatriculas();
void adicionar_indice(int id, long pos);
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

void adicionar_indice(int id, long pos) {
    FILE *fp = fopen("alunos.idx", "ab");
    IndexAluno idx = { id, pos, 0 };
    fwrite(&idx, sizeof(IndexAluno), 1, fp);
    fclose(fp);
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
    a.removido = 0;

    long pos = ftell(arquivo);
    fwrite( &a, sizeof(Aluno), 1, arquivo );
    fclose(arquivo);

    adicionar_indice(a.matricula, pos);

    printf( "Aluno inserido com sucesso!\n" );
}

/*
static long long tam_arquivo(FILE *arquivo) {
    fseek(arquivo, 0, SEEK_END);
    long long fim = ftell(arquivo);
    return fim;
}
*/