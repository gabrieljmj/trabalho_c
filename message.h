#include <sys/socket.h>
#include <stdbool.h>
#include "user.h"

enum message_codes {
    CODE_CREATE = 1,
    CODE_GET,
    CODE_GET_ALL,
    CODE_UPDATE,
    CODE_DELETE,
    CODE_SUCCESS_RESPONSE,
    CODE_ERROR_CREATING_RESPONSE,
    CODE_ERROR_UPDATING_RESPONSE,
    CODE_ERROR_DELETING_RESPONSE,
    CODE_ERROR_USER_ALREADY_EXISTS_RESPONSE,
    CODE_ERROR_USER_NOT_FOUND_RESPONSE,
    CODE_GET_ALL_USERS_RESPONSE,
    CODE_CLOSE_CONNECTION,
    CODE_NO_USER_FOUND
};

struct message {
    int code;
    struct user u;
    int unique;
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