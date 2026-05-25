CC = gcc
CFLAGS = -Wall -Wextra -O2

SRCS = programaTrab.c \
	utils/utils.c \
	Estacao/Estacao.c \
	ParEstacoes/ParEstacoes.c \
	SetNomes/SetNomes.c \
	IO/IO.c \
	CRUD/CRUD.c

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
	./programaTrab < testes/create.in

d:
	./programaTrab < testes/delete.in

u:
	./programaTrab < testes/update.in

i: 
	./programaTrab < testes/insert.in

s: 
	./programaTrab < testes/select.in

zip:
	zip -r t.zip . -x "*.git*" -x "testes/*" -x "Makefile" -x "*.zip" -x "*.bin" -x "*.csv" 
