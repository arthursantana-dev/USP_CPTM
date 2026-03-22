all: programaTrab.c
	gcc -o programaTrab programaTrab.c

run: programaTrab
	./programaTrab

truncate: clean
	truncate -s 0 estacoes.bin

v:
	valgrind --leak-check=yes ./programaTrab

clean:
	rm -f programaTrab
	rm -f *.o