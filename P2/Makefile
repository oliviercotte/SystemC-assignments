include Makefile.h

ASSN = P2

MAKE = make
SYSTEMC_HOME = /usr/local/systemc-2.3.1

SRCDIR = orig
ARCHIVE = $(SRCDIR)/srcar.a
SCFILES = JPEG.cpp main.cpp
SCHEADERS = JPEG.h Block.h #my_timed_fifo.h

EXE = jpeg

GOLD_DIR = golden
GOLD_PICTURE = gold.jpg

GOLD_LOG1 = gold1.log
GOLD_LOG2 = gold2.log
GOLD_LOG3 = gold3.log
GOLD_LOG4 = gold4.log

OUT_PICTURE = test.jpg
TEST1 = test1.log
TEST2 = test2.log
TEST3 = test3.log
TEST4 = test4.log


test: test1 test2 test3 test4

archive: 
	$(MAKE) -C $(SRCDIR) archive
	
test1: $(SCFILES) archive
	@#rm -f test.jpg
	@echo "TEST1:"
	g++ -DFIFO_SIZE=1 -g -I. -I$(SRCDIR) -I$(SYSTEMC_HOME)/include -L. -L$(SYSTEMC_HOME)/lib-linux64 -o $(EXE) $(SCFILES) $(ARCHIVE) -lsystemc -lm
	./$(EXE) | tee $(TEST1)
	@#compare generated jpeg with gold
	@cmp -s $(GOLD_DIR)/$(GOLD_PICTURE) $(OUT_PICTURE); \
    RETVAL=$$?; \
    if [ $$RETVAL -eq 0 ]; then \
            cmp -s $(GOLD_DIR)/$(GOLD_LOG1) $(TEST1); \
			RETVAL=$$?; \
			if [ $$RETVAL -eq 0 ]; then \
				echo "    TEST1 PASSED"; \
			else \
				echo "    $(TEST1) does not match golden..... TEST1 FAILED"; \
			fi\
    else \
            echo "    Output jpeg file does not match golden...... TEST1 FAILED"; \
    fi

test2: $(SCFILES) archive
	@#rm -f test.jpg
	@echo "TEST2:"
	g++ -DFIFO_SIZE=16 -g -I. -I$(SRCDIR) -I$(SYSTEMC_HOME)/include -L. -L$(SYSTEMC_HOME)/lib-linux64 -o $(EXE) $(SCFILES) $(ARCHIVE) -lsystemc -lm
	./$(EXE) | tee $(TEST2)
	@#compare generated jpeg with gold
	@cmp -s $(GOLD_DIR)/$(GOLD_PICTURE) $(OUT_PICTURE); \
    RETVAL=$$?; \
    if [ $$RETVAL -eq 0 ]; then \
            cmp -s $(GOLD_DIR)/$(GOLD_LOG2) $(TEST2); \
			RETVAL=$$?; \
			if [ $$RETVAL -eq 0 ]; then \
				echo "    TEST2 PASSED"; \
			else \
				echo "    $(TEST2) does not match golden....... TEST2 FAILED"; \
			fi\
    else \
            echo "    Output jpeg file does not match golden....... TEST2 FAILED"; \
    fi
	
test3: $(SCFILES) archive
	@#rm -f test.jpg
	@echo "TEST3:"
	g++ -DFIFO_SIZE=64 -g -I. -I$(SRCDIR) -I$(SYSTEMC_HOME)/include -L. -L$(SYSTEMC_HOME)/lib-linux64 -o $(EXE) $(SCFILES) $(ARCHIVE) -lsystemc -lm
	./$(EXE) | tee $(TEST3)
	@#compare generated jpeg with gold
	@cmp -s $(GOLD_DIR)/$(GOLD_PICTURE) $(OUT_PICTURE); \
    RETVAL=$$?; \
    if [ $$RETVAL -eq 0 ]; then \
            cmp -s $(GOLD_DIR)/$(GOLD_LOG3) $(TEST3); \
			RETVAL=$$?; \
			if [ $$RETVAL -eq 0 ]; then \
				echo "    TEST3 PASSED"; \
			else \
				echo "    $(TEST3) does not match golden....... TEST3 FAILED"; \
			fi\
    else \
            echo "    Output jpeg file does not match golden....... TEST3 FAILED"; \
    fi
	
test4: $(SCFILES) archive
	@#rm -f test.jpg
	@echo "TEST4:"
	g++ -DFIFO_SIZE=100 -g -I. -I$(SRCDIR) -I$(SYSTEMC_HOME)/include -L. -L$(SYSTEMC_HOME)/lib-linux64 -o $(EXE) $(SCFILES) $(ARCHIVE) -lsystemc -lm
	./$(EXE) | tee $(TEST4)
	@#compare generated jpeg with gold
	@cmp -s $(GOLD_DIR)/$(GOLD_PICTURE) $(OUT_PICTURE); \
    RETVAL=$$?; \
    if [ $$RETVAL -eq 0 ]; then \
            cmp -s $(GOLD_DIR)/$(GOLD_LOG4) $(TEST4); \
			RETVAL=$$?; \
			if [ $$RETVAL -eq 0 ]; then \
				echo "    TEST4 PASSED"; \
			else \
				echo "    $(TEST4) does not match golden....... TEST4 FAILED"; \
			fi\
    else \
            echo "    Output jpeg file does not match golden....... TEST4 FAILED"; \
    fi
	
run:	all
	./$(EXE)
	
submit: clean
	mkdir -p $(ASSN)-$(ID)
	cp -r $(SRCDIR) $(SCFILES) $(SCHEADERS) $(GOLD_DIR) $(HEADERS) Makefile Makefile.h $(ASSN)-$(ID)
	gtar cvfz $(ASSN)-$(ID).tar.gz $(ASSN)-$(ID)
	
clean:#careful with this commands as it force-recursively removes all files ending with ~ and .o
	$(MAKE) -C $(SRCDIR) clean
	rm -rf *~ *.o $(EXE) *.log $(ASSN)-$(ID) *.tar *.tar.gz *.jpg
