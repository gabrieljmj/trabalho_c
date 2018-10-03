#include <sys/socket.h>
#include <stdbool.h>
#include "user.h"

#define CODE_CREATE 1
#define CODE_GET 2
#define CODE_GET_ALL 3
#define CODE_UPDATE 4
#define CODE_DELETE 5
#define CODE_SUCCESS_RESPONSE 6
#define CODE_ERROR_CREATING_RESPONSE 7
#define CODE_ERROR_UPDATING_RESPONSE 8
#define CODE_ERROR_DELETING_RESPONSE 9
#define CODE_ERROR_USER_ALREADY_EXISTS_RESPONSE 10
#define CODE_ERROR_USER_NOT_FOUND_RESPONSE 11
#define CODE_GET_ALL_USERS_RESPONSE 12
#define CODE_CLOSE_CONNECTION 13

struct message {
    int code;
    struct user u;
    bool unique;
};

void sendMessage(struct message msg, int s) {
    if (send(s, &msg, sizeof(struct message), 0) < 0) {
        perror("Erro ao enviar uma mensagem ");
        exit(5);
    }
}

void receiveMessage(struct message *msg, int s) {
    if (recv(s, msg, sizeof(struct message), 0) < 0) {
        perror("Erro ao receber a mensagem");
        exit(6);
    }
}