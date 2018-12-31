Assignment Zero: C++ Warmup 
 
CS314 Operating Systems 
 Write a program in C++ that implements a smart memory manager for a graph that has no memory leaks. Assemble a 10x10 grid of nodes connected by edges.  
 
 Use a C++ class or struct to represent a node.  Use C++ pointers to represent an edge.  Edges are bidirectional so you will need two pointers (one from node A to node B and one from node B to node A) to reperesent an edge. A node will have two (corner), three (grid edge, non-corner), or four edges (noncorner, non-grid edge) depending on its location in the grid. Delete a random node.  If the deletion results in a node that has no edges going to it, then delete that node as welll. 
 
 For example if the node below and to the right of the upper left corner node of the grid have been deleted, then the upper left corner node of the grid has no edges and should be deleted. The program terminates when there are no more nodes to delete. NOTE: You must use the new operator to construct and assemble nodes in the grid and you must use the delete operator to eliminate nodes in the grid. NOTE: You can read about memory leak detection schemes here > https://gcc.gnu.org/onlinedocs/libstdc++/manual/debug.html NOTE: I'll be using valgrind to check for memory leaks with this command:  





The executable file is "main" to run the program.

The program creates a multi-dimensional array of numbers (0-99) and puts those numbers in nodes. The nodes are all connected (right,left,up,down) within the limitations of a grid. So the nodes in the last column do not point right, instead those pointers are NULL, for example. Once the grid is set up with all the nodes connected, I generate a random number by filling an array with the numbers 0-99 and shuffling the array. I originally used rand() but found this to be pseudo-random and consistently gave me close to the same "order" of "random" numbers each time I ran the program.

This program follows the structure of the HelloWorld program on the course website. Main.cpp holds my code and main is the executable file. There is also a makefile included in the Zip.
