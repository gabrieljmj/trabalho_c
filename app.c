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
        printf("\nErro ao salvar alterações!!\n");
        return;
    }

    printf("\nDados do usuário atualizados:\n\n");
    showUser(res.u);
    printf("\n\n");
}

void deleteUser(int s) {
    struct user u;
    struct message req = { CODE_DELETE };
    struct message res;

    printf("Usuário: ");
    scanf("%s", u.username);

    req.u = u;

    sendMessage(req, s);
    receiveMessage(&res, s);

    printf("Response code: %d\n", res.code);
}

void allUsers(int s) {
    
}

void showMenu(int s) {
	int option;

	while(1) {
		printf("Menu:\n 1 - Adicionar usuário\n 2 - Buscar usuário por username\n 3 - Alterar usuário\n 4 - Excluir usuário\nOpção: ");
		scanf("%d", &option);

		switch(option) {
			case 1:
                system("clear");
				createUser(s);
				break;
            case 2:
                system("clear");
                getUser(s);
                break;
            case 3:
                system("clear");
                updateUser(s);
                break;
            case 4:
                system("clear");
                deleteUser(s);
                break;
            case 5:
                system("clear");
                allUsers(s);
                break;
            default:
                printf("Opção inválida!");
		}
	}
}

int main(int argc, char** argv) {
	setlocale(LC_ALL, "");

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
