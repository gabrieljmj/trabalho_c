#include <sys/socket.h>
#include "user.h"

struct message {
	int instruction;
	struct user u;
};

void sendMessage(struct message msg, int s) {
	if (send(s, &msg, sizeof(struct message), 0) < 0) {
        perror("Erro ao enviar uma mensagem ");
        exit(5);
    }
}