#include <stdio.h>

void troca(int *a, int *b){
    int temp = *a;
    int temp1 = *b;
    *a = temp1;
    *b = temp;
    temp = 0;
    temp1 = 0;
}

int main(){
    int x = 5, y = 10;

    printf("Antes da função: %d, %d\n", x, y);
    troca(&x, &y);
    printf("Depois da função: %d, %d\n", x, y);

    return 0;
}