COMPILER=clang++
OPTIONS=-g -Wall

all: splay_tree_test

splay_tree_test: splay_tree.o
	$(COMPILER) $(OPTIONS) splay_tree.o splay_tree_test.cpp -o splay_tree_test

splay_tree.o:
	$(COMPILER) $(OPTIONS) -c splay_tree.cpp
