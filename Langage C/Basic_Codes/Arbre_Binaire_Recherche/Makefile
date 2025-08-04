CC=gcc
CCFLAGS= -Wall -Werror -g 
LIBFLAGS= 
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)
EXEC= abr

all: $(EXEC)


$(EXEC): $(OBJ)
	$(CC) $(LIBFLAGS) $^ -o $@  

%.o: %.c
	$(CC) $(CCFLAGS) -o $@ -c $<

.depend:
	$(CC) -MM $(SRC) > .depends
-include .depends

clean:
	rm -f $(OBJ) $(EXEC) 
