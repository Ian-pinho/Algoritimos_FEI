#include<stdio.h>
#include"func.h"

int main(){
    float a = 25, b = 10;

    printf("A soma é: %.2f\n", sum(a, b));
    printf("A subtração é: %.2f\n", sub(a, b));
    printf("A multiplicação é: %.2f\n", mult(a, b));
    
    if (ehDiv(b)){
        printf("A divisão é: %.2f\n", div(a, b));
    }
    else{
        printf("Impossível, divisão por 0.\n");
    }
}