#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#include "message.h"

void showUser(struct user u) {
    printf("Usuário: %s\n", u.username);
    printf("Nome: %s\n", u.name);
    printf("E-mail: %s", u.email);
}

/**
 * Cria um usuário
 */
void createUser(int s) {
    struct user u;
    struct message msg;

    printf("Nome: ");
    scanf("%s", u.name);

    printf("Username: ");
    scanf("%s", u.username);

    printf("E-mail: ");
    scanf("%s", u.email);

    u.status = 1;

    msg.code = CODE_CREATE;
    msg.u = u;

    sendMessage(msg, s);
}

/**
 * Mostra os dados de um usuário a partir de seu username
 */
void getUser(int s) {
    struct user u;
    struct message req;
    struct message res;
    int id;

    printf("Usuário: ");
    scanf("%s", u.username);

    req.code = CODE_GET;
    req.u = u;

    sendMessage(req, s);
    receiveMessage(&res, s);

    if (res.code == CODE_ERROR_USER_NOT_FOUND_RESPONSE) {
        printf("\nUsuário não encontrado!\n");
        return;
    }

    printf("\nDados do usuário:\n\n");
    showUser(res.u);
    printf("\n\n");
}

/**
 * Atualiza os dados de um usuário
 */
void updateUser(int s) {
    struct user u;
    struct message req;
    struct message res;
    int alterData;

    printf("Usuário: ");
    scanf("%s", u.username);

    printf("Dados a serem alterados:\n\n");

    printf("Nome: ");
    scanf("%s", u.name);

    printf("E-mail: ");
    scanf("%s", u.email);

    req.code = CODE_UPDATE;
    req.u = u;

    sendMessage(req, s);
    receiveMessage(&res, s);

    if (res.code == CODE_ERROR_USER_NOT_FOUND_RESPONSE) {
        printf("\nUsuário não encontrado!\n");
        return;
    } else if (res.code == CODE_ERROR_UPDATING_RESPONSE) {
        printf("\nErro ao salvar alterações!\n");
        return;
    }

    printf("\nDados do usuário atualizados:\n\n");
    showUser(res.u);
    printf("\n\n");
}


/**
 * Delete um usuário
 */
void deleteUser(int s) {
    struct user u;
    struct message req = { CODE_DELETE };
    struct message res;

    printf("Usuário: ");
    scanf("%s", u.username);

    req.u = u;

    sendMessage(req, s);
    receiveMessage(&res, s);

    if (res.code == CODE_ERROR_USER_NOT_FOUND_RESPONSE) {
        printf("\nUsuário não encontrado!\n");
        return;
    }

    if (res.code == CODE_ERROR_DELETING_RESPONSE) {
        printf("\nErro ao excluir usuário!\n");
        return;
    }

    if (res.code == CODE_SUCCESS_RESPONSE) {
        printf("\nUsuário deletado com sucesso!\n");
    }
}

/**
 * Retorna os dados de todos os usuários
 */
void getAllUsers(int s) {
    struct message req;
    struct message res;
    int last = 0;
  
    req.code = CODE_GET_ALL;
  
    sendMessage(req, s);
  
    while(!last) {
        receiveMessage(&res, s);
            
        showUser(res.u);

        printf("\n\n");
            
        last = res.unique ? 1 : 0;
    }

    if (!last) {
        printf("\nNenhum usuário cadastrado!\n");
    }
}

void closeConnection(int s) {
    struct message req;

    req.code = CODE_CLOSE_CONNECTION;

    sendMessage(req, s);
    exit(0);
}

/**
 * Exibe o menu
 */
void showMenu(int s) {
    int option;

    while(1) {
        printf("Menu:\n 1 - Adicionar usuário\n 2 - Buscar usuário por username\n 3 - Listar todos os usuários\n 4 - Alterar usuário\n 5 - Excluir usuário\n 0 - Sair\nOpção: ");
        scanf("%d", &option);

        system("clear");

        switch(option) {
            case 1:
                createUser(s);
                break;
            case 2:
                getUser(s);
                break;
            case 3:
                getAllUsers(s);
                break;
            case 4:
                updateUser(s);
                break;
            case 5:
                deleteUser(s);
                break;
            case 0:
                closeConnection(s);
            default:
                printf("Opção inválida!\n\n");
                break;
        }
    }
}

int main(int argc, char** argv) {
    setlocale(LC_ALL, "");
printf("%d", CODE_GET);
    unsigned short port; // Porta que o cliente ira se conectar
    struct hostent *hostnm; // Informação do host servidor
    struct sockaddr_in server; // Estrutura do servidor
    int s; // Socket do cliente

    if (argc != 3) {
        fprintf(stderr, "Uso correto: %s hostname port\n", argv[0]);
        exit(1);
    }

    // O nome do host é o primeiro argumento. Obtem o endereço do servidor.
    hostnm = gethostbyname(argv[1]);

    if (hostnm == (struct hostent *) 0) {
        fprintf(stderr, "Falha no Gethostbyname \n");
        exit(2);
    }

    port = (unsigned short) atoi(argv[2]);
    server.sin_family      = AF_INET;
    server.sin_port        = htons(port);
    server.sin_addr.s_addr = *((unsigned long *)hostnm->h_addr);

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Erro ao criar o Socket");
        exit(3);
    }


    if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Erro ao conectar com o servidor");
        exit(4);
    }

    showMenu(s);

    return 0;
}
