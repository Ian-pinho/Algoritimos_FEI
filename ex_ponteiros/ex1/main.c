#include <stdio.h>

int main(){

    int x = 10;
    int *ptr = &x;

    printf("Valor de X: %d\n", x);
    printf("Valor apontado por ptr: %d\n", *ptr);
    printf("Endereço de X: %p\n", &x);
    printf("Endereço guardado em ptr: %p\n", ptr);
    printf("Endereço de ptr: %p\n", &ptr);

    return 0;
}