#include<stdio.h>

int main(){
    int idade; 
   
    printf("Digite a sua idade: \n");
    scanf("%d", &idade);

    if(idade>=18){
        printf("Idade: %d\nVocê é maior de idade!\n", idade);
    }
    else{
        printf("Idade: %d\nVocê não é maior de idade!\n", idade);
    }

    return 0;
}