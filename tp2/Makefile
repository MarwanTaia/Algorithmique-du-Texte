CC = gcc

CPPFLAGS = -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=500 -D_FORTIFY_SOURCE=2

CFLAGS = -std=c18 -Wpedantic -Wall -Wextra -Wconversion -Wwrite-strings\
         -Werror -fstack-protector-all -fpie -O3 -g

LDFLAGS = -Wl,-z,relro,-z,now -pie

OBJECTS = singleStringText_generator.o wordList_generator.o demo.o

PROGS = singleStringText_generator wordList_generator demo

all: $(PROGS) 

singleStringText_generator: singleStringText_generator.o
	$(CC) $(LDFLAGS) -o $@ $^

wordList_generator: wordList_generator.o
	$(CC) $(LDFLAGS) -o $@ $^

demo: demo.o
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	$(RM) $(OBJECTS) $(PROGS)