Assignment 4:

Part A:
     Go to the sub-directory PartA in this folder. Execute the code by running ./main in the Linux command line. The program has two arrays: one array for the name of the rascals and one array for the name of the toppings. There is one more array for four semaphores. Each semaphore works to block/allow a rascal or the teacher to run. S[] 0 - 2 are for the rascals and the last semaphore S[3] is for Miss Crabtree. The semaphores are all initialized to 0 to start them as blocked except for Miss Crabtree's semaphore which is set to 1 so it runs. Miss Crabtree puts a topping on the table, and then a method decides which rascal needs the topping. Depending on which rascal needs the topping, the corresponding semaphore to the rascal is unblocked.

Part B:
     Go to the sub-directory PartB in this folder. Execute the code by running ./main in the Linux command line. The program will ask if you want to change the number of Readers that are executing in the program (the default is currently set to 5). If you answer Y then the program will prompt you for a number to set for the readers, otherwise 5 readers will run. The program will simulate the multiple readers reading from the database while one writer is writing.

Part C:
     Go to the sub-directory PartC in this folder. Execute the code by running ./main in the Linux command line. The program creates one thread -- the doctor -- and 13 patients that enter the office one at a time. If the doctor is sleeping they wake the doctor up and are examined. If the doctor is seeing someone, the patient waits in the waiting room if the waiting room is not full, otherwise the patient leaves. Whenever the doctor is done seeing patients and there is no one else in the waiting room, the doctor sleeps.
     

***** The multi-threaded cylinder proble has its own separate README.txt in the sub-directory zlavoieAssignment3_2. *****
