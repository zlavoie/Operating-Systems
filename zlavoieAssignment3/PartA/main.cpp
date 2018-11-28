#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <main.h>
#include <pthread.h>
#include <queue>
#include <mutex>
#include <sstream>
#include <cstdlib>
#include <semaphore.h> 
#include <string>
#include <list>

using namespace std;
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

void Display(list <int> list1);
void *setPrimes(void* prime);

struct item{
int N; //Number of primes wanted: decrement to 0 as primes found
sem_t se; //for queue waiting
wqueue <int> Nums;  //queue of numbers 2....N for numberz entered
};

int main(int argc, char* argv[])
{
	int n; //Take in user input from command line of how many primes to print
    stringstream num;
    num<<argv[1];
    num>>n;
	//END USER INPUT DECISION
	
	item p; //Declare Struct
	p.N = n; //store n in struct
	p.Nums.push(1);
	sem_init(&p.se,0,1);//Initialize Sempahore
	
	pthread_t t;//declare pthread
	pthread_create(&t,NULL,setPrimes,(void *)&p); //Create 2 Thread
	
	
   	int i=2; //count for boundless numbers to put in queue
    while(true){ //create numbers boundlessly
		sem_wait(&p.se);
		  p.Nums.push(i);	
		sem_post(&p.se);
		i++;
    }
	return 0;
}

void *setPrimes(void* p) {
	item* s = (item *) p;
	if((s->N) == 0){
		//Have found all primes requested, quit program
		cout<<"\n";
		exit(0);
	}
	sem_wait(&s->se);
	int check1 = s->Nums.size();
	sem_post(&s->se);
	if(check1 != 0){
	  	int prime; //declare prime for thread
	  //Have not found all primes, get next prime
		sem_wait(&s->se); 
		
		prime = s->Nums.front();
		if(prime==1){
			s->Nums.pop();
			prime = s->Nums.front();
			s->Nums.pop();
		}else{
		s->Nums.pop();
		}
		cout<<"PRIME: "<<prime<<endl;
		sem_post(&s->se);
	        
		item nq; //need new struct to create new queueS
		nq.N=(s->N)-1; //Update num of primes needed
		sem_init(&nq.se,0,1); //initialze new semaphore
		pthread_t t2; //declare new thread for new prime
		pthread_create(&t2,NULL,setPrimes,(void *)&nq); //create new prime thread	

	while(true){
		sem_wait(&nq.se);
	   	   bool check = s->Nums.size();
		  sem_post(&nq.se);
		  while(check!=0){
		    sem_wait(&nq.se);
			int x=s->Nums.front();
		/*cout<<"X: "<<x<<endl;
		cout<<"Back: "<<s->Nums.back()<<endl;
		cout<<"Prime: "<<prime<<endl;*/
			sem_post(&nq.se);
	       	  if((x%prime)==0){ //Not a prime, remove from queue
				sem_wait(&nq.se);
				s->Nums.pop(); //discard number, do not send on
				//cout<<"SIZE After Remove: "<<s->Nums.size()<<endl;
				sem_post(&nq.se);
		  }else{
			    sem_wait(&nq.se);
				 //cout<<"REMOVE: "<<s->Nums.front()<<endl;
			    nq.Nums.push(s->Nums.front());//send number on to next stack to be filteredF
			    s->Nums.pop();//discard from main stack
				 // cout<<"SIZE: "<<s->Nums.size()<<endl;
			    sem_post(&nq.se);
		  }
		}////End s->Nums.size() is not empty
		  sem_destroy(&nq.se);
	}//End outter while
	}//End s->Nums.size() is not empty
}//end method
