# Makefile to build C bits n bobs :)

PROJECT	= compress
HEADERS	= $(PROJECT).h
OBJ	= compress.o videoInfo.o
LIB = compress.so

C11	= gcc -std=c11
CFLAGS	= -Wall -Werror -pedantic -fpic
LIBFLAGS = -shared


$(PROJECT): $(OBJ)
	$(C11) $(CFLAGS) -o $(PROJECT) $(OBJ)

compress.o: compress.c videoInfo.c $(HEADERS)
		$(C11) $(CFLAGS) -c compress.c
		
videoInfo.o: videoInfo.c $(HEADERS)
		$(C11) $(CFLAGS) -c videoInfo.c

$(LIB): $(OBJ)
		$(C11) $(LIBFLAGS) -o $@ $^
clean:
	rm -f $(OBJ)