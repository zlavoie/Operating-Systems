#include <pthread.h> 
#include <semaphore.h> 
#include <stdio.h> 
#include <unistd.h>
#include <cstdint>
#include <iostream>
#include <stdlib.h>  
#include<bits/stdc++.h> 
using namespace std; 
void *Doctor(void *arg);
void *Patient(void *arg);

int DoctorNum=1;//Number of Doctors
int Patients=13;//Number of Patients
int chairs = 10;//Number of Chairs in the Waiting Room
int Max=chairs;

typedef struct {
    sem_t empty,used;
	int replenish[13]={1,2,3,4,5,6,7,8,9,10,11,12,13};
} controls;
 
//Shared Struct
controls t;
 
template <typename T> class wqueue{
    queue <T> WrapQ;
    pthread_mutex_t m_mutex;
    pthread_cond_t m_condv; 

  public:
    wqueue() {
        pthread_mutex_init(&m_mutex, NULL);
        pthread_cond_init(&m_condv, NULL);
    }
    ~wqueue() {
        pthread_mutex_destroy(&m_mutex);
        pthread_cond_destroy(&m_condv);
    }
	
    void push(T item) {
        pthread_mutex_lock(&m_mutex);
        WrapQ.push(item);
        pthread_cond_signal(&m_condv);
        pthread_mutex_unlock(&m_mutex);
    }
	
    T pop() {
        pthread_mutex_lock(&m_mutex);
        while (WrapQ.size() == 0) {
            pthread_cond_wait(&m_condv, &m_mutex);
        }
        T item = WrapQ.front();
        WrapQ.pop();
        pthread_mutex_unlock(&m_mutex);
        return item;
    }
	
    int size() {
        pthread_mutex_lock(&m_mutex);
        int size = WrapQ.size();
        pthread_mutex_unlock(&m_mutex);
        return size;
    }
	
	T front(){
		 pthread_mutex_lock(&m_mutex);
        int front = WrapQ.front();
        pthread_mutex_unlock(&m_mutex);
        return front;
	}
};

wqueue <int> s;
wqueue <int> q;
int main()
{
    pthread_t dr, patient;
    sem_init(&t.empty, 0, 10);
    sem_init(&t.used, 0, 0);
    pthread_create(&dr, NULL, Doctor, NULL);
		
	//Create Patients: Producers
    for (int index = 1; index <= Patients; index++)
    {
		s.push(index);
       pthread_create(&patient, NULL, Patient, (void*) &index);
    }
	
	 for (int i = 1; i <= Patients; i++)
	 {
	pthread_join(patient,NULL);
	 }
}

void *Patient(void *arg)
{
	int index = (intptr_t)arg;
	while(true) 
    {
		   sem_wait(&t.empty);
		cout<<"Waiting Room Current Capacity: "<<q.size()<<endl;
		if(q.size()!=10 && s.size()!=0){
			cout<<"Patient "<<s.front()<<" entered the waiting room."<<endl;
			q.push(s.front());//Take front of customer queue and put into waiting room
			s.pop();
			cout<<"Patient "<<q.front()<<" is waiting to see the doctor."<<endl;
		}else if(q.size()==10){
			cout<<"Waiting Room is Full With "<<q.size()<<" Patients! Patient is Leaving..."<<endl;
		}if(s.size()==0){
			//sleep(10);
			for(int i=0;i<13;i++){
				s.push(t.replenish[i]);
			}
		}
		//Queue is FuLL
		sem_post(&t.used);
	}//Finish While Loop
}//Finish Patient Method
 
void *Doctor(void *arg)
{
     while (true)
    {
        sem_wait(&t.used);
    	if(q.size()==0){ //No patients in the queue, dr is sleeping
			cout<<"\nSleeping Doctor: No Patients. \n"<<endl;
			sleep(2); //Sleeping
		}else{
			int x = q.front();
			cout<<"Examining Patient: "<<q.front()<<" Removing From Queue."<<endl;
			q.pop();
		}
        sem_post(&t.empty);
    }
}