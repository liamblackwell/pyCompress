# Makefile to build C bits n bobs :)

#Project file variables
PROJECT	= compress
HEADERS	= $(PROJECT).h
OBJ	= compress.o videoInfo.o
TOBJ = testCompress.o
TEST = testCompress
LIB = compress.so

#Compilation variables
C11	= gcc -std=c11
CFLAGS	= -Wall -Werror -pedantic -fpic -O3  #Compile with all warnings to be careful, and -fpic for library comp, and optimization flag O3
LIBFLAGS = -shared

#Rules for making - to use on cli with 'make [rule]'
all: $(LIB) $(PROJECT) $(TEST) clean
exe: $(PROJECT) clean
test: $(TEST) clean
lib: $(LIB) clean

$(TEST): $(TOBJ)
	$(C11) $(CFLAGS) -o $(TEST) $(TOBJ)

$(LIB): $(OBJ)
	$(C11) $(LIBFLAGS) -o $(LIB) $(OBJ) 

$(PROJECT): $(OBJ)
	$(C11) $(CFLAGS) -o $(PROJECT) $(OBJ)

compress.o: compress.c videoInfo.c $(HEADERS)
		$(C11) $(CFLAGS) -c compress.c
		
videoInfo.o: videoInfo.c $(HEADERS)
		$(C11) $(CFLAGS) -c videoInfo.c

testCompress.o: testCompress.c
		$(C11) $(CFLAGS) -c testCompress.c

clean:
	rm -f $(OBJ) $(TOBJ)