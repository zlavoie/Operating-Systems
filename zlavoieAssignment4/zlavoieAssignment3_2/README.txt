From Assignment 3:

PartC:
	Results for PartC are written in a textfile called "results.txt" in the
	subdirectory called PartC.

	EX1:
		Single Threaded Cylinder Run:
		     ./main 10 10 20 .5 .1 .025 .001 10000
		     
		Part C Run:
		     ./main 10 10 20 .5 .1 .025 .001 10000 50

	Analysis of EX1:
	- When computing the 10x10 cylinder example with a max of 50 threads, PartC ran at about 0.098911 seconds and the single threaded program ran at about 0.007193 seconds. On a smaller cylinder with more threads, the thread overhead actually decreases efficiency. If we were to alter EX1 input for Part C Run to: ./main 10 10 20 .5 .1 .025 .001 10000 5 (where there are less max threads in the run), then Part C would run quicker at about 0.070116 seconds.

But, if we are computing a larger cylinder, the multiple threads allow us to compute the cylinder noticeably faster than PartC as seen in EX2.

      EX2:
		Single Threaded Cylinder Run: ./main 100 100 20 .5 .1 .025 .001 10000

		Part C Run: ./main 100 100 20 .5 .1 .025 .001 10000 50
      
      Analysis of EX2:
      - For a larger cylinder (100x100) the large amount of threads is helpful in computing the cylinder quicker. Part C runs at about 57.7222 seconds, while the single threaded cylinder runs at about 165.749 seconds. Using more threads can be beneficial in larger computing but can be counter productive when performing computation on relatively small numbers.
