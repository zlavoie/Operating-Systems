The executable file is "main" to run the program.

The program creates a multi-dimensional array of numbers (0-99) and puts those numbers in nodes. The nodes are all connected (right,left,up,down) within the limitations of a grid. So the nodes in the last column do not point right, instead those pointers are NULL, for example. Once the grid is set up with all the nodes connected, I generate a random number by filling an array with the numbers 0-99 and shuffling the array. I originally used rand() but found this to be pseudo-random and consistently gave me close to the same "order" of "random" numbers each time I ran the program.

This program follows the structure of the HelloWorld program on the course website. Main.cpp holds my code and main is the executable file. There is also a makefile included in the Zip.
