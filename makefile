CC = gcc
CFLAGS = -Wall -Wextra -O2

SRCS = programaTrab.c \
	buffer/buffer.c \
	CREATE/CREATE.c \
	utils/utils.c \
	Estacao/Estacao.c \
	arquivo/arquivo.c \
	ParEstacoes/ParEstacoes.c \
	DELETE/DELETE.c \
	UPDATE/UPDATE.c \
	SetNomes/SetNomes.c \
	SELECT/SELECT.c \
	lista/lista.c \
	Header/Header.c \
	no/no.c \
	INSERT/INSERT.c

OBJS = $(SRCS:.c=.o)

EXEC = programaTrab

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: programaTrab
	./programaTrab

truncate: clean
	truncate -s 0 estacoes.bin

v:
	valgrind --leak-check=yes ./programaTrab

clean:
	rm -f $(OBJS) $(EXEC)
c:
	./programaTrab < create.in

d:
	./programaTrab < delete.in

u:
	./programaTrab < update.in

i: 
	./programaTrab < insert.in

s: 
	./programaTrab < select.in
