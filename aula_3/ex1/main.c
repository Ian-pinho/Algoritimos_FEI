#include<stdio.h>
#include"maior.h"

int main(){
    float n1, n2;
    printf("Digite um número: \n");
    scanf("%f", &n1);
    printf("Digite um número: \n");
    scanf("%f", &n2);

    return maior(n1,n2);
}