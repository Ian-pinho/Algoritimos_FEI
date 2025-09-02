#include<stdio.h>

float media(float x, float y, float z){
        return((x+y+z)/3);
}

int main(){
    float n1 = 7, n2 = 6.5, n3 = 9.3;

    if(media(n1,n2,n3)>=7){
        printf("Média: %.2f\nAluno aprovado!\n", media(n1, n2, n3));
    }
    else if(media(n1,n2,n3)>=5){
        printf("Média: %.2f\nAluno de recuperação.\n", media(n1, n2, n3));
    }
    else{
        printf("Média: %.2f\nAluno reprovado!\n", media(n1, n2, n3));
    }

    return 0;
}