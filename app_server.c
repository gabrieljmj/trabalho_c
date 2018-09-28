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
#include <ncurses.h>

#include "message.h"
#include "sockets_conn_functions.h"

#define FILE_NAME "users"

FILE *fdoc;

void openFile(char *file, char *opts) {
	if ((fdoc = fopen(file, opts)) == NULL) {
        printf("\nO arquivo nao pode ser aberto!! \n");
        getch();
        exit(1);
    }
}

void saveUser(struct user u) {
	openFile(FILE_NAME, "ab+");
	fseek(fdoc,0L, SEEK_END);
printf("%s", u.name);
	if(fwrite(&u, sizeof(struct user), 1, fdoc) != 1) {
        printf("\n Erro de gravacao!");
        getch();
    } else {
        printf("\n Gravacao feita com sucesso!\n\n");
    }
}

void checkArguments(int argc, char** argv);

int main(int argc, char** argv) {
    

    /*checkArguments(argc, argv);
    definePort(&port, argv);
    createSocket(&s);
    server.sin_family = AF_INET;
    server.sin_port   = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;
    bindSocketToServerAdress(&s, server, port);
    waitForConnections(&s);

    if (recv(ns, &msg, sizeof(struct message), 0) == -1)
    {
        perror("Erro ao receber a mensagem");
        exit(6);
    }

    printf("%s\n", msg.u.name);*/

    unsigned short port;       /* port server binds to                  */
    char buf_cli[100];              /* Buffer para enviar e receber os dados */
    char buf_server[100];              /* Buffer para enviar e receber os dados */
    struct sockaddr_in client; /* Informações do cliente            */
    struct sockaddr_in server; /* Informações do endereço do servidor            */
    int s;                     /* Socket para aceitar conecxões      */
    int ns;                    /* Socket para conectar com os clientes            */
    int namelen;               /* Tamanho do nome do cliente                 */
    struct message msg;

    /*
     * Checa os argumentos. Devem ser somente: O número da porta para a comunicação.
     */

    if (argc != 2)
    {
        fprintf(stderr, "Uso correto: %s port\n", argv[0]);
        exit(1);
    }

    /*
     * O primeiro argumento deve ser a porta.
     */
    port = (unsigned short) atoi(argv[1]);

    /*
     * Cria o socket para aceitar conexões.
     */
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Erro ao criar o socket");
        exit(2);
    }

    /*
     * Bind the socket to the server address.
     */
    server.sin_family = AF_INET;
    server.sin_port   = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(s, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
       perror("Erro no Bind()");
       exit(3);
    }

    
    /*
     * Esperando por conexões. Especificando apenas uma conexão .
     */
    if (listen(s, 1) != 0)
    {
        perror("Erro na espera de conexões");
        exit(4);
    }

    /*
     * Aceitando conexões.
     */
    namelen = sizeof(client);

    if ((ns = accept(s, (struct sockaddr *)&client, &namelen)) == -1)
    {
        perror("Erro ao aceitar as conexões");
        exit(5);
    }

    /*
     * Recebe a mensagem pelo socket conectado.
     */
    if (recv(ns, &msg, sizeof(struct message), 0) == -1)
    {
        perror("Erro ao receber a mensagem");
        exit(6);
    }

    switch (msg.instruction) {
        case 1:
            saveUser(msg.u);
            break;
    }

    strcpy(buf_server,"Mensagem recebida:");


    /*
     * Envia a mensagem para o cliente.
     */
    if (send(ns, buf_server, sizeof(buf_server), 0) < 0)
    {
        perror("Erro ao enviar a mensagem");
        exit(7);
    }
    
}

void checkArguments(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Uso correto: %s port\n", argv[0]);
        exit(1);
    }
}