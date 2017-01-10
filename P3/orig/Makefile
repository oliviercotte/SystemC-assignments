#--------------VERY IMPORTANT-------------------------
#-------DO NOT MODIFY THIS MAKEFILE---------------------

SOURCES = chendct.c huffencode.c HuffEncode_aux.c quantize.c Quantize_aux.c ReadBmp_aux.c zigzag.c Zigzag_aux.c
HEADERS = ChenDCT_aux.h HuffEncode_aux.h ReadBmp_aux.h Quantize_aux.h Zigzag_aux.h

EXE = jpeg-orig

GOLD_DIR = ../golden
GOLD_PICTURE = gold.jpg

OUT_PICTURE = test.jpg

OBJECTS = ${SOURCES:.c=.o}
ARCHIVE = srcar.a
# clear out all suffixes
.SUFFIXES:
# list only those we use
.SUFFIXES: .o .c

# define a suffix rule for .c -> .o
.c.o :
	gcc -c $<

%.o: %.c $(HEADERS)
	gcc -c $<


archive: $(OBJECTS)
	ar rvs $(ARCHIVE) $(OBJECTS)
	
# target to build the C-reference
run: archive
	gcc -c main.c
	gcc -o $(EXE) main.o $(ARCHIVE)
	./$(EXE)
	cmp -s $(GOLD_DIR)/$(GOLD_PICTURE) $(OUT_PICTURE)
	
clean:#careful with this commands as it force-recursively removes all files ending with ~ and .o
	rm -rf *~ *.o $(EXE) *.jpg *.a
