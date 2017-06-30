all: LSTraversal

Block.o: Block.h Block.cpp
	g++ -c Block.cpp -o Block.o
	
Scanner.o: Scanner.h Scanner.cpp TraversalConfiguration.h
	g++ -c Scanner.cpp -o Scanner.o
	
LSTraversal: main.cpp Block.o Scanner.o TraversalConfiguration.h 
	g++ main.cpp Block.o Scanner.o -o LSTraversal

	
clean:
	rm -fr *.o LSTraversal