include Makefile.h

ASSN = P5

MAKE = make
CPP = g++ -Wno-deprecated
SYSTEMC_HOME = /usr/local/systemc-2.3.1

SRCDIR = orig
ARCHIVE = $(SRCDIR)/srcar.a
SCFILES = Bus.cpp os.cpp CPU.cpp HW_Quant.cpp main.cpp
SCHEADERS = Bus.h AddressMap.h HW_Quant.h CPU.h System.h Block.h os.h error.h

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

TEST1_OPTS = -DFIFO_SIZE=1 -DREAD_PRIORITY=5 -DDCT_PRIORITY=4 \
		-DZIGZAG_PRIORITY=2 -DHUFFMAN_PRIORITY=1

TEST2_OPTS = -DFIFO_SIZE=2 -DREAD_PRIORITY=3 -DDCT_PRIORITY=4 \
		-DZIGZAG_PRIORITY=1 -DHUFFMAN_PRIORITY=2

TEST3_OPTS = -DFIFO_SIZE=3 -DREAD_PRIORITY=2 -DDCT_PRIORITY=2 \
		-DZIGZAG_PRIORITY=1 -DHUFFMAN_PRIORITY=3

TEST4_OPTS = -DFIFO_SIZE=4 -DREAD_PRIORITY=2 -DDCT_PRIORITY=3 \
		-DZIGZAG_PRIORITY=5 -DHUFFMAN_PRIORITY=4

test: test1 test2 test3 test4

archive: 
	$(MAKE) -C $(SRCDIR) archive
	
test1: $(SCFILES) archive
	@#rm -f test.jpg
	@echo "TEST1:"
	$(CPP) $(TEST1_OPTS) -g -I. -I$(SRCDIR) -I$(SYSTEMC_HOME)/include -L. -L$(SYSTEMC_HOME)/lib-linux64 -o $(EXE) $(SCFILES) $(ARCHIVE) -lsystemc -lm
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
	$(CPP) $(TEST2_OPTS) -g -I. -I$(SRCDIR) -I$(SYSTEMC_HOME)/include -L. -L$(SYSTEMC_HOME)/lib-linux64 -o $(EXE) $(SCFILES) $(ARCHIVE) -lsystemc -lm
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
	$(CPP) $(TEST3_OPTS) -g -I. -I$(SRCDIR) -I$(SYSTEMC_HOME)/include -L. -L$(SYSTEMC_HOME)/lib-linux64 -o $(EXE) $(SCFILES) $(ARCHIVE) -lsystemc -lm
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
	$(CPP) $(TEST4_OPTS) -g -I. -I$(SRCDIR) -I$(SYSTEMC_HOME)/include -L. -L$(SYSTEMC_HOME)/lib-linux64 -o $(EXE) $(SCFILES) $(ARCHIVE) -lsystemc -lm
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
