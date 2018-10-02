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

## Running
```bash
$ ./bin/server 3000
```

```bash
$ ./bin/app localhost 3000
```