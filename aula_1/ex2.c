#include <stdio.h>

int main(){
    int h, m, s, tAtual, tFaltante;
    
    printf("Digite as horas do horário atual: ");
    scanf("%d", &h);
    printf("Digite os minutos do horário atual: ");
    scanf("%d", &m);
    printf("Digite os segundos do horário atual: ");
    scanf("%d", &s);

    tAtual = h*60*60+m*60+s;
    tFaltante = 24*60*60-tAtual;

    printf("Se passaram %d segundos desde as 00:00:00 e faltam %d segundos para meia noite.\n", tAtual, tFaltante);

    return 0;    
}