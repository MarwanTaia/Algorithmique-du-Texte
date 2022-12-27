CC = gcc

CPPFLAGS = -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=500 -D_FORTIFY_SOURCE=2

CFLAGS = -std=c18 -Wpedantic -Wall -Wextra -Wconversion -Wwrite-strings\
         -Werror -fstack-protector-all -fpie -O3 -g

LDFLAGS = -Wl,-z,relro,-z,now -pie

OBJECTS = trie.o

PROGS = trie

all: $(PROGS) 

$(PROGS): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $(PROGS)

clean:
	$(RM) $(OBJECTS) $(PROGS)