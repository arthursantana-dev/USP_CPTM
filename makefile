all: programaTrab.c
	gcc -o programaTrab programaTrab.c

run: programaTrab
	./programaTrab

clean:
	rm -f programaTrab
	rm -f *.o