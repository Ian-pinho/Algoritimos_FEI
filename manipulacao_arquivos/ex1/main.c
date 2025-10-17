#include <stdio.h>

int main(){
    int n = 0;
    int acm = 0;
    FILE *f = fopen("numeros.bin", "wb");
    if(f == NULL) return 1;

    for(int i = 0; i < 5; i++){
        printf("Digite um número: \n");
        scanf("%d", &n);
        fwrite(&n, sizeof(int), 1, f);
    }
    fclose(f);

    f = fopen("numeros.bin", "rb");
    if(f == NULL) return 1;
    while(fread(&n, sizeof(int), 1, f) == 1){
        acm += n;
    }
    printf("%d\n", acm);

    return 0;
}