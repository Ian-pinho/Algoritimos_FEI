#include<stdio.h>
#include"numeros.h"

int main(){
    int a = 3, b = 4;

    printf("%d\n", par(a));
    printf("%d\n", par(b));

    printf("%d\n", prim(a));
    printf("%d\n", prim(b));
    
    printf("%d\n", fat(a));
    printf("%d\n", fat(b));
    
    printf("%d\n", maior(a, b));

    printf("%d\n", pot(a, b));
    printf("%d\n", pot(b, a));
}