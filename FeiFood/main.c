#include <stdio.h>

int main(){
    int login = 0;
    int escolha = 0;
    while(1){
        if(login == 0){
            menu_principal()
            if(escolha == 1){
                cadastrar_usuario()
            }
        }
    }

    int menu_principal(){
        printf("1 - Cadastrar usuário\n");
        printf("2 - Login\n");
        printf("3 - Sair\n");
        return(scanf(%d, &escolha));
    }
    int cadastrar_usuario(){
        FILE *u = fopen("usuarios.txt", "a");
        if (u == NULL) {
            printf("Erro ao abrir arquivo\n");
            return 1;
        }
        char nome[20], senha[20];
        printf("Digite seu nome de usuário: ");
        fgets(nome, sizeof(nome), stdin);
        fprintf(u, "%s,", nome);

        printf("Digite a sua senha: ");
        fgets(senha, sizeof(senha), stdin);
        fprintf(u, "%s\n", senha);

        fclose(u);
        return 0;
    }
    return 0;
}