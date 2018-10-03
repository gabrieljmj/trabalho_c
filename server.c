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
//#include "sockets_conn_functions.h"

#define FILE_NAME "users.dat"

FILE *fdoc;

void openFile(char *file, char *opts) {
    if ((fdoc = fopen(file, opts)) == NULL) {
        printf("\nO arquivo nao pode ser aberto!! \n");
        getch();
        exit(1);
    }
}

void saveUser(struct user u, int ns) {
    struct message res;
    openFile(FILE_NAME, "ab+");
    fseek(fdoc, 0L , SEEK_END);

    if(fwrite(&u, sizeof(struct user), 1, fdoc) != 1) {
        res.code = CODE_ERROR_CREATING_RESPONSE;
        printf("\n Erro de gravacao!");
        return;
    } else {
        res.code = CODE_SUCCESS_RESPONSE;
        res.u = u;
    }

    fclose(fdoc);
    sendMessage(res, ns);
}

int searchUser(struct user u) {
    int found = 0, pos = 0;
    struct user tempUser;

    openFile(FILE_NAME, "rb");

    rewind(fdoc);

    while (!feof(fdoc) && !found) {
        fread(&tempUser, sizeof(struct user), 1, fdoc);

        if (!feof(fdoc)) {
            if (strcmp(u.username, tempUser.username) == 0) {
                if (tempUser.status == 0) {
                    pos = -2;
                }

                found = 1;
            } else{
                pos++;
            }
        } else{
            pos = -1;
        }
    }

    if (!found) {
        pos = -1;
    }

    fclose(fdoc);

    return pos;
}

void getUser(struct user u, int ns) {
    struct message res;
    int pos = searchUser(u);

    if (pos < 0) {
        res.code = CODE_ERROR_USER_NOT_FOUND_RESPONSE;
    } else {
        openFile(FILE_NAME, "rb+");
        res.code = CODE_SUCCESS_RESPONSE;

        fseek(fdoc, pos * sizeof(struct user), SEEK_SET);
        fread(&res.u, sizeof(struct user), 1, fdoc);
    }

    sendMessage(res, ns);

    fclose(fdoc);
}

void updateUser(struct user u, int ns) {
    int pos = searchUser(u);
    struct message res;

    if (pos < 0) {
        printf("err");
        res.code = CODE_ERROR_USER_NOT_FOUND_RESPONSE;
    } else {
        openFile(FILE_NAME, "rb+");

        fseek(fdoc, pos * sizeof(struct user), SEEK_SET);

        if (fwrite(&u, sizeof(struct user), 1, fdoc)!=1) {
            res.code = CODE_ERROR_UPDATING_RESPONSE;
            printf("\nErro na gravacao!");
            getch();
        } else {
            res.code = CODE_SUCCESS_RESPONSE;
            res.u = u;
            printf("\nAlteracao feita com sucesso!");
            getch();
        }
    }

    fclose(fdoc);
    sendMessage(res, ns);
}

void deleteUser(struct user u, int ns) {
    int pos = searchUser(u);
    struct message res;

    if (pos < 0) {
        res.code = CODE_ERROR_USER_NOT_FOUND_RESPONSE;
    } else {
        openFile(FILE_NAME, "rb+");

        fseek(fdoc, pos * sizeof(struct user), SEEK_CUR);
        fread(&u, sizeof(struct user), 1, fdoc);

        u.status = 0;

        fseek(fdoc, pos * sizeof(struct user), SEEK_SET);

        if (fwrite(&u, sizeof(struct user),1, fdoc)!=1) {
            res.code = CODE_ERROR_DELETING_RESPONSE;
            printf("\nErro na gravacao!");
        } else {
            res.code = CODE_SUCCESS_RESPONSE;
            res.u = u;
            printf("\nUsuário excluído com sucesso!");
        }
    }

    fclose(fdoc);
    sendMessage(res, ns);
}

void getAllUsers(int ns) {
    struct user tempUser;
    struct message res;
    int i, counter = 0;
      
    res.code = CODE_SUCCESS_RESPONSE;

    openFile(FILE_NAME, "rb");
      
    while (!feof(fdoc)) {
        fread(&tempUser, sizeof(struct user), 1, fdoc);
        counter++;
    }

    counter--;

    fclose(fdoc);

    openFile(FILE_NAME, "rb");

    for (i = 1; i <= counter; i++) {
        fread(&tempUser, sizeof(struct user), 1, fdoc);
        
        if (tempUser.status == 1) {
            res.u = tempUser;
            
            if (i == counter) {
              res.unique = 1;
            }
            
            sendMessage(res, ns);
        }
    }

    fclose(fdoc);
}

void closeConnection(int ns, int s) {
    close(ns);
    close(s);
    exit(0);
}

void checkArguments(int argc, char** argv);

int main(int argc, char** argv) {
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
    checkArguments(argc, argv);

    /*
     * O primeiro argumento deve ser a porta.
     */
    port = (unsigned short) atoi(argv[1]);

    /*
     * Cria o socket para aceitar conexões.
     */
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Erro ao criar o socket");
        exit(2);
    }

    /*
     * Bind the socket to the server address.
     */
    server.sin_family = AF_INET;
    server.sin_port   = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(s, (struct sockaddr *)&server, sizeof(server)) < 0) {
       perror("Erro no Bind()");
       exit(3);
    }

    if (listen(s, 1) != 0) {
        perror("Erro na espera de conexões");
        exit(4);
    }

    namelen = sizeof(client);

    if ((ns = accept(s, (struct sockaddr *)&client, &namelen)) == -1) {
        perror("Erro ao aceitar as conexões");
        exit(5);
    }

    while(1) {
        if (recv(ns, &msg, sizeof(struct message), 0) == -1) {
            perror("Erro ao receber a mensagem");
            exit(6);
        } 

        switch (msg.code) {
            case CODE_CREATE:
                saveUser(msg.u, ns);
                break;
            case CODE_GET:
                getUser(msg.u, ns);
                break;
            case CODE_UPDATE:
                updateUser(msg.u, ns);
                break;
            case CODE_DELETE:
                deleteUser(msg.u, ns);
                break;
            case CODE_GET_ALL:
                getAllUsers(ns);
                break;
            case CODE_CLOSE_CONNECTION:
                closeConnection(ns, s);
        }
    }
}

void checkArguments(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Uso correto: %s port\n", argv[0]);
        exit(1);
    }
}