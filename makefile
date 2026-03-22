CC = gcc
CFLAGS = -Wall -Wextra -O2

SRCS = programaTrab.c \
	buffer/buffer.c \
	CREATE/CREATE.c \
	utils/utils.c \
	Estacao/Estacao.c \
	arquivo/arquivo.c \
	ParEstacoes/ParEstacoes.c


OBJS = $(SRCS:.c=.o)

EXEC = programaTrab

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: all programaTrab
	./programaTrab

truncate: clean
	truncate -s 0 estacoes.bin

v:
	valgrind --leak-check=yes ./programaTrab

clean:
	rm -f $(OBJS) $(EXEC)