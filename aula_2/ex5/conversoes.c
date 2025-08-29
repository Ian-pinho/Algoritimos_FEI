#include<stdio.h>
#include"conversoes.h"

int menu(){
    int opc = 0;
    while(1){
        printf("Digite 1 para fazer conversões entre Celsius/Fahrenheit\n");
        printf("Digite 2 para fazer conversões entre metros/quilômetros\n");
        printf("Digite 3 para fazer conversões entre segundos/minutos\n");
        printf("Digite a opção desejada: ");
        scanf("%d", &opc);
        printf("%d\n", opc);

        return opc;
    }
}

float CouF(){
    float n1;
    printf("Digite o valor em Celsius para fazer a conversão entre Celsius/Fahrenheit\n");
    scanf("%f", &n1);

    return n1*9/5+32;

}
float MouQ(){
    float n1;
    printf("Digite o valor em metros para fazer a conversão entre metros/Kilometros\n");
    scanf("%f", &n1);

    return n1/1000;

}
float SouM(){
    float n1;
    printf("Digite o valor em segundos para fazer a conversão entre segundos/minutos\n");
    scanf("%f", &n1);

    return n1/60;

}