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
    printf("Nome: %s", u.name);
}

void createUser(int s) {
    struct user u;
    struct message msg;

    printf("Nome: ");
    scanf("%s", u.name);

    printf("Username: ");
    scanf("%s", u.username);

    u.status = 1;

    msg.action = ACTION_CREATE;
    msg.u = u;

    sendMessage(msg, s);
}

void getUser(int s) {
    struct user u;
    struct message req;
    struct message res = { RESPONSE };
    int id;

    printf("Usuário: ");
    scanf("%s", u.username);

    req.action = ACTION_GET;
    req.u = u;

    sendMessage(req, s);
    receiveMessage(&res, s);

    if (res.action == ERROR_RESPONSE) {
        printf("\nUsuário não encontrado!\n\n");
        return;
    }

    printf("\nDados do usuário:\n\n");
    showUser(res.u);
    printf("\n\n");
}

void getAllUsers() {

}

void showMenu(int s) {
	int option;

	while(1) {
		printf("Menu:\n 1 - Adicionar usuário\n 2 - Buscar usuário por username\n 3 - Listar todos os usuários\n 4 - Alterar usuário\n 5 - Excluir usuário\nOpção: ");
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
            default:
                printf("Opção inválida!");
		}
	}
}

int main(int argc, char** argv) {
	setlocale(LC_ALL, "");

	unsigned short port;       /* Porta que o cliente ira se conectar              */
    char buf[100];              /* Buffer para enviar e receber as informações    */
    struct hostent *hostnm;    /* Informação do host servidor             */
    struct sockaddr_in server; /* Estrutura do servidor                           */
    int s;                     /* Socket do cliente       */

    if (argc != 3)
    {
        fprintf(stderr, "Uso correto: %s hostname port\n", argv[0]);
        exit(1);
    }

    /*
     * O nome do host é o primeiro argumento. Obtem o endereço do servidor.
     */
    hostnm = gethostbyname(argv[1]);

    if (hostnm == (struct hostent *) 0) {
        fprintf(stderr, "Falha no Gethostbyname \n");
        exit(2);
    }

    port = (unsigned short) atoi(argv[2]);

    server.sin_family      = AF_INET;
    server.sin_port        = htons(port);
    server.sin_addr.s_addr = *((unsigned long *)hostnm->h_addr);

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Erro ao criar o Socket");
        exit(3);
    }

    if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("Erro ao conectar com o servidor");
        exit(4);
    }

    showMenu(s);
}