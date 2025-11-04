#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 40
#define ALIMENTOS_FILE "alimentos.txt"
#define USUARIOS_FILE "usuarios.txt"
#define PEDIDOS_FILE "pedidos.txt"
#define AVALIACOES_FILE "avaliacoes.txt"

typedef struct {
    int id;
    char nome[128];
    double preco;
} Alimento;

typedef struct {
    char usuario[MAX_NAME];
    int pedido_id;
    int nota;
} Avaliacao;

/* ---------- utilitários ---------- */

void strip_newline(char *s) {
    size_t len = strlen(s);
    if (len == 0) return;
    if (s[len-1] == '\n') s[len-1] = '\0';
}

/* lê uma linha da stdin com fgets e remove newline */
void read_line(char *buffer, int size) {
    if (fgets(buffer, size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    strip_newline(buffer);
}

/* verifica se arquivo existe */
int file_exists(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) return 0;
    fclose(f);
    return 1;
}

/* ---------------------- alimentos ------------------------ */

/* carrega todos os alimentos em vetor. retorna quantidade lida */
int carregar_alimentos(Alimento *vet, int max) {
    FILE *f = fopen(ALIMENTOS_FILE, "r");
    if (!f) return 0;
    int count = 0;
    char linha[256];
    while (fgets(linha, sizeof(linha), f) && count < max) {
        strip_newline(linha);
        if (strlen(linha) == 0) continue;
        /* formato esperado: id;nome;preco */
        char *p = strtok(linha, ";");
        if (!p) continue;
        vet[count].id = atoi(p);
        p = strtok(NULL, ";");
        if (!p) continue;
        strncpy(vet[count].nome, p, sizeof(vet[count].nome)-1);
        vet[count].nome[sizeof(vet[count].nome)-1] = '\0';
        p = strtok(NULL, ";");
        if (!p) vet[count].preco = 0.0;
        else vet[count].preco = atof(p);
        count++;
    }
    fclose(f);
    return count;
}

/* busca por substring no nome (case insensitive simples) e imprime resultados.
   retorna número de resultados. */
int buscar_alimentos_por_nome(const char *termo, Alimento *resultado, int maxres) {
    Alimento vet[200];
    int n = carregar_alimentos(vet, 200);
    int found = 0;
    char termo_low[128];
    strncpy(termo_low, termo, sizeof(termo_low)-1);
    termo_low[sizeof(termo_low)-1] = '\0';
    for (int i = 0; termo_low[i]; i++) if (termo_low[i] >= 'A' && termo_low[i] <= 'Z') termo_low[i] += 32;
    
    for (int i = 0; i < n && found < maxres; i++) {
        char nome_low[128];
        strncpy(nome_low, vet[i].nome, sizeof(nome_low)-1);
        nome_low[sizeof(nome_low)-1] = '\0';
        for (int j = 0; nome_low[j]; j++) if (nome_low[j] >= 'A' && nome_low[j] <= 'Z') nome_low[j] += 32;
        if (strstr(nome_low, termo_low) != NULL) {
            if (resultado) resultado[found] = vet[i];
            printf("ID: %d | %s | R$ %.2f\n", vet[i].id, vet[i].nome, vet[i].preco);
            found++;
        }
    }
    return found;
}

/* busca por id e retorna ponteiro para struct local (ou NULL) */
int encontrar_alimento_por_id(int id, Alimento *out) {
    Alimento vet[200];
    int n = carregar_alimentos(vet, 200);
    for (int i = 0; i < n; i++) {
        if (vet[i].id == id) {
            if (out) *out = vet[i];
            return 1;
        }
    }
    return 0;
}

/* ---------- usuários ---------- */

int cadastrar_usuario() {
    FILE *u = fopen(USUARIOS_FILE, "a");
    if (u == NULL) {
        printf("Erro ao abrir arquivo de usuarios\n");
        return 1;
    }
    char nome[MAX_NAME], senha[MAX_NAME];
    printf("Digite seu nome de usuario: ");
    read_line(nome, sizeof(nome));
    printf("Digite sua senha: ");
    read_line(senha, sizeof(senha));

    /* validações simples: nome não vazio */
    if (strlen(nome) == 0) {
        printf("Nome vazio. Cadastro cancelado.\n");
        fclose(u);
        return 1;
    }

    /* salvar: formato nome;senha\n */
    fprintf(u, "%s;%s\n", nome, senha);
    fclose(u);
    printf("Usuario cadastrado com sucesso.\n");
    return 0;
}

/* tenta realizar login. Em sucesso, copia nome para usuario_logado e retorna 1.
   caso contrário, retorna 0. */
int login_usuario(char *usuario_logado) {
    char nome[MAX_NAME], senha[MAX_NAME];
    printf("Nome de usuario: ");
    read_line(nome, sizeof(nome));
    printf("Senha: ");
    read_line(senha, sizeof(senha));

    FILE *f = fopen(USUARIOS_FILE, "r");
    if (!f) {
        printf("Nenhum usuario cadastrado ainda.\n");
        return 0;
    }
    char linha[256];
    while (fgets(linha, sizeof(linha), f)) {
        strip_newline(linha);
        if (strlen(linha) == 0) continue;
        char *tok = strtok(linha, ";");
        if (!tok) continue;
        char tmp_nome[MAX_NAME];
        strncpy(tmp_nome, tok, sizeof(tmp_nome)-1);
        tmp_nome[sizeof(tmp_nome)-1] = '\0';
        tok = strtok(NULL, ";");
        char tmp_senha[MAX_NAME] = "";
        if (tok) strncpy(tmp_senha, tok, sizeof(tmp_senha)-1);
        if (strcmp(tmp_nome, nome) == 0 && strcmp(tmp_senha, senha) == 0) {
            strncpy(usuario_logado, nome, MAX_NAME-1);
            usuario_logado[MAX_NAME-1] = '\0';
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

/* ---------- pedidos ---------- */

/* determina próximo id de pedido lendo todos os existentes */
int proximo_id_pedido() {
    if (!file_exists(PEDIDOS_FILE)) return 1;
    FILE *f = fopen(PEDIDOS_FILE, "r");
    if (!f) return 1;
    int maxid = 0;
    char linha[512];
    while (fgets(linha, sizeof(linha), f)) {
        strip_newline(linha);
        if (strlen(linha) == 0) continue;
        /* formato: id;usuario;itens;total */
        char *tok = strtok(linha, ";");
        if (!tok) continue;
        int id = atoi(tok);
        if (id > maxid) maxid = id;
    }
    fclose(f);
    return maxid + 1;
}

/* salva pedido no arquivo. itens_ids é array de ints com count elementos. total é valor. */
int salvar_pedido(int id, const char *usuario, int *itens_ids, int count, double total) {
    FILE *f = fopen(PEDIDOS_FILE, "a");
    if (!f) return 0;
    fprintf(f, "%d;%s;", id, usuario);
    for (int i = 0; i < count; i++) {
        fprintf(f, "%d", itens_ids[i]);
        if (i < count-1) fprintf(f, "|");
    }
    fprintf(f, ";%.2f\n", total);
    fclose(f);
    return 1;
}

/* listar pedidos do usuario; imprime e retorna número encontrado */
int listar_meus_pedidos(const char *usuario) {
    if (!file_exists(PEDIDOS_FILE)) {
        printf("Nenhum pedido registrado.\n");
        return 0;
    }
    FILE *f = fopen(PEDIDOS_FILE, "r");
    if (!f) return 0;
    char linha[512];
    int found = 0;
    printf("Seus pedidos:\n");
    while (fgets(linha, sizeof(linha), f)) {
        strip_newline(linha);
        if (strlen(linha) == 0) continue;
        char copia[512];
        strncpy(copia, linha, sizeof(copia)-1);
        copia[sizeof(copia)-1] = '\0';
        char *tok = strtok(copia, ";");
        if (!tok) continue;
        int id = atoi(tok);
        tok = strtok(NULL, ";");
        if (!tok) continue;
        char usuario_lido[MAX_NAME];
        strncpy(usuario_lido, tok, sizeof(usuario_lido)-1);
        usuario_lido[sizeof(usuario_lido)-1] = '\0';
        if (strcmp(usuario_lido, usuario) != 0) continue;
        tok = strtok(NULL, ";"); /* itens */
        char itens_str[256] = "";
        if (tok) strncpy(itens_str, tok, sizeof(itens_str)-1);
        tok = strtok(NULL, ";"); /* total */
        double total = 0.0;
        if (tok) total = atof(tok);
        printf("ID: %d | Itens: %s | Total: R$ %.2f\n", id, itens_str, total);
        found++;
    }
    fclose(f);
    if (found == 0) printf("Nenhum pedido encontrado para este usuario.\n");
    return found;
}

/* exclui pedido do arquivo se pertence ao usuario; retorna 1 se excluiu */
int excluir_pedido(const char *usuario, int pedido_id) {
    if (!file_exists(PEDIDOS_FILE)) return 0;
    FILE *f = fopen(PEDIDOS_FILE, "r");
    if (!f) return 0;
    FILE *tmp = fopen("tmp_pedidos.txt", "w");
    if (!tmp) { fclose(f); return 0; }
    char linha[512];
    int encontrado = 0;
    while (fgets(linha, sizeof(linha), f)) {
        strip_newline(linha);
        if (strlen(linha) == 0) continue;
        char copia[512];
        strncpy(copia, linha, sizeof(copia)-1);
        copia[sizeof(copia)-1] = '\0';
        char *tok = strtok(copia, ";");
        if (!tok) continue;
        int id = atoi(tok);
        tok = strtok(NULL, ";");
        if (!tok) continue;
        char usuario_lido[MAX_NAME];
        strncpy(usuario_lido, tok, sizeof(usuario_lido)-1);
        usuario_lido[sizeof(usuario_lido)-1] = '\0';
        if (id == pedido_id && strcmp(usuario_lido, usuario) == 0) {
            /* pular esta linha -> excluir */
            encontrado = 1;
            continue;
        } else {
            fprintf(tmp, "%s\n", linha);
        }
    }
    fclose(f);
    fclose(tmp);
    if (encontrado) {
        remove(PEDIDOS_FILE);
        rename("tmp_pedidos.txt", PEDIDOS_FILE);
        return 1;
    } else {
        remove("tmp_pedidos.txt");
        return 0;
    }
}

/* ---------- avaliacoes ---------- */

int salvar_avaliacao(const Avaliacao *a) {
    FILE *f = fopen(AVALIACOES_FILE, "a");
    if (!f) return 0;
    fprintf(f, "%s;%d;%d\n", a->usuario, a->pedido_id, a->nota);
    fclose(f);
    return 1;
}

/* ---------- operações do usuário logado ---------- */

/* criar pedido: mostra busca de alimentos, permite escolher IDs, calcula total simbólico e salva */
void fazer_pedido(const char *usuario) {
    char busca[128];
    printf("Buscar alimento (digite parte do nome): ");
    read_line(busca, sizeof(busca));
    Alimento encontrados[100];
    int n_found = buscar_alimentos_por_nome(busca, encontrados, 100);
    if (n_found == 0) {
        printf("Nenhum alimento encontrado para o termo.\n");
        return;
    }
    printf("Digite quantos itens deseja adicionar: ");
    char tmp[16];
    read_line(tmp, sizeof(tmp));
    int qtd = atoi(tmp);
    if (qtd <= 0) {
        printf("Quantidade invalida.\n");
        return;
    }
    int itens_ids[50];
    int added = 0;
    double total = 0.0;
    for (int i = 0; i < qtd; i++) {
        printf("Digite o ID do item %d (conforme listagem): ", i+1);
        read_line(tmp, sizeof(tmp));
        int id = atoi(tmp);
        Alimento a;
        if (!encontrar_alimento_por_id(id, &a)) {
            printf("ID %d nao encontrado. Tente outro.\n", id);
            i--; /* repetir esta posição */
            continue;
        }
        itens_ids[added++] = id;
        total += a.preco; /* preço simbólico do arquivo */
    }
    int id_pedido = proximo_id_pedido();
    if (salvar_pedido(id_pedido, usuario, itens_ids, added, total)) {
        printf("Pedido %d registrado. Total: R$ %.2f\n", id_pedido, total);
    } else {
        printf("Erro ao salvar pedido.\n");
    }
}

/* permitir avaliação: usuário escolhe id do pedido (deve ser seu) e nota 0-5 */
void avaliar_pedido(const char *usuario) {
    printf("Seus pedidos:\n");
    int n = listar_meus_pedidos(usuario);
    if (n == 0) return;
    printf("Digite o ID do pedido que deseja avaliar: ");
    char tmp[16];
    read_line(tmp, sizeof(tmp));
    int id = atoi(tmp);
    /* verificar se o pedido pertence ao usuario */
    FILE *f = fopen(PEDIDOS_FILE, "r");
    if (!f) {
        printf("Arquivo de pedidos inacessivel.\n");
        return;
    }
    int pertence = 0;
    char linha[512];
    while (fgets(linha, sizeof(linha), f)) {
        strip_newline(linha);
        if (strlen(linha) == 0) continue;
        char copia[512]; strncpy(copia, linha, sizeof(copia)-1);
        char *tok = strtok(copia, ";");
        if (!tok) continue;
        int id_lido = atoi(tok);
        tok = strtok(NULL, ";");
        if (!tok) continue;
        if (id_lido == id && strcmp(tok, usuario) == 0) {
            pertence = 1; break;
        }
    }
    fclose(f);
    if (!pertence) {
        printf("Pedido nao encontrado ou nao pertence a voce.\n");
        return;
    }
    printf("Digite a nota (0 a 5): ");
    read_line(tmp, sizeof(tmp));
    int nota = atoi(tmp);
    if (nota < 0 || nota > 5) {
        printf("Nota invalida.\n");
        return;
    }
    Avaliacao a;
    strncpy(a.usuario, usuario, MAX_NAME-1); a.usuario[MAX_NAME-1] = '\0';
    a.pedido_id = id;
    a.nota = nota;
    if (salvar_avaliacao(&a)) {
        printf("Avaliacao salva.\n");
    } else {
        printf("Erro ao salvar avaliacao.\n");
    }
}

int alterar_pedido(const char *usuario) {
    if (!file_exists(PEDIDOS_FILE)) {
        printf("Nenhum pedido encontrado.\n");
        return 0;
    }

    // Mostra os pedidos do usuário
    listar_meus_pedidos(usuario);
    printf("Digite o ID do pedido que deseja alterar: ");
    char tmp[32];
    read_line(tmp, sizeof(tmp));
    int id_alvo = atoi(tmp);

    FILE *f = fopen(PEDIDOS_FILE, "r");
    FILE *tmpf = fopen("tmp_pedidos.txt", "w");
    if (!f || !tmpf) {
        printf("Erro ao abrir arquivos.\n");
        if (f) fclose(f);
        if (tmpf) fclose(tmpf);
        return 0;
    }

    char linha[512];
    int alterado = 0;

    while (fgets(linha, sizeof(linha), f)) {
        strip_newline(linha);
        if (strlen(linha) == 0) continue;

        char copia[512];
        strncpy(copia, linha, sizeof(copia)-1);
        copia[sizeof(copia)-1] = '\0';

        char *tok = strtok(copia, ";");
        if (!tok) continue;
        int id = atoi(tok);
        tok = strtok(NULL, ";");
        if (!tok) continue;
        char usuario_lido[MAX_NAME];
        strncpy(usuario_lido, tok, sizeof(usuario_lido)-1);
        usuario_lido[sizeof(usuario_lido)-1] = '\0';

        if (id == id_alvo && strcmp(usuario_lido, usuario) == 0) {
            // Pedido encontrado
            char *itens_str = strtok(NULL, ";");
            char *total_str = strtok(NULL, ";");
            double total = total_str ? atof(total_str) : 0.0;

            printf("\nPedido atual:\nItens: %s | Total: R$ %.2f\n", itens_str, total);
            printf("O que deseja fazer?\n");
            printf("1 - Adicionar alimento\n");
            printf("2 - Remover alimento\n");
            printf("Escolha: ");
            read_line(tmp, sizeof(tmp));
            int escolha = atoi(tmp);

            // Copia itens atuais para vetor de inteiros
            int itens_ids[50];
            int count = 0;
            char *token = strtok(itens_str, "|");
            while (token && count < 50) {
                itens_ids[count++] = atoi(token);
                token = strtok(NULL, "|");
            }

            if (escolha == 1) {
                // Adicionar novo item
                printf("Digite o nome (ou parte) do alimento para adicionar: ");
                char termo[128];
                read_line(termo, sizeof(termo));
                Alimento encontrados[50];
                int n = buscar_alimentos_por_nome(termo, encontrados, 50);
                if (n == 0) {
                    printf("Nenhum alimento encontrado.\n");
                } else {
                    printf("Digite o ID do alimento que deseja adicionar: ");
                    read_line(tmp, sizeof(tmp));
                    int id_add = atoi(tmp);
                    Alimento a;
                    if (encontrar_alimento_por_id(id_add, &a)) {
                        itens_ids[count++] = id_add;
                        total += a.preco;
                        printf("Alimento adicionado: %s (R$ %.2f)\n", a.nome, a.preco);
                    } else {
                        printf("ID nao encontrado.\n");
                    }
                }
            } else if (escolha == 2) {
                // Remover item existente
                printf("IDs atuais do pedido: ");
                for (int i = 0; i < count; i++) printf("%d ", itens_ids[i]);
                printf("\nDigite o ID do alimento a remover: ");
                read_line(tmp, sizeof(tmp));
                int id_remove = atoi(tmp);

                int novo_count = 0;
                int removido = 0;
                Alimento a;
                double preco_removido = 0.0;

                for (int i = 0; i < count; i++) {
                    if (itens_ids[i] == id_remove && !removido) {
                        if (encontrar_alimento_por_id(id_remove, &a))
                            preco_removido = a.preco;
                        removido = 1;
                        continue;
                    }
                    itens_ids[novo_count++] = itens_ids[i];
                }

                if (removido) {
                    count = novo_count;
                    total -= preco_removido;
                    printf("Item %d removido (R$ %.2f).\n", id_remove, preco_removido);
                } else {
                    printf("ID nao encontrado no pedido.\n");
                }
            } else {
                printf("Opcao invalida.\n");
            }

            // Reescreve o pedido alterado no arquivo temporário
            fprintf(tmpf, "%d;%s;", id, usuario);
            for (int i = 0; i < count; i++) {
                fprintf(tmpf, "%d", itens_ids[i]);
                if (i < count - 1) fprintf(tmpf, "|");
            }
            fprintf(tmpf, ";%.2f\n", total);
            alterado = 1;
        } else {
            // Mantém pedidos inalterados
            fprintf(tmpf, "%s\n", linha);
        }
    }

    fclose(f);
    fclose(tmpf);

    if (alterado) {
        remove(PEDIDOS_FILE);
        rename("tmp_pedidos.txt", PEDIDOS_FILE);
        printf("Pedido alterado com sucesso!\n");
        return 1;
    } else {
        remove("tmp_pedidos.txt");
        printf("Pedido nao encontrado ou nao pertence a voce.\n");
        return 0;
    }
}


/* menu depois de logado */
void menu_usuario(const char *usuario) {
    while (1) {
        printf("\n-- Menu (usuario: %s) --\n", usuario);
        printf("1 - Buscar alimento\n");
        printf("2 - Fazer pedido\n");
        printf("3 - Listar meus pedidos\n");
        printf("4 - Excluir pedido\n");
        printf("5 - Avaliar pedido\n");
        printf("6 - Logout\n");
        printf("7 - Alterar pedido\n");
        printf("Escolha: ");
        char tmp[16];
        read_line(tmp, sizeof(tmp));
        int opc = atoi(tmp);
        if (opc == 1) {
            char termo[128];
            printf("Termo de busca: ");
            read_line(termo, sizeof(termo));
            buscar_alimentos_por_nome(termo, NULL, 100);
        } else if (opc == 2) {
            fazer_pedido(usuario);
        } else if (opc == 3) {
            listar_meus_pedidos(usuario);
        } else if (opc == 4) {
            listar_meus_pedidos(usuario);
            printf("Digite ID do pedido a excluir: ");
            read_line(tmp, sizeof(tmp));
            int id = atoi(tmp);
            if (excluir_pedido(usuario, id)) printf("Pedido %d excluido.\n", id);
            else printf("Falha ao excluir (verifique se o id existe e pertence a voce).\n");
        } else if (opc == 5) {
            avaliar_pedido(usuario);
        } else if (opc == 6) {
            printf("Logout...\n");
            break;
        } else if (opc == 7) {
            alterar_pedido(usuario);
        }else {
            printf("Opcao invalida.\n");
        }
    }
}

/* ---------- menus principais ---------- */

void menu_principal_print() {
    printf("\n-- FEIFood --\n");
    printf("1 - Cadastrar usuario\n");
    printf("2 - Login\n");
    printf("3 - Sair\n");
    printf("Escolha: ");
}

int main() {
    char usuario_logado[MAX_NAME] = "";
    while (1) {
        menu_principal_print();
        char escolha_str[16];
        read_line(escolha_str, sizeof(escolha_str));
        int escolha = atoi(escolha_str);
        if (escolha == 1) {
            cadastrar_usuario();
        } else if (escolha == 2) {
            printf("---- Login ----\n");
            if (login_usuario(usuario_logado)) {
                printf("Login bem-sucedido. Bem-vindo, %s!\n", usuario_logado);
                menu_usuario(usuario_logado);
                usuario_logado[0] = '\0'; /* deslogar */
            } else {
                printf("Login falhou. Verifique usuario/senha.\n");
            }
        } else if (escolha == 3) {
            printf("Saindo...\n");
            break;
        } else {
            printf("Opcao invalida.\n");
        }
    }
    return 0;
}
