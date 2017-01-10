#--------------VERY IMPORTANT-------------------------
#-------DO NOT MODIFY THIS MAKEFILE---------------------

SOURCES = producer.c consumer.c

EXE = pc-orig

OBJECTS = ${SOURCES:.c=.o}
ARCHIVE = srcar.a
# clear out all suffixes
.SUFFIXES:
# list only those we use
.SUFFIXES: .o .c

# define a suffix rule for .c -> .o
.c.o :
	gcc -c $<

%.o: %.c
	gcc -c $<


archive: $(OBJECTS)
	ar rvs $(ARCHIVE) $(OBJECTS)
	
# target to build the C-reference
run: archive
	gcc -c main.c
	gcc -o $(EXE) main.o $(ARCHIVE)
	./$(EXE)
	
clean:#careful with this commands as it force-recursively removes all files ending with ~ and .o
	rm -rf *~ *.o $(EXE) *.a
