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

void menuPrincipal( FILE* indiceAlunos, FILE* dadosAlunos, FILE* indiceDisciplinas , FILE* dadosDisciplinas, FILE* indiceMatriculas, FILE* dadosMatriculas );
void menuAlunos( FILE* indice, FILE* dados, FILE* indiceM , FILE* dadosM );
void menuDisciplinas( FILE* indice, FILE* dados, FILE* indiceM, FILE* dadosM );
void menuMatriculas( FILE* indiceM, FILE* dadosM, FILE* indiceA , FILE* indiceD );
Pagina* criaPagina( int folha );
long long abreIndice( FILE* indice );
void dividirPag( long long posiP, long long posiF, int posi, int id, long long dados, FILE* indice );
void addChave_naoCheio( long long posi, int id, long long dados, FILE* indice );
long long inserir( FILE* indice, long long posiRaiz, int id, long long dados );
long long buscar( long long posi, int id, FILE* indice );
void addAluno( FILE* arquivo, FILE* indice, long long* root );
void imprimeAluno(FILE *dados, long long pos);
void imprimePagAluno(FILE *indice, FILE *dados, long long posPag);
void atualizarAluno( long long root, FILE* indice, FILE* dados );
void addDisciplina( FILE* arquivo, FILE* indice, long long* root );
void imprimeDisciplina( FILE *dados, long long pos );
void imprimePagDisciplina( FILE *indice, FILE *dados, long long posPag );
void addMatricula( FILE* arquivoM, FILE* indiceM, FILE* indiceA, FILE* indiceD, long long* root );
void atualizarDisciplina( long long root, FILE* indice, FILE* dados );
void imprimeMatricula( FILE *dados, long long pos );
void imprimePagMatricula( FILE *indice, FILE *dados, long long posPag );
void atualizarMedia( long long root, FILE* indice, FILE* dados );
void removerChaveDePagina( Pagina* p, int posi );
void removerFilhoDePagina( Pagina* p, int posi );
long long buscarAntecessor( FILE* indice, long long posi, int* id, long long* dados );
long long buscarSucessor( FILE* indice, long long posi, int* id, long long* dados );
void mergePaginas( FILE* indice, long long posiP, int idxEsq, int idxDir );
void pegarEmprestadoDir( FILE* indice, long long posiP, int idxEsq, int idxDir );
void pegarEmprestadoEsq( FILE* indice, long long posiPai, int idxEsq, int idxDir );
int removerRecursivo( FILE* indice, long long posiP, int id );
void removerChave( FILE* indice, long long* raiz, int id );
void removerAluno( FILE* indiceA, FILE* indiceM, FILE* dadosM, long long* posiA, long long* posiM );
void removerDisciplina( FILE* indiceD, FILE* indiceM, FILE* dadosM, long long* posiD, long long* posiM );
void removerMatricula( FILE* indice, long long* posi );

int main() {
    FILE *indiceAlunos = fopen("alunos.idx", "r+b");
    if (!indiceAlunos) {
        indiceAlunos = fopen("alunos.idx", "w+b");
    }

    FILE *dadosAlunos = fopen("alunos.data", "r+b");
    if (!dadosAlunos) {
        dadosAlunos = fopen("alunos.data", "w+b");
    }

    FILE *indiceDisciplinas = fopen("disciplinas.idx", "r+b");
    if (!indiceDisciplinas) {
        indiceDisciplinas = fopen("disciplinas.idx", "w+b");
    }

    FILE *dadosDisciplinas = fopen("disciplinas.data", "r+b");
    if (!dadosDisciplinas) {
        dadosDisciplinas = fopen("disciplinas.data", "w+b");
    }

    FILE *indiceMatriculas = fopen("matriculas.idx", "r+b");
    if (!indiceMatriculas) {
        indiceMatriculas = fopen("matriculas.idx", "w+b");
    }

    FILE *dadosMatriculas = fopen("matriculas.data", "r+b");
    if (!dadosMatriculas) {
        dadosMatriculas = fopen("matriculas.data", "w+b");
    }
    
    menuPrincipal( indiceAlunos, dadosAlunos, indiceDisciplinas, dadosDisciplinas, indiceMatriculas, dadosMatriculas );

    fclose(indiceAlunos);
    fclose(dadosAlunos);
    fclose(indiceDisciplinas);
    fclose(dadosDisciplinas);
    fclose(indiceMatriculas);
    fclose(dadosMatriculas);
    
    return 0;
}

void menuPrincipal( FILE* indiceAlunos, FILE* dadosAlunos, FILE* indiceDisciplinas , FILE* dadosDisciplinas, FILE* indiceMatriculas, FILE* dadosMatriculas ) {
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
            menuAlunos( indiceAlunos, dadosAlunos, indiceMatriculas, dadosMatriculas );
            break;
        case 2:
            menuDisciplinas( indiceDisciplinas, dadosDisciplinas, indiceMatriculas, dadosMatriculas );
            break;
        case 3:
            menuMatriculas( indiceMatriculas, dadosMatriculas, indiceAlunos, indiceDisciplinas );
            break;
        case 4:
            return;
            break;
        }
        
    }
}

void menuAlunos( FILE* indice, FILE* dados, FILE* indiceM , FILE* dadosM ) {
    int m;
    
    do {
        printf( "\t### ALUNOS ###\n" );
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
    } while( m <= 0 || m > 6 );

    long long raiz = abreIndice( indice );

    switch (m) {
        case 1:{
            addAluno( dados, indice, &raiz );
            fseek(indice, 0, SEEK_SET);
            fwrite(&raiz, sizeof(long long), 1, indice);
            break;
        }
        case 2:{
            if ( raiz == -1 ) {
                printf("Nenhum aluno cadastrado\n");
                return;
            }

            printf( "### LISTA DE ALUNOS ###\n" );
            imprimePagAluno( indice, dados, raiz );
            break;
        }
        case 3:{
            atualizarAluno( raiz, indice, dados ); 
            fseek( indice, 0, SEEK_SET );
            fwrite( &raiz, sizeof(long long), 1, indice );
            break;
        }
        case 4: {
            long long raizM = abreIndice( indiceM );
            removerAluno( indice, indiceM, dadosM, &raiz, &raizM );
            fseek( indice, 0, SEEK_SET );
            fwrite( &raiz, sizeof(long long), 1, indice );
            fseek( indiceM, 0, SEEK_SET );
            fwrite( &raizM, sizeof(long long), 1, indiceM );//esqueci de trocar indice por indiceM e ME FERREI TOTAL a msm coisa nas disciplinas 
            break;
        }
        case 5:{
            fseek( indice, 0, SEEK_SET );
            fwrite( &raiz, sizeof(long long), 1, indice );
            return;
            break;    
        }
    }
}

void menuDisciplinas( FILE* indice, FILE* dados, FILE* indiceM, FILE* dadosM ) {
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

    long long raiz = abreIndice( indice );

    switch (m) {
        case 1:{
            addDisciplina( dados, indice, &raiz );
            fseek( indice, 0, SEEK_SET );
            fwrite( &raiz, sizeof(long long), 1, indice );
            break;
        }
        case 2:{
            if ( raiz == -1 ) {
                printf( "Nenhuma disciplina cadastrada\n" );
                return;
            }

            printf( "### LISTA DE DISCIPLIANS ###\n" );
            imprimePagDisciplina( indice, dados, raiz );
            break;
        }
        case 4: {
            long long raizM = abreIndice( indiceM );
            removerDisciplina( indice, indiceM, dadosM, &raiz, &raizM );
            fseek( indice, 0, SEEK_SET );
            fwrite( &raiz, sizeof(long long), 1, indice );
            fseek( indiceM, 0, SEEK_SET );
            fwrite( &raizM, sizeof(long long), 1, indiceM );
            break;
        }
        case 5:{
            fseek( indice, 0, SEEK_SET );
            fwrite( &raiz, sizeof(long long), 1, indice );
            return;
            break;   
        } 
    }
}

void menuMatriculas( FILE* indiceM, FILE* dadosM, FILE* indiceA , FILE* indiceD ) {
    int m;
    
    do {
        printf( "\t### MATRICULAS ###\n" );
        printf( "\t1.Adicionar\n" );
        printf( "\t2.Listar\n" );
        printf( "\t3.Atualizar Media\n" );
        printf( "\t4.Deletar\n" );
        printf( "\t5.Sair\n" );
        printf( "Escolha: " );
        scanf( "%d", &m );
        getchar();
        
        if ( m <= 0 || m > 5 ) {
            printf( "Numero invalido, tente novamente\n" );
        }
    } while( m <= 0 || m > 5 );

    long long raiz = abreIndice( indiceM );

    switch (m) {
        case 1:{
            addMatricula( dadosM, indiceM, indiceA, indiceD, &raiz );
            fseek( indiceM, 0, SEEK_SET );
            fwrite( &raiz, sizeof(long long), 1, indiceM );
            break;
        }
        case 2:{
            if ( raiz == -1 ) {
                printf("Nenhuma matricula cadastrada\n");
                return;
            }

            printf( "### LISTA DE MATRICULAS ###\n" );
            imprimePagMatricula( indiceM, dadosM, raiz );
            break;
        }
        case 3:{
            atualizarMedia( raiz, indiceM, dadosM );
            fseek( indiceM, 0, SEEK_SET );
            fwrite( &raiz, sizeof(long long), 1, indiceM );
            break;
        }
        case 4:{
            removerMatricula(indiceM, &raiz);
            fseek( indiceM, 0, SEEK_SET );
            fwrite( &raiz, sizeof(long long), 1, indiceM );
            break;
        }
        case 5:{
            fseek( indiceM, 0, SEEK_SET );
            fwrite( &raiz, sizeof(long long), 1, indiceM );
            return;
            break;
        }    
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

void dividirPag( long long posiP, long long posiF, int posi, int id, long long dados, FILE* indice ) {
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

    int k = CHAVES;
    while ( k > 0 && id < tempIds[k - 1] ) {
        tempIds[k] = tempIds[k - 1];
        tempDados[k] = tempDados[k - 1];
        k--;// nao acredito q tinha faltado isso
    }
    tempIds[k] = id;
    tempDados[k] = dados;

    int mediana = ( CHAVES + 1 ) / 2;
    int sobeId = tempIds[mediana];
    long long sobeDados = tempDados[mediana];

    Pagina* novaP = criaPagina( f.folha );
    novaP->n = 0;

    for ( i = mediana + 1; i < CHAVES + 1; i++ ) { //o mediana + 1 me deu um trabalho
        novaP->id[novaP->n] = tempIds[i];
        novaP->pos[novaP->n] = tempDados[i];
        novaP->n++;
    }
    if ( novaP->folha == 0 ) {
        for ( i = 0; i < FILHOS; i++ ) {
            novaP->filhos[novaP->n] = tempFilhos[i];
        }   
    }

    f.n = mediana;
    for ( i = 0; i < mediana; i++ ) {
        f.id[i] = tempIds[i];
        f.pos[i] = tempDados[i]; 
    }
    if ( f.folha == 0 ) {
        for ( i = 0; i < FILHOS; i++ ) {
            f.filhos[i] = tempFilhos[i];
        }
    }
    for ( i = mediana; i < CHAVES; i++ ) {
        f.id[i] = 0;
        f.pos[i] = -1;
    }
    
    fseek( indice, 0, SEEK_END );
    long long posiNova = ftell(indice);
    fwrite( novaP, sizeof(Pagina), 1, indice );
    free(novaP);
    
    for ( i = p.n; posi < i; i-- ) {
        p.filhos[i + 1] = p.filhos[i];
        p.id[i] = p.id[i - 1];
        p.pos[i] = p.pos[i - 1];
    }

    p.filhos[posi + 1] = posiNova;
    p.id[posi] = sobeId;
    p.pos[posi] = sobeDados;
    p.n++;

    fseek( indice, posiP, SEEK_SET );
    fwrite( &p, sizeof(Pagina), 1, indice );
    fseek( indice, posiF, SEEK_SET );
    fwrite( &f, sizeof(Pagina), 1, indice );

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
            dividirPag( posi, p.filhos[indiceFilho], indiceFilho, id, dados, indice );
            /*fseek( indice, posi, SEEK_SET ); 
            fread( &p, sizeof(Pagina), 1, indice );

            if ( id > p.id[indiceFilho] ) {
                indiceFilho++;
            }*/
        } else { 
            addChave_naoCheio( p.filhos[indiceFilho], id, dados, indice );
        }
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
        long long posiNova = ftell(indice);
        fwrite( novaR, sizeof(Pagina), 1, indice );
        free(novaR);

        dividirPag( posiNova, posiRaiz, 0, id, dados, indice );

        return posiNova;
    }

    addChave_naoCheio( posiRaiz, id, dados, indice );
    fseek( indice, 0, SEEK_SET );
    fwrite( &posiRaiz, sizeof(long long), 1, indice );

    return posiRaiz;
}

long long buscar( long long posi, int id, FILE* indice ) {
    if( posi == -1 ) {
        return -1;
    }

    int i = 0;
    Pagina p;
    fseek( indice, posi, SEEK_SET );
    fread( &p, sizeof(Pagina), 1, indice );

    while ( i < p.n && id > p.id[i] ) {
        i++;
    }

    if ( i < p.n && p.id[i] == id ) {
        return p.pos[i];
    }

    if ( p.folha == -1 ){
        return -1;
    }

    return buscar( p.filhos[i], id, indice );
}

long long abreIndice( FILE* indice ) {
    fseek( indice, 0, SEEK_END );
    long long tam = ftell( indice );
    
    if ( tam >= (long long)sizeof(long long) ) {
        long long root;
        fseek(indice, 0, SEEK_SET);
        fread( &root, sizeof(long long), 1, indice );

        if (root < (long long)sizeof(long long) || root >= tam) {
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

void addAluno( FILE* arquivo, FILE* indice, long long* root ) { 
    if ( !arquivo ) { 
        printf("Nao abriu"); 
        return; 
    }

    int validacao1;
    long long validacao2;
    Aluno a;

    do {
        printf( "Matricula: " );
        validacao1 = scanf( "%d", &a.matricula );

        if ( a.matricula <= 0 || validacao1 != 1 ) {
            printf( "Matricula invalida, tente novamente\n" );
            while (getchar() != '\n');
        }

        validacao2 = buscar( *root, a.matricula, indice );
        if ( validacao2 != -1) {
            printf( "Matricula ja existente, tente novamente\n" );
            while (getchar() != '\n');
        }

    }while( a.matricula <= 0 || validacao1 != 1 || validacao2 != -1 );


    printf( "Nome: " );
    getchar();
    fgets( a.nome, 100, stdin );
    a.nome[strcspn(a.nome, "\n")] = '\0';

    fseek( arquivo, 0, SEEK_END ); 
    long long pos = ftell( arquivo );
    fwrite( &a, sizeof(Aluno), 1, arquivo );

    *root = inserir( indice, *root, a.matricula, pos );
    fseek(indice, 0, SEEK_SET);
    fwrite(root, sizeof(long long), 1, indice);

    printf( "Aluno inserido\n" );
}

void imprimeAluno( FILE *dados, long long pos ) {
    if (pos < 0) return;

    Aluno a;

    fseek(dados, pos, SEEK_SET);
    fread(&a, sizeof(Aluno), 1, dados);

    printf("Matricula: %d | Nome: %s\n", a.matricula, a.nome);
}

void imprimePagAluno( FILE *indice, FILE *dados, long long posPag ) {
    if (posPag < 0) {
        return;
    }

    Pagina pag;
    fseek( indice, posPag, SEEK_SET );
    fread( &pag, sizeof(Pagina), 1, indice );

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

void atualizarAluno( long long root, FILE* indice, FILE* dados ) {
    int id, validacao1;
    long long posi;

    do {
        printf("Matricula: ");
        validacao1 = scanf("%d", &id);
        if ( id <= 0 || validacao1 != 1 ) {
            printf( "Matricula invalida, tente novamente\n" );
            while (getchar() != '\n');
        }

        posi = buscar( root, id, indice );
        if ( posi == -1 ) {
            printf( "Matricula nao existente, tente novamente\n" );
        }
    }while( id <= 0 || validacao1 != 1 || posi == -1 );

    Aluno a;
    fseek( dados, posi, SEEK_SET );
    fread( &a, sizeof(Aluno), 1, dados );

    printf("Novo nome: ");
    getchar();
    fgets( a.nome, 100, stdin );
    a.nome[strcspn(a.nome, "\n")] = '\0';

    fseek( dados, posi, SEEK_SET );
    fwrite( &a, sizeof(Aluno), 1, dados );
}

void addDisciplina( FILE* arquivo, FILE* indice, long long* root ) { 
    if ( !arquivo ) { 
        printf("Nao abriu"); 
        return; 
    }

    int validacao1;
    long long validacao2;
    Disciplinas d;
    do {
        printf( "Codigo da disciplina: " );
        validacao1 = scanf( "%d", &d.codigo );

        if ( d.codigo <= 0 || validacao1 != 1 ) {
            printf( "Codigo invalido, tente novamente\n" );
            while (getchar() != '\n');
        }

        validacao2 = buscar( *root, d.codigo, indice );
        if ( validacao2 != -1 ) {
            printf( "Codico de disciplina ja existente, tente novamente\n" );
        }
    }while( d.codigo <= 0 || validacao1 != 1 || validacao2 != -1 );

    printf( "Nome: " );
    getchar();
    fgets( d.nome, 100, stdin );
    d.nome[strcspn(d.nome, "\n")] = '\0';

    fseek( arquivo, 0, SEEK_END ); 
    long long pos = ftell( arquivo );
    fwrite( &d, sizeof(Disciplinas), 1, arquivo );

    *root = inserir( indice, *root, d.codigo, pos );
    fseek(indice, 0, SEEK_SET);
    fwrite(root, sizeof(long long), 1, indice);

    printf( "Disciplina inserido\n" );
}

void imprimeDisciplina( FILE *dados, long long pos ) {
    if (pos < 0) return;

    Disciplinas d;

    fseek( dados, pos, SEEK_SET );
    fread( &d, sizeof(Disciplinas), 1, dados );

    printf( "Codigo: %d | Nome: %s\n", d.codigo, d.nome );
}

void imprimePagDisciplina( FILE *indice, FILE *dados, long long posPag ) {
    if (posPag < 0) {
        return;
    }

    Pagina pag;
    fseek( indice, posPag, SEEK_SET );
    fread( &pag, sizeof(Pagina), 1, indice );

    int i;
    for ( i = 0; i < pag.n; i++ ) {

        if (pag.filhos[i] >= 0){
            imprimePagDisciplina( indice, dados, pag.filhos[i] );
        }
        imprimeDisciplina( dados, pag.pos[i] );
    }

    if ( pag.filhos[i] >= 0 )
        imprimePagDisciplina( indice, dados, pag.filhos[i] );
}

void atualizarDisciplina( long long root, FILE* indice, FILE* dados ) {
    int id, validacao1;
    long long posi;

    do {
        printf("Codigo: ");
        validacao1 = scanf("%d", &id);
        if ( id <= 0 || validacao1 != 1 ) {
            printf( "Codigo invalido, tente novamente\n" );
            while (getchar() != '\n');
        }

        posi = buscar( root, id, indice );
        if ( posi == -1 ) {
            printf( "Codigo nao existente, tente novamente\n" );
        }
    }while( id <= 0 || validacao1 != 1 || posi == -1 );

    Disciplinas d;
    fseek( dados, posi, SEEK_SET );
    fread( &d, sizeof(Disciplinas), 1, dados );

    printf("Novo nome: ");
    getchar();
    fgets( d.nome, 100, stdin );
    d.nome[strcspn(d.nome, "\n")] = '\0';

    fseek( dados, posi, SEEK_SET );
    fwrite( &d, sizeof(Disciplinas), 1, dados );
}

void addMatricula( FILE* arquivoM, FILE* indiceM, FILE* indiceA, FILE* indiceD, long long* root ) { 
    if ( !arquivoM ) { 
        printf("Nao abriu"); 
        return; 
    }

    int validacao1;
    long long validacao2;
    Matriculas m;
    do {
        printf( "ID: " );
        validacao1 = scanf( "%d", &m.id );

        if ( m.id <= 0 || validacao1 != 1 ) {
            printf( "ID invalido, tente novamente\n" );
            while (getchar() != '\n');
        }

        
        validacao2 = buscar( *root, m.id, indiceM );
        if ( validacao2 != -1 ) {
            printf( "ID ja existente, tente novamente\n" );
        }
    }while( m.id <= 0 || validacao1 != 1 || validacao2 != -1 );

    do {
        printf( "Numero de matricula do aluno: " );
        validacao1 = scanf( "%d", &m.matricula_aluno );

        if ( m.matricula_aluno <= 0 || validacao1 != 1 ) {
            printf( "Numero de matricula invalido, tente novamente\n" );
            while (getchar() != '\n');
        }

        long long raizA = abreIndice(indiceA);
        validacao2 = buscar( raizA, m.matricula_aluno, indiceA );
        if ( validacao2 == -1 ) {
            printf( "Numero de matricula nao existente, tente novamente\n" );
        }
    }while( m.matricula_aluno <= 0 || validacao1 != 1 || validacao2 == -1 );

    do {
        printf( "Codigo da disciplina: " );
        validacao1 = scanf( "%d", &m.codigo_disciplina );

        if ( m.codigo_disciplina <= 0 || validacao1 != 1 ) {
            printf( "Codigo invalido, tente novamente\n" );
            while (getchar() != '\n');
        }

        long long raizD = abreIndice( indiceD );
        validacao2 = buscar( raizD, m.codigo_disciplina, indiceD );
        if ( validacao2 == -1 ) {
            printf( "Codigo nao existente, tente novamente\n" );
        }
    }while( m.codigo_disciplina <= 0 || validacao1 != 1 || validacao2 == -1 );

    m.media = 0;

    fseek( arquivoM, 0, SEEK_END ); 
    long long pos = ftell( arquivoM );
    fwrite( &m, sizeof(Matriculas), 1, arquivoM );

    *root = inserir( indiceM, *root, m.id, pos );
    fseek(indiceM, 0, SEEK_SET);
    fwrite(root, sizeof(long long), 1, indiceM);

    printf( "Matricula inserida\n" );
}

void imprimeMatricula( FILE *dados, long long pos ) {
    if (pos < 0) return;

    Matriculas m;

    fseek( dados, pos, SEEK_SET );
    fread( &m, sizeof(Matriculas), 1, dados );

    printf( "ID: %d | Matricula do aluno: %d | Codigo da disciplina: %d | Media: %.2f\n", m.id, m.matricula_aluno, m.codigo_disciplina, m.media );
}

void imprimePagMatricula( FILE *indice, FILE *dados, long long posPag ) {
    if (posPag < 0) {
        return;
    }

    Pagina pag;
    fseek( indice, posPag, SEEK_SET );
    fread( &pag, sizeof(Pagina), 1, indice );

    int i;
    for ( i = 0; i < pag.n; i++ ) {

        if (pag.filhos[i] >= 0){
            imprimePagMatricula( indice, dados, pag.filhos[i] );
        }
        imprimeMatricula( dados, pag.pos[i] );
    }

    if ( pag.filhos[i] >= 0 )
        imprimePagMatricula( indice, dados, pag.filhos[i] );
}

void atualizarMedia( long long root, FILE* indice, FILE* dados ) {
    int id, validacao1;
    long long posi;

    do {
        printf("ID da matricula: ");
        validacao1 = scanf("%d", &id);
        if ( id <= 0 || validacao1 != 1 ) {
            printf( "ID invalido, tente novamente\n" );
            while (getchar() != '\n');
        }

        posi = buscar( root, id, indice );
        if ( posi == -1 ) {
            printf( "ID nao existente, tente novamente\n" );
        }
    }while( id <= 0 || validacao1 != 1 || posi == -1 );

    Matriculas m;
    fseek( dados, posi, SEEK_SET );
    fread( &m, sizeof(Matriculas), 1, dados );

    do {
        printf("Media: ");
        validacao1 = scanf("%f", &m.media);
        if ( m.media <= 0 || m.media > 10 || validacao1 != 1 ) {
            printf( "Media invalida, tente novamente\n" );
            while (getchar() != '\n');
        }
    }while( m.media <= 0 || m.media > 10 || validacao1 != 1 );

    fseek( dados, posi, SEEK_SET );
    fwrite( &m, sizeof(Matriculas), 1, dados );
    
}

void removerChaveDePagina( Pagina* p, int posi ) {
    for ( int i = posi; i < p->n - 1; i++ ) {
        p->id[i] = p->id[i+1];
        p->pos[i] = p->pos[i+1];
    }
    p->n--;
}

void removerFilhoDePagina( Pagina* p, int posi ) {
    for ( int i = posi; i < p->n + 1; i++ ) {
        p->filhos[i] = p->filhos[i+1];
    }
}

long long buscarAntecessor( FILE* indice, long long posi, int* id, long long* dados ) {
    Pagina p;
    fseek( indice, posi, SEEK_SET );
    fread( &p, sizeof(Pagina), 1, indice );
    
    if ( p.folha == 1 ) {
        *id = p.id[p.n - 1];
        *dados = p.pos[p.n - 1];
        return posi;
    }
    return buscarAntecessor( indice, p.filhos[p.n], id, dados );
}

long long buscarSucessor( FILE* indice, long long posi, int* id, long long* dados ) {
    Pagina p;
    fseek( indice, posi, SEEK_SET );
    fread( &p, sizeof(Pagina), 1, indice );
    
    if ( p.folha == 1 ) {
        *id = p.id[0];
        *dados = p.pos[0];
        return posi;
    }
    return buscarSucessor( indice, p.filhos[0], id, dados );
}

void mergePaginas( FILE* indice, long long posiP, int idxEsq, int idxDir ) {
    Pagina p, esq, dir;
    fseek( indice, posiP, SEEK_SET ); 
    fread( &p, sizeof(Pagina), 1, indice );

    long long posiEsq = p.filhos[idxEsq];
    long long posiDir = p.filhos[idxDir];
    
    fseek( indice, posiEsq, SEEK_SET ); 
    fread( &esq, sizeof(Pagina), 1, indice );
    fseek( indice, posiDir, SEEK_SET ); 
    fread( &dir, sizeof(Pagina), 1, indice );
    
    esq.id[esq.n] = p.id[idxEsq];
    esq.pos[esq.n] = p.pos[idxEsq];
    esq.n++;
    
    for ( int i = 0; i < dir.n; i++ ) {
        esq.id[esq.n] = dir.id[i];
        esq.pos[esq.n] = dir.pos[i];
        if (!esq.folha) esq.filhos[esq.n] = dir.filhos[i]; 
        esq.n++;
    }
    if ( esq.folha == 0 ) {
        esq.filhos[esq.n] = dir.filhos[dir.n];
    }
    
    for ( int i = idxEsq; i < p.n - 1; i++ ) {
        p.id[i] = p.id[i + 1];
        p.pos[i] = p.pos[i + 1];
        p.filhos[i + 1] = p.filhos[i + 2];
    }
    p.n--;
    
    fseek( indice, posiP, SEEK_SET ); 
    fwrite( &p, sizeof(Pagina), 1, indice );
    fseek( indice, posiEsq, SEEK_SET ); 
    fwrite( &esq, sizeof(Pagina), 1, indice );

}

void pegarEmprestadoDir( FILE* indice, long long posiP, int idxEsq, int idxDir ) {
    Pagina p, esq, dir;
    fseek( indice, posiP, SEEK_SET ); 
    fread( &p, sizeof(Pagina), 1, indice );

    long long offEsq = p.filhos[idxEsq];
    long long offDir = p.filhos[idxDir];
    fseek( indice, offEsq, SEEK_SET ); 
    fread( &esq, sizeof(Pagina), 1, indice );
    fseek( indice, offDir, SEEK_SET ); 
    fread( &dir, sizeof(Pagina), 1, indice );

    esq.id[esq.n] = p.id[idxEsq];
    esq.pos[esq.n] = p.pos[idxEsq];
    esq.filhos[esq.n + 1] = dir.filhos[0];
    esq.n++;

    p.id[idxEsq] = dir.id[0];
    p.pos[idxEsq] = dir.pos[0];

    for( int i=0; i < dir.n - 1; i++ ) {
        dir.id[i] = dir.id[i + 1];
        dir.pos[i] = dir.pos[i + 1];
        dir.filhos[i] = dir.filhos[i + 1];
    }
    dir.filhos[dir.n - 1] = dir.filhos[dir.n];
    dir.n--;

    fseek( indice, posiP, SEEK_SET ); 
    fwrite( &p, sizeof(Pagina), 1, indice) ;
    fseek( indice, offEsq, SEEK_SET ); 
    fwrite( &esq, sizeof(Pagina), 1, indice );
    fseek( indice, offDir, SEEK_SET ); 
    fwrite( &dir, sizeof(Pagina), 1, indice );
}

void pegarEmprestadoEsq( FILE* indice, long long posiPai, int idxEsq, int idxDir ) {
    Pagina p, esq, dir;
    fseek( indice, posiPai, SEEK_SET ); 
    fread( &p, sizeof(Pagina), 1, indice );

    long long offEsq = p.filhos[idxEsq];
    long long offDir = p.filhos[idxDir];
    fseek( indice, offEsq, SEEK_SET ); 
    fread( &esq, sizeof(Pagina), 1, indice );
    fseek( indice, offDir, SEEK_SET ); 
    fread( &dir, sizeof(Pagina), 1, indice );

    for( int i = dir.n; i > 0; i-- ) {
        dir.id[i] = dir.id[i - 1];
        dir.pos[i] = dir.pos[i - 1];
        dir.filhos[i + 1] = dir.filhos[i];
    }
    dir.filhos[1] = dir.filhos[0];
    dir.id[0] = p.id[idxEsq];
    dir.pos[0] = p.pos[idxEsq];
    dir.filhos[0] = esq.filhos[esq.n];
    dir.n++;

    p.id[idxEsq] = esq.id[esq.n - 1];
    p.pos[idxEsq] = esq.pos[esq.n - 1];
    esq.n--;

    fseek( indice, posiPai, SEEK_SET ); 
    fwrite( &p, sizeof(Pagina), 1, indice );
    fseek( indice, offEsq, SEEK_SET ); 
    fwrite( &esq, sizeof(Pagina), 1, indice );
    fseek( indice, offDir, SEEK_SET ); 
    fwrite( &dir, sizeof(Pagina), 1, indice );
}

int removerRecursivo( FILE* indice, long long posiP, int id ) {
    if ( posiP == -1 ) return 0;

    Pagina p;
    fseek( indice, posiP, SEEK_SET );
    fread( &p, sizeof(Pagina), 1, indice );

    int idx = 0;
    while ( idx < p.n && id > p.id[idx] ) {
        idx++;
    }

    int removido = 0;
    int fVerificar = idx;

    if ( idx < p.n && id == p.id[idx] ) {
        if ( p.folha ) {
            removerChaveDePagina( &p, idx );
            fseek( indice, posiP, SEEK_SET );
            fwrite( &p, sizeof(Pagina), 1, indice );
            return 1;
        } else {
            long long posiSucessor = p.filhos[idx + 1];
            int sobeID; 
            long long sobeDados;
            buscarSucessor( indice, posiSucessor, &sobeID, &sobeDados );
            
            p.id[idx] = sobeID;
            p.pos[idx] = sobeDados;
            fseek( indice, posiP, SEEK_SET );
            fwrite( &p, sizeof(Pagina), 1, indice );
            
            removido = removerRecursivo( indice, p.filhos[idx + 1], sobeID );
            fVerificar = idx + 1;
        }
    } else {
        removido = removerRecursivo( indice, p.filhos[idx], id );
        fVerificar = idx;
    }

    if ( removido == 1) {
        fseek( indice, posiP, SEEK_SET );
        fread( &p, sizeof(Pagina), 1, indice );
        
        if ( fVerificar > p.n ) {
            fVerificar = p.n;
        }
        
        if (p.folha == 1) {
            return 1; 
        }

        long long posiF = p.filhos[fVerificar];
        Pagina f;
        fseek( indice, posiF, SEEK_SET );
        fread( &f, sizeof(Pagina), 1, indice );

        if (f.n < MIN_CHAVES ) {
            if ( fVerificar > 0 ) {
                Pagina esq;
                fseek( indice, p.filhos[fVerificar - 1], SEEK_SET );
                fread( &esq, sizeof(Pagina), 1, indice );

                if ( esq.n > MIN_CHAVES ) {
                    pegarEmprestadoEsq( indice, posiP, fVerificar - 1, fVerificar );
                    return 1;
                }
            }
            
            if ( fVerificar < p.n ) {
                Pagina dir;
                fseek( indice, p.filhos[fVerificar+1], SEEK_SET );
                fread( &dir, sizeof(Pagina), 1, indice );

                if ( dir.n > MIN_CHAVES ) {
                    pegarEmprestadoDir( indice, posiP, fVerificar, fVerificar + 1 );
                    return 1;
                }
            }
            
            if ( fVerificar < p.n ) {
                mergePaginas( indice, posiP, fVerificar, fVerificar + 1 );
            } else {
                mergePaginas( indice, posiP, fVerificar - 1, fVerificar );
            }
        }
    }

    return removido;
}

void removerChave( FILE* indice, long long* raiz, int id ) {
    if ( buscar( *raiz, id, indice ) == -1) {
        printf( "ID nao existente\n" );
        return;
    }

    removerRecursivo( indice, *raiz, id );

    Pagina p;
    fseek( indice, *raiz, SEEK_SET );
    fread( &p, sizeof(Pagina), 1, indice );
    
    if ( p.n == 0 && p.folha == 0 ) {
        long long novaRaiz = p.filhos[0];
        *raiz = novaRaiz;
        
        fseek(indice, 0, SEEK_SET);
        fwrite(raiz, sizeof(long long), 1, indice);
    } 
    else if ( p.n == 0 && p.folha == 1 ) {
        *raiz = -1; 
        fseek( indice, 0, SEEK_SET );
        fwrite( raiz, sizeof(long long), 1, indice );
    }
}

void cascataRemover( FILE* indiceM, FILE* dadosM, long long* raizM, int tipoID, int id ) {
    rewind( dadosM );
    Matriculas m;
    int idsRemover[1000]; 
    int c = 0;
    
    while( fread( &m, sizeof(Matriculas), 1, dadosM ) ) {
        if ( buscar( *raizM, m.id, indiceM ) != -1 ) {
            if ( ( tipoID == 1 && m.matricula_aluno == id ) || ( tipoID == 2 && m.codigo_disciplina == id )) { //1 = Aluno || 2 = Disciplina
                idsRemover[c++] = m.id;
            }
        }
    }
    
    for ( int i = 0; i < c; i++ ) {
        removerChave( indiceM, raizM, idsRemover[i] );
        printf( "Matricula removida\n" );
    }
}

void removerAluno( FILE* indiceA, FILE* indiceM, FILE* dadosM, long long* posiA, long long* posiM ) {
    int id, validacao1;
    long long validacao2;
    
    do {
        printf( "Matricula: " ); 
        validacao1 = scanf( "%d", &id );
    
         if ( id <= 0 || validacao1 != 1 ) {
            printf( "Matricula invalida, tente novamente\n" );
            while (getchar() != '\n');
        }

        validacao2 = buscar( *posiA, id, indiceA );
        if ( validacao2 == -1 ) {
            printf( "Matricula nao existente, tente novamente\n" ); 
        }
    }while( id <= 0 || validacao1 != 1 || validacao2 == -1 );

    removerChave( indiceA, posiA, id );
    printf( "Aluno removido\n" );
    
    cascataRemover( indiceM, dadosM, posiM, 1, id );
}

void removerDisciplina( FILE* indiceD, FILE* indiceM, FILE* dadosM, long long* posiD, long long* posiM ) {
    int id, validacao1;
    long long validacao2;
    
    do {
        printf( "Codigo: " );  
        validacao1 = scanf( "%d", &id );
    
         if ( id <= 0 || validacao1 != 1 ) {
            printf( "Codigo invalido, tente novamente\n" );
            while (getchar() != '\n');
        }

        validacao2 = buscar( *posiD, id, indiceD );
        if ( validacao2 == -1 ) {
            printf( "Codigo nao existentente, tente novamente\n" ); 
        }
    }while( id <= 0 || validacao1 != 1 || validacao2 == -1 );
    

    removerChave( indiceD, posiD, id );
    printf( "Disciplina removida\n" );
    
    cascataRemover( indiceM, dadosM, posiM, 2, id );
}

void removerMatricula( FILE* indice, long long* posi ) {
    int id, validacao1;
    long long validacao2;

    do {
        printf( "ID: " );   
        validacao1 = scanf( "%d", &id );
    
         if ( id <= 0 || validacao1 != 1 ) {
            printf( "ID invalido, tente novamente\n" );
            while (getchar() != '\n');
        }

        validacao2 = buscar( *posi, id, indice );
        if ( validacao2 == -1 ) {
            printf( "ID nao existentente, tente novamente\n" ); 
        }
    }while( id <= 0 || validacao1 != 1 || validacao2 == -1 );
    
    removerChave( indice, posi, id );
    printf( "Matricula removida\n" );
}