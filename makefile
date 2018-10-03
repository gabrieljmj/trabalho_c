all: bin/app bin/server

bin/app: app.c
	gcc app.c -o bin/app

bin/server: server.c
	gcc server.c -o bin/server -ltinfo -lncurses