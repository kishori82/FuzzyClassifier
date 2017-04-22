# Specify the compiler
#CC = g++ -Wall -O3 -march=athlon 
CC = gcc -lm -Wall -O2  -g

# The files used to make the program.
HEADERS = header.h 
SOURCES = fuzzy-classify.c partition-data-first.c  mem-function-second.c  binary-mem-third.c  mem-to-classes-fourth.c  creates-trims-ID3-tree-fifth.c classify-ID3-test-sixth.c extract-rules-seventh.c  rule-generation-eight.c rand-init-network-ninth.c know-encoded-init-network-tenth.c
OBJECTS = $(SOURCES:.C=.o)
MAIN    = fuzzy-classify

# Pattern rule
%.o : %.C $(HEADERS)
	$(CC) -c  $<

# Make the source
${MAIN}: $(HEADERS) $(OBJECTS) 
	$(CC) $(OBJECTS) -o $(MAIN) $(LDLIBS)

tags: $(HEADERS) $(SOURCES)
	etags --c++ $(HEADERS) $(SOURCES) 
 
clean :
	rm -f *.o ${MAIN}

archive:
	tar cvf quorum.tar $(HEADERS) $(SOURCES) Makefile
