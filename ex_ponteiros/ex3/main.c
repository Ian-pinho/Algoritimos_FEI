#include <stdio.h>

int main(){
    int v[5] = {10, 20, 30, 40, 50};
    int *ptr = v;

    for(int i = 0; i < 5; i++){
        printf("%d\n", *(ptr+i));
        printf("%p\n", ptr+i);
    }
    return 0;
}