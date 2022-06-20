SH_SRC = common.c scanner.c parser.c command.c command_grp.c command_line.c shell.c
SS_SRC = common.c util.c shell_server.c
SC_SRC = common.c util.c shell_client.c

all: shell ss sc
	echo 'done'

shell: $(SH_SRC)
	gcc $(SH_SRC) -o shell

ss: $(SS_SRC)
	gcc $(SS_SRC) -o ss

sc: $(SC_SRC)
	gcc $(SC_SRC) -o sc
 