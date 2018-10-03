all: bin/app bin/server

bin/app: app.c message.h user.h
	gcc app.c -o bin/app

bin/server: server.c message.h user.h
	gcc server.c -o bin/server -ltinfo -lncurses