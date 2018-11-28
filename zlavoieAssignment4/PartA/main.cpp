
#include <pthread.h> 
#include <semaphore.h> 
#include <stdio.h> 
#include <unistd.h>
#include <iostream>
#include <stdlib.h>     /* srand, rand */ 
#include<bits/stdc++.h> 
using namespace std; 

string rascal[] = { "Spanky","Alfalfa","Darla" }; //array of the three rascals
string topping[] = {"Jimmies","Ice Cream","Chocolate Sauce"};
//array of the toppings that MissCrabtree is putting on the table one at a time, randomly choosing.

//int N=2;
sem_t S[3]; //Array of semaphores corresponding to the three rascals in the rascal array (0=Spanky, 1=Alfalfa, 2=Darla). The last semaphore S[3] is for MissCrabtree

void eat(int topping);
int MissCrabtree(int N);
void *Rascals(void *rascal);

void eat(int topping) 
{ //Rascal eats topping if they need it
    sem_post(&S[topping]); 
    cout<<rascal[topping]<<" is eating. \n"<<endl; 
	//choose random time that the rascal is eating it
	int t = rand() % 5;
    sem_wait(&S[topping]); 
	sleep(t);
} 
  
int MissCrabtree(int N) 
{ //Miss Crabtree puts down a random topping: she has an unlimited amount of each
    sem_wait(&S[N]); 
	int t = rand() % 3;
	cout<<"Miss Crabtree Put Down: "<< topping[t]<<"."<<endl;
    sem_post(&S[N]); 
	return t;
} 

void *Rascals(void *rascNum) 
{ //MissCrabtree gets a topping, depending on which rascal needs it, that rascal eats it for a certain amount of time
	int N = *((int *)rascNum);
	bool continuous = true;
    while(continuous) { 
		N=3;
		//Get topping
		int topping = MissCrabtree(N); 
		//Decide which rascal needs topping and eat for certain amount of time
        eat(topping); 
    } 
} 
  
int main() 
{ 
  int N = 2;
    int i; 
    pthread_t thread_id[N]; 
  
    for (i = 0; i < N;i++) {
        sem_init(&S[i], 0, 0);
	}//Block 3 rascals
	N=3;
	sem_init(&S[N],0,1); //Allow Miss Crabtree to start

		
    for (i = 0; i < N; i++) { // create rascal processes 
        pthread_create(&thread_id[i], NULL, Rascals,(void *)&N);
    } 
  
    for (i = 0; i < N; i++) 
        pthread_join(thread_id[i], NULL); 
} 