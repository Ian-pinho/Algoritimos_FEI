#include<stdio.h>
#include"maior.h"

float maior(float n1, float n2){
    if(n1>n2){
        printf("%.2f é maior que %.2f.", n1, n2);
    }
    else if(n1<n2){
        printf("%.2f é maior que %.2f.", n2, n1);
    }
    else{
        printf("Os dois números %.2f, %.2f são iguais!", n1, n2);
    }
}