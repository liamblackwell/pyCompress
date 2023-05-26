# Makefile to build C bits n bobs :)

PROJECT	= compress
HEADERS	= $(PROJECT).h
OBJ	= compress.o

C11	= cc -std=c11
CFLAGS	= -Wall -Werror -pedantic

$(PROJECT) : $(OBJ)
	$(C11) $(CFLAGS) -o $(PROJECT) $(OBJ)

compress.o : compress.c $(HEADERS)
		$(C11) $(CFLAGS) -c compress.c

clean:
	rm -f $(PROJECT) $(OBJ)

