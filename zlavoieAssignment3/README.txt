ls
 - There are three sub-directories for each part of the program. PartA is for the sieve, PartB is the unthreaded cylinder heat dispersion program.
 
PartA:
	Test Run:
	     ./main 10

	Results:
		PRIME: 2
		PRIME: 3
		PRIME: 5
		PRIME: 7
		PRIME: 11
		PRIME: 13
		PRIME: 17
		PRIME: 19
		PRIME: 23
		PRIME: 29

PartB:
	Results for PartB are written in a textfile called "results.txt" in the
	subdirectory called PartB. For a cylinder greater than 100 x 100 (such
	as 1000x1000), the cylinder takes a long time to calculate on a single thread.
