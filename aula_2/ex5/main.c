#include<stdio.h>
#include"conversoes.h"

int main(){
    int opc = menu();

    if(opc ==1){
        printf("%.2f\n", CouF());
    }
    if(opc ==2){
        printf("%.2f\n", MouQ());
    }
    if(opc ==3){
        printf("%.2f\n", SouM());
    }
    return(0);
}