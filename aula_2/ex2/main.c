#include<stdio.h>
#include"func.h"

int main(){
    printf("A soma é: %d\n", soma(3, 7));
    printf("A soma é: %.2f\n", somaf(3.3, 7.2));
    printf("A subtração é: %d\n", sub(7, 3));
    return 0;
}