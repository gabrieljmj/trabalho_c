# Trabalho Redes
----------------
## Compile app.c
```bash
$ gcc app.c -o bin/app
```

## Compile app_server.c
```bash
$ gcc app_server.c -o bin/server -ltinfo -lncurses
```

## Compile via Makefile
```bash
$ make
```

## Running
### Terminal 1
```bash
$ ./bin/server 3000
```

### Terminal 2
```bash
$ ./bin/app localhost 3000
```