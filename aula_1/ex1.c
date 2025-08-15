#include <stdio.h>
#include <math.h>

int main(){
    float NF, lab, lab1, lab2, lab3, pj, pv;
    printf("Digite a nota do laboratório 1: ");
    scanf("%f", &lab1);
    printf("Digite a nota do laboratório 2: ");
    scanf("%f", &lab2);
    printf("Digite a nota do laboratório 3: ");
    scanf("%f", &lab3);
    printf("Digite a nota do projeto: ");
    scanf("%f", &pj);
    printf("Digite a nota da prova final: ");
    scanf("%f", &pv);

    lab = (lab1+lab2+lab3)/3;
    NF = lab*0.2+0.8*sqrt(pj*pv);

    printf("Sua nota final é: %.1f\n", NF);

    return 0;
}