#include <stdio.h>
#include <math.h>

#define TAM_HASH 90000000000
#define P 31

long long primaria( char *word );

int main(){
    char word[100];

    scanf( "%[^\n]", word );
    printf("%s ", word);
    long long k = primaria( word );
    printf("%lld", k);

}

long long primaria( char *word ){
    //somatorio até n - 1 das (letras segundo a tabela ascii * P elevado a i) mod(%) TAM_HASH
    long long num, pot = 1, k = 0;
    
    while ( *word ) {
        num = *word;

        k = (k + ((num * pot) % TAM_HASH )) % TAM_HASH;  
        pot *= P;        

        word++;
    }

    return k;
}