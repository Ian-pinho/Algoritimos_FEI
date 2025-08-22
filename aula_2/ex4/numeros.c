int par(int x){
    if(x%2 == 0){
        return 1;
    }
    else{
        return 0;
    }
}

int prim(int x){
    for(int i = 2; i < x ;i++){
        if(x % i == 0){
            return 0;
        }
    }
    return 1;
}

int fat(int x){
    int fator = 1;
    for(int i = 1; i <= x; i++){
        fator = fator *i;
    }
    return fator;
}

int maior(int x, int y){
    if(x >= y){
        return x;
    }
    return y;
}

int pot(int base, int exp){
    int potencia = 1;
    for(int i = 1; i <= exp; i++){
        potencia = potencia*base;
    }
    return potencia;
}