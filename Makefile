CC= gcc-4.9
CFLAGS= -g -O3
LIBS=
UNAME = $(shell uname)
ifneq ($(UNAME),Darwin)
  LIBS += -lrt -lm
endif
EXEC = boat
MODULES = token.o grammer.o load.o array_buffer_helpers.o main.o 

main: $(MODULES)
	$(CC) $(CFLAGS) $(LIBS) $(MODULES) -o $(EXEC)

main.o: lib/main.c
	$(CC) $(CFLAGS) -c lib/main.c $(LIBS) -o main.o

token.o: lib/token.c lib/token.h
	$(CC) $(CFLAGS) -c lib/token.c $(LIBS) -o token.o

grammer.o: lib/grammer.c lib/grammer.h
	$(CC) $(CFLAGS) -c lib/grammer.c $(LIBS) -o grammer.o

load.o: lib/load.c lib/load.h
	$(CC) $(CFLAGS) -c lib/load.c $(LIBS) -o load.o

array_buffer_helpers.o: lib/array_buffer_helpers.c lib/array_buffer_helpers.h
	$(CC) $(CFLAGS) -c lib/array_buffer_helpers.c $(LIBS) -o array_buffer_helpers.o

clean:
	rm -r *.o *.dSYM *.swp $(EXEC) 2> /dev/null
