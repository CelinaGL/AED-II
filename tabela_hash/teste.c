#include <stdio.h>

int main() {
    char c[] = "AC", *ca;
    int num;

    ca = c;
    num = *ca;
    printf("%d ",num);
    ca++;
    num = *ca;
    printf("%d ",num);
}