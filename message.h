#include <sys/socket.h>
#include "user.h"

#define CODE_CREATE 1
#define CODE_GET 2
#define CODE_UPDATE 3
#define CODE_DELETE 4
#define CODE_SUCCESS_RESPONSE 5
#define CODE_ERROR_CREATING_RESPONSE 6
#define CODE_ERROR_UPDATING_RESPONSE 7
#define CODE_ERROR_DELETING_RESPONSE 8
#define CODE_ERROR_USER_ALREADY_EXISTS_RESPONSE 9
#define CODE_ERROR_USER_NOT_FOUND_RESPONSE 10

struct message {
	int code;
	struct user u;
	int extra;
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