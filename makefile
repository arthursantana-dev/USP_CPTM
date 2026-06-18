CC = gcc
CFLAGS = -Wall -Wextra -O2

SRCS = programaTrab.c \
	Estacao/Estacao.c \
	Header/Header.c \
	EstruturasAuxiliares/SetNomes.c \
	EstruturasAuxiliares/ParEstacoes.c \
	BTREE/btree_busca.c \
	BTREE/btree_insercao.c \
	BTREE/btree_remocao.c \
	BTREE/BTREE.c \
	IO/IO.c \
	IO/misc.c \
	IO/parser.c \
	CRUD/crud_create.c \
	CRUD/crud_delete.c \
	CRUD/crud_insert.c \
	CRUD/crud_update.c \
	CRUD/crud_select.c \
	CRUD/crud_order_by.c

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

vv: 
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./programaTrab < v.in

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

ci:
	./programaTrab < create_index.in

di:
	./programaTrab < delete_i.in

ii:
	./programaTrab < insert_i.in

oi: 
	./programaTrab < order_by.in

zip:
	zip -r t.zip . -x "*.git*" -x "testes/*" -x "Makefile" -x "*.zip" -x "*.bin" -x "*.csv" -x "*.in" 
