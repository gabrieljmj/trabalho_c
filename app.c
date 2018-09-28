#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

#include "message.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 

/*struct user {
	char name[15];
};

/*struct message {
	char buffer[100];
	int size;
};

/*void sendMessage(struct message msg, int s) {
	if (send(s, msg.buffer, msg.size, 0) < 0) {
        perror("Erro ao enviar uma mensagem ");
        exit(5);
    }
}*/

void createUserQuery(char *msg, struct user u) {
	sprintf(msg, "Nome: %s", u.name);
}

void create(int s) {
	struct user u;
	struct message msg;
	char name[15];

	printf("Nome: ");
	scanf("%s", u.name);

	//createUserQuery(msg.buffer, u);
	msg.instruction = 1;
    msg.u = u;

	//printf("Mensagem: %s", msg.buffer);
	sendMessage(msg, s);
}

void showMenu(int s) {
	int option;

	while(1) {
		printf("Menu:\n 1 - Criar usuário\nOpção: ");
		scanf("%d", &option);

		switch(option) {
			case 1:
				create(s);
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