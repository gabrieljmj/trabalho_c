#include <sys/socket.h>
#include "user.h"

#define ACTION_CREATE 1
#define ACTION_GET 2
#define ACTION_UPDATE 3
#define ACTION_DELETE 4
#define RESPONSE 5
#define ERROR_RESPONSE 6

struct message {
	int action;
	struct user u;
};

void sendMessage(struct message msg, int s) {
	if (send(s, &msg, sizeof(struct message), 0) < 0) {
        perror("Erro ao enviar uma mensagem ");
        exit(5);
    }
}

void receiveMessage(struct message *msg, int s) {
	if (recv(s, *msg, sizeof(struct message), 0) < 0) {
        perror("Erro ao receber a mensagem");
        exit(6);
    }
}