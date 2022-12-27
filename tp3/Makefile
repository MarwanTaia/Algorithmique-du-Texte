CC = gcc

CPPFLAGS = -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=500 -D_FORTIFY_SOURCE=2

CFLAGS = -std=c18 -Wpedantic -Wall -Wextra -Wconversion -Wwrite-strings\
         -Werror -fstack-protector-all -fpie -O3 -g

LDFLAGS = -Wl,-z,relro,-z,now -pie

OBJECTS = genere-texte.o genere-mots.o ac-hachage.o

PROGS = genere-texte genere-mots ac-hachage

all: $(PROGS)

genere-texte: genere-texte.o
	$(CC) $(LDFLAGS) -o $@ $^

genere-mots: genere-mots.o
	$(CC) $(LDFLAGS) -o $@ $^

ac-hachage: ac-hachage.o
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	$(RM) $(OBJECTS) $(PROGS)