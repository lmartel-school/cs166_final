COMPILER=clang++
OPTIONS=-g -Wall

all: clean test

clean:
	rm -f *.o splay_tree_test rope_test

test: splay_tree.o rope.o
	$(COMPILER) $(OPTIONS) splay_tree.o rope.o test.cpp -o test

splay_tree.o:
	$(COMPILER) $(OPTIONS) -c splay_tree.cpp

rope.o:
	$(COMPILER) $(OPTIONS) -c rope.cpp
