TO RUN ALL THREE PARTS OF THE ASSIGNMENT2:
       cd PartC -> make clean -> make -> ./main

To view the assignment2 in its respective parts, there are folders that show the variations of each. PartC puts all three parts together in one program.

**Note: in each subdirectory (PartA,PartB,PartC) there is a folder called "test" that holds a Hello World program to run tests with. **

PART A:
     The first part entered into the command line is a file that needs to be searched for in the PATH enviornment variable if it does not begin with './' '../' or '../', otherwise it is an absolute path and try to open the file being specified. If the file cannot open then it does not exist.

     Examples Tried In Command Line:
     	      Zoe's Shell> ./test/main
	      	    Hello World!

	      Zoe's Shell> ls
	      	    main.cpp main.o main makefile

PART B:
     If the command line is terminated with an '&", such as: "cat ./main &", then the parent will NOT wait for the child to finish before providing another prompt to the user for the shell.
     
     Examples Tried In Command Line:
     	      Zoe's Shell>./main &
	      	    Zoe's Shell > Zoe's Shell >

PART C:
     This part deals with redirection and pipes. Part C is a build on from Part A and Part B and can do everything in this one program. For redirections, part C can handle a single '<' or '>' in the command line, redirecting stdin or stdout, or it can handle having both '<' and '>' in the command line.

     Examples Tried In Command Line:
     	      Zoe's Shell > ./main > out.txt
	      Zoe's Shell > cat out.txt
	      	    Hello World!
	      Zoe's Shell >

	      ---- Next Example ---

	      Zoe's Shell > cat out.txt
	      Zoe's Shell > cat out.txt | wc -l
	      	    1
	      Zoe's Shell >
     	    
