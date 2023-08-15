CC=clang
CFLAGS= -Wall -Werror -Wpedantic -Wextra
LFLAGS=-lm $(shell pkg-config --libs gmp)
all: keygen encrypt decrypt
keygen: keygen.o
	$(CC) -o keygen keygen.o numtheory.o randstate.o rsa.o $(LFLAGS)
keygen.o:  keygen.c numtheory.c randstate.c randstate.h numtheory.h rsa.h rsa.c
	$(CC) $(CFLAGS) -c keygen.c numtheory.c randstate.c numtheory.h randstate.h rsa.h rsa.c
encrypt: encrypt.o
	$(CC) -o encrypt encrypt.o numtheory.o randstate.o  rsa.o $(LFLAGS)
encrypt.o: encrypt.c numtheory.c randstate.c randstate.h numtheory.h rsa.h rsa.c
	$(CC) $(CFLAGS) -c encrypt.c numtheory.c randstate.c numtheory.h randstate.h rsa.h rsa.c
decrypt: decrypt.o
	$(CC) -o decrypt decrypt.o numtheory.o randstate.o rsa.o $(LFLAGS)
decrypt.o: decrypt.c numtheory.c randstate.c randstate.h numtheory.h rsa.h rsa.c
	$(CC) $(CFLAGS) -c decrypt.c numtheory.c randstate.c numtheory.h randstate.h rsa.h rsa.c
clean:
	rm -f keygen.o decrypt.o encrypt.o  numtheory.o randstate.o rsa.o keygen decrypt encrypt randstate.h.gch rsa.h.gch numtheory.h.gch 
format:
	clang-format -i -style=file *.[ch]


