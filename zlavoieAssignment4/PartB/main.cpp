#include <pthread.h> 
#include <semaphore.h> 
#include <stdio.h> 
#include <unistd.h>
#include <cstdint>
#include <iostream>
#include <stdlib.h>     /* srand, rand */ 
#include<bits/stdc++.h> 
 
int NumWriters=1;     /* total number of producers - 1 */
int NumReaders=5;       /* total number of consumers - 5 */

using namespace std; 

typedef struct {
    sem_t SharedData;  
    sem_t wr;
} controls;
 
//Shared Struct
controls s;
 
//Shared Number of Processes Reading
int ReadingProcesses = 0;
 
void *Writer(void *arg);
void *Read(void *arg);

int countW, countR;
 
int write_request = 0;
 
int main()
{
	string Response;
	cout<<"Current Number of Readers is set at: "<<NumReaders<<". Would you like to change the number (Y/N)?"<<endl;
	cin>>Response;
	if(Response=="Y"){
		cout<<"Change the number of readers to: ";
		cin>>NumReaders;
	}
   //writer/read threads
    pthread_t tWrite, tRead;
 
    /* creates semaphores */
    sem_init(&s.SharedData, 0, 1);
    sem_init(&s.wr, 0, 1);
     
    /* loop control */
    int index;
    /* creates write thread */
    for (index = 0; index < NumWriters; index++)
    {
       // Create 1 writer
       pthread_create(&tWrite, NULL, Writer, (void*)index);
    }
		//Create 5 readers
    for (index = 0; index < NumReaders; index++)
    {
       pthread_create(&tRead, NULL, Read, (void*)index);
    }
	
    /* destroy semaphore */
    sem_destroy(&s.SharedData);
    sem_destroy(&s.wr);
    pthread_exit(NULL);
}

void *Read(void *arg)
{
   int index = (intptr_t)arg;
    while(true)
    {
        sem_wait(&s.SharedData);
		  cout<<"Read thread Number: "<<index<<endl;
        ReadingProcesses++;
        if (ReadingProcesses == 1)
        {
           sem_wait(&s.wr);
		}
           sem_post(&s.SharedData);
           cout<<"Read database"<<endl;
           sem_wait(&s.SharedData);
           ReadingProcesses--;
        if(ReadingProcesses == 0)
        {
			//Open The Writer since no more readers
              sem_post(&s.wr);
		}
              sem_post(&s.SharedData);
    		   sleep(1);
	}
}
 
void *Writer(void *arg)
{
    int index = (intptr_t)arg;
     while (true)
    {
        sem_wait(&s.wr);
	cout<<"\n"<<endl;
    	cout<<"Write thread: "<<index<<endl;
		cout<<"Create Data"<<endl;
        cout<<"Write to database \n"<<endl;
        sem_post(&s.wr);
		sleep(3);
    }
}
