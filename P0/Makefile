include Makefile.h

MAKE = make
CPP = g++ -Wno-deprecated
SYSTEMC_HOME = /CMC/tools/systemc

SRCDIR = orig
ARCHIVE = $(SRCDIR)/srcar.a

SYSTEMC_HOME = /CMC/tools/systemc
SOURCES = producer.c consumer.c
SCFILES = pc.cpp main.cpp
SCHEADERS = pc.h

EXE = pc

GOLD_DIR = golden
GOLD_PICTURE = gold.jpg
GOLD_LOG1 = gold1.log
GOLD_LOG2 = gold2.log

TEST1 = test1.log
TEST2 = test2.log


TEST1_OPTS = -DFIFO_SIZE=1

TEST2_OPTS = -DFIFO_SIZE=2


test: test1 test2

archive: 
	$(MAKE) -C $(SRCDIR) archive

test1: $(SCFILES) archive
	@echo "TEST1:"
	@$(CPP) $(TEST1_OPTS) -g -I. -I$(SRCDIR) -I$(SYSTEMC_HOME)/include -L. -L$(SYSTEMC_HOME)/lib-linux64 -o $(EXE) $(SCFILES) $(ARCHIVE) -lsystemc -lm
	@./$(EXE) | tee $(TEST1)
	@cmp -s $(GOLD_DIR)/$(GOLD_LOG1) $(TEST1); \
	RETVAL=$$?; \
	if [ $$RETVAL -eq 0 ]; then \
		echo "    $(TEST1) PASSED"; \
	else \
		echo "    $(TEST1) FAILED"; \
	fi\
    

test2: $(SCFILES) archive
	@echo "TEST2:"
	@$(CPP) $(TEST2_OPTS) -g -I. -I$(SRCDIR) -I$(SYSTEMC_HOME)/include -L. -L$(SYSTEMC_HOME)/lib-linux64 -o $(EXE) $(SCFILES) $(ARCHIVE) -lsystemc -lm
	@./$(EXE) | tee $(TEST2)
	@cmp -s $(GOLD_DIR)/$(GOLD_LOG2) $(TEST2); \
	RETVAL=$$?; \
	if [ $$RETVAL -eq 0 ]; then \
		echo "    $(TEST2) PASSED"; \
	else \
		echo "    $(TEST2) FAILED"; \
	fi\
	
run:	all
	./$(EXE)
	
submit: clean
	mkdir -p $(ASSN)-$(ID)
	cp -r $(SRCDIR) $(SCFILES) $(SCHEADERS) $(GOLD_DIR) $(HEADERS) Makefile Makefile.h $(ASSN)-$(ID)
	gtar cvfz $(ASSN)-$(ID).tar.gz $(ASSN)-$(ID)
	
clean:#careful with this commands as it force-recursively removes all files ending with ~ and .o
	$(MAKE) -C $(SRCDIR) clean
	rm -rf *~ *.o $(EXE) *.log *.tar *.tar.gz
