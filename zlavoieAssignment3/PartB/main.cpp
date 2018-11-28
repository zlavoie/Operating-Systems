//resultf to write to file is causing errors in compiling when transfering resultf
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <main.h>
#include <list>
#include <iterator>
#include <sstream>
#include <cstdlib>
#include <algorithm> 
#include <string>
#include <ctime>
#include <fstream>
#include <stdio.h>
#include "main.h"

using namespace std;
//H is for the height, C is for Circumference, N is for Number of random cells (iterations), and X is for the prev iteration temp;
int H,C,N,X,MAX,NumThreads,cells,startCell,FLOPS; //Sent in through Command line
double Delta,C0,C1,C2;  //Sent in through Command line
double tinit; //Sent in through Command line
list <string> arr; // N/A
double cyl [1000][1000]; //Constantly changing values of the cylinder after each iteration
double origcyl [1000][1000]; // Does not get manipulated until next iteration where the updated cylinder (cyl[][]) becomes old cylinder (origcyl[][])


void getFLOPS();
void ExecTime(double begin); //prints out Execution Time
void Iterations(); //Recalcs values for each iteration of cylinder
void SplitCylWork(); //Helper thread in Iterations that divides the cylinder for different threads to recalc values
void PrintCyl(); // Prints out cyl[][]
void AvgTemp(); //Prints out average temp of the whole cylinder (cyl[][])
void MaxDelta();//Prints out max delta of whole cylinder(cyl[][])
void SetRandVals(bool test); //For testing purposes
void TINIT(); //Sets initial values of cylinder (cyl[][])
double round(double d); //Helper method to round the double values in the cylinder (cyl[][]) grid
string int2string(int value);//Helper method to convert integer to String
bool Convergence();//Helper method checking for convergence
double getNeighbor(bool firstCol, bool firstRow,bool lastRow, bool lastCol,int h,int c );//Calculates the value of all the neighbors for a certain cell in cylinder (oldcyl[][])
double getDiag(bool firstCol, bool firstRow, bool lastRow, bool lastCol,int h,int c);//Calculates the value of all the diagonals for a certain cell in cylinder (oldcyl[][])
void InitializePrev(); //Resets the oldcyl[][] to reflect the last iteration of cyl[][]
bool FixedNum(int h,int c);
double doubleRand();
void SetCells(int N);//Picks certain cells that do NOT change during iterations --- Constant
int FindRand(int max); //Radnomizer for picking cells
void Initialize(int part);//Initializes variable for initial run of the simulation

int main(int argc, char* argv[])
{ 
//Set up clock and set up file to write results to 
double begin = (double)clock() / CLOCKS_PER_SEC;

bool test = true;
//Set args from command line
 cout<<"arg1: "<<argv[1]<<" arg2: "<<argv[2]<<endl;
//Get H, C , and tinit, from command line
stringstream in;
in<<argv[1];
in >> H; //Set Height
in.clear();
in<<argv[2];
in >> C; //Set Circumference
in.clear();
in << argv[3];
in >> tinit; //Set Default value for cells: tinit
    in.clear();
in << argv[8];
in >> MAX; //Set MAX iterations
    in.clear();
in << argv[4];
in >> C0; //Set C0
    in.clear();
    in << argv[5];
in >> C1; //Set C1
    in.clear();
in << argv[6];
in >> C2; //Set C2
    in.clear();
    in << argv[7];
in >> Delta; //Set Delta
    in.clear();
cout<<"\nHeight: "<<H<<" Circ: "<<C<<endl;
cout<<"tinit: "<<tinit<<" C0,C1,C2: "<<C0<<", "<<C1<<", "<<C2<<endl;
cout<<"Delta: "<<Delta<<" Max: "<<MAX<<"\n"<<endl;
    
//Initialize(1);
//Sets all default cells to temp specified: tinit
TINIT();

//Decides how many random cells get set
if(test==false){
Initialize(2);
SetCells(N);   
SetRandVals(test);
}else{
//Find N Random Cell
N = 3; //Hard Coded for Testing
SetCells(N);  
}   
//Iteration
 Iterations();
    
//print out all information needed to file    
//-------------------------------------------------------------- 
//Cylinder Cells Print Out
    //cout<<"Print Cylinder"<<endl;
      PrintCyl();
//MAX DELTA
   // cout<<"Max Delta"<<endl;
    MaxDelta();
//AVG TEMP AMONGST CELLS 
    //cout<<"Average Temp"<<endl;
    AvgTemp();
//ESTIMATE NUMBER OF FLOPS
  getFLOPS();
	
//EXECUTION TIME
    //cout<<"Exec Time"<<endl;
    ExecTime(begin);
 return 0;
}

//--------------------------------------------------------------
bool FixedNum(int h,int c){
    string temp3=int2string(h)+int2string(c);
                std::list<string>::iterator findIter = std::find(arr.begin(), arr.end(), temp3);
                
                if(findIter!=arr.end()){
                   // cout<<"FIXED IS TRUE"<<endl;
                    return true;
                }else{
                   // cout<<"FIXED IS FALSE"<<endl;
                    return false;
                }
}

double getNeighbor(bool firstCol, bool firstRow,bool lastRow, bool lastCol,int h,int c){
	//cout<<"H: "<<h<<" C:"<<c<<endl;
   double neighbor;
    if(firstRow){
		//cout<<"First Row"<<endl;
		//First Row correct
    if(firstCol){
    neighbor = (origcyl[h][C-1] + origcyl[h+1][c] + origcyl[h][c+1])*C1;
		FLOPS++;
    }else if(lastCol){
		//cout<<"Last Col"<<endl;
		  neighbor = (origcyl[h][0] + origcyl[h][c-1] + origcyl[h+1][c]+origcyl[h-1][c])*C1;
		FLOPS++;
	}else{
    neighbor = (origcyl[h][c-1] + origcyl[h+1][c] + origcyl[h][c+1])*C1; 
		FLOPS++;
    }
    }else if(lastRow){
		//cout<<"Last Row"<<endl;
		//LastRow Correct
     if(firstCol){
       neighbor = (origcyl[h][C-1]+ origcyl[h][c+1]+origcyl[h-1][c])*C1; 
		 FLOPS++;
     }else if(lastCol){
		  neighbor = (origcyl[h][0] + origcyl[h][c-1]+origcyl[h-1][c])*C1;
		FLOPS++;
	}  else{
      neighbor = (origcyl[h][c-1] +origcyl[h][c+1]+origcyl[h-1][c])*C1;   
		 FLOPS++;
     } 
    }else{
		//cout<<"Other Cell"<<endl;
		//Any other cell correct
		    if(firstCol){
       neighbor = (origcyl[h][C-1] + origcyl[h-1][c] + origcyl[h][c+1]+origcyl[h+1][c])*C1; 
		 FLOPS++;
     }else if(lastCol){
		  neighbor = (origcyl[h][0] + origcyl[h][c-1] + origcyl[h+1][c]+origcyl[h-1][c])*C1;
		FLOPS++;
	}  else{
      neighbor = (origcyl[h][c-1] + origcyl[h-1][c] + origcyl[h][c+1]+origcyl[h+1][c])*C1;   
		 FLOPS++;
     } 
	}
    return neighbor;
}

double getDiag(bool firstCol, bool firstRow, bool lastRow, bool lastCol,int h,int c){
    //RACE CONDITION: accessing origcyl -- we do have an orig cyl that contains data being used to process info
    double diag;
    //sem_wait(&s);
    if(firstRow){
    if(firstCol){
    diag = (origcyl[h+1][C-1] + origcyl[h+1][c+1])*C2;
		FLOPS++;
    }else if(lastCol){
		 diag = (origcyl[h+1][0] + origcyl[h+1][c-1]+origcyl[h-1][c-1]+origcyl[h-1][0])*C2;
		FLOPS++;
	}else{
      diag = (origcyl[h+1][c-1] + origcyl[h+1][c+1])*C2; 
		FLOPS++;
    }}
	else if(lastRow){
	if(firstCol){
        diag = (origcyl[h-1][C-1]+origcyl[h-1][c+1])*C2;
		FLOPS++;
    }else if(lastCol){
		 diag = (origcyl[h-1][c-1] +origcyl[h-1][0])*C2;
		FLOPS++;
	}else{
	diag = (origcyl[h-1][c-1] +origcyl[h-1][c+1])*C2;
		FLOPS++;	
	}
	} 
		else{
		if(firstCol){
    diag = (origcyl[h+1][C-1] + origcyl[h+1][c+1]+origcyl[h-1][C-1]+origcyl[h-1][c+1])*C2;
		FLOPS++;
    }else if(lastCol){
		 diag = (origcyl[h+1][0] + origcyl[h+1][c-1]+origcyl[h-1][c-1]+origcyl[h-1][0])*C2;
		FLOPS++;
}else{
      diag=(origcyl[h+1][c-1] + origcyl[h+1][c+1]+origcyl[h-1][c-1]+origcyl[h-1][c+1])*C2;
		FLOPS++;
			
	}}
    //sem_post(&s);
    return diag;
}

bool Convergence(){
  bool allConverge;
    for(int a=0;a<H;a++){
        for(int b=0;b<C;b++){
            double difference = origcyl[a][b]-cyl[a][b];
			FLOPS++;
            //cout<<"ITERATION: "<<i<<endl;
				if(difference<=Delta){
                allConverge = true;
            }else{
               allConverge = false;
				break;
            } 
        }
    }
	return allConverge;
}

string int2string(int value) {
    char buffer[20]; // Max num of digits for 64 bit number
    sprintf(buffer,"%d", value);
    return string(buffer);
}

double round(double d) 
{ 
    double rounded;
   // cout<<"INTEGER: "<<d<<endl;
    ostringstream strs;
    strs << d;
    string str = strs.str();
    for(int x=0;x<str.length();x++){
        if(str[x]=='.'){
            str=str.substr(0,x+3);
            break;
        }
    }
    stringstream convert(str); // stringstream used for the conversion initialized with the contents of Text

if ( !(convert >> rounded) )//give the value to Result using the characters in the string
    rounded = 0;//if that fails set Result to 0
    //cout<<"ROUNDED: "<<rounded<<endl;
    return rounded;
}

void TINIT(){
    //Sets all default cells to temp specified: tinit
 for(int h=0;h<H;h++){
     for(int c=0;c<C;c++){
         cyl[h][c] = tinit;
     }
 }
}

void SetRandVals(bool test){
    for (int i=3;i<N;i++){ 
int randH,randC;

randH = FindRand(H);
randC = FindRand(C);   
string tempS=int2string(randH)+int2string(randC);
 std::list<string>::iterator findIter = std::find(arr.begin(), arr.end(), tempS);
    while(findIter!=arr.end()){
        randH = FindRand(H);
        randC = FindRand(C);   
        tempS=int2string(randH)+int2string(randC);
         std::list<string>::iterator findIter = std::find(arr.begin(), arr.end(), tempS); 
    }
    
int temp = cyl[randH][randC]; //find temp in random square
//reset temp to be lower/higher than temp already stored there
    if(test==false){
    int HighLow = FindRand(2);
    if(HighLow == 0){//Set Temperature to higher
    int HighLow = temp;
    while(HighLow<=temp){
        HighLow = doubleRand();
    }
    cyl[randH][randC] = HighLow; //set temp to higher
}else{ //Set Temperature to lower
HighLow = temp;
    while(HighLow>=temp){
        HighLow = doubleRand();
    }
    cyl[randH][randC] = HighLow; //set temp to lower
}
}else{//End testing check
     cyl[randH][randC] = 100;   
    }
    
} //End setting random cell values
}

void MaxDelta(){
    ofstream resultf;
        resultf.open("results.txt",fstream::app);
    double max = 0;
            for(int h=0;h<H;h++){
        for(int c=0;c<C;c++){
            if(h==0&&c==0){
                max = origcyl[h][c]-cyl[h][c];
            }else{
                if(origcyl[h][c]-cyl[h][c] > max){
                    max = origcyl[h][c]-cyl[h][c];
                }
            }
            }
        }
    cout<<"Maximum Delta Across Cells:"<<max<<" C"<<endl;
    resultf << "Maximum Delta Across Cells: "<<max<<" C"<<endl;
    resultf.close();
}

void AvgTemp(){
    ofstream resultf;
    resultf.open("results.txt",fstream::app);
    double average = 0;
    for(int h=0;h<H;h++){
        for(int c=0;c<C;c++){
            average = average+cyl[h][c];
            }
        }
    average = round(average/(H*C));
	FLOPS++;
    resultf << "\nAverage Temp: "<<average<<" C"<<endl;
    cout<<"Average Temp: "<<average<<" C"<<endl;
    resultf.close();
}

void PrintCyl(){
    ofstream resultf;
    resultf.open("results.txt",fstream::app);
	cout<<"Cylinder: \n";
        resultf << "Cylinder: \n";
      for(int h=0;h<H;h++){
        for(int c=0;c<C;c++){
            resultf << cyl[h][c] << " ";
			cout << cyl[h][c] << " ";
            }
		  	cout<<"\n";
            resultf << "\n";
        }
	cout<<"\n"<<endl;
    resultf.close();
}

void Iterations(){
    for(int i=0;i<MAX;i++){ //Max iterations of the simulation
        //Reset Original Cylinder -- this is the copy of the cylinder cells we will use to do the calculations and then we will save the updated data in cyl[][] which is the updated and chaing values
        InitializePrev();
		SplitCylWork();
	
		bool done = Convergence();
        if(done==true){
        cout<<"Convergence at iteration: "<<i<<endl;
        
			break;
        }
		
    }//end max iteration for loop 
}//end method

void SplitCylWork(){
	  for(int h=0;h<H;h++){
            for(int c=0;c<C;c++){
			double newTemp=0;
            double neighbor=0;
            double diag=0;
            bool fixed = FixedNum(h, c); 
                if(fixed == false){
                    
                if(h==0){//have to consider boundary case where position cyclinder[x][0] is adjacent to position [x][C-1]
                   newTemp = round(newTemp+((origcyl[h][c])*C0));
           		FLOPS++;
                //have to consider no D0,N0,nor D1 for weighted average
                   if(c==0){
                       neighbor = getNeighbor(true,true,false,false,h,c);
                      diag =  getDiag(true,true,false,false,h,c);
                   }else if(c==C-1){
				   neighbor = getNeighbor(false,true,false,true,h,c);
                   diag =  getDiag(false,true,false,true,h,c);
				}else{
                       neighbor = getNeighbor(false,true,false,false,h,c);
                   diag =  getDiag(false,true,false,false,h,c);
                   }
                   newTemp = round((newTemp + neighbor + diag)); // New Temperature
					FLOPS++;
                   cyl[h][c] = newTemp;
               }//END FIRST ROW SPECIAL CASE
					else if(h==H-1){
				//h==H-1
					newTemp = round(newTemp+(origcyl[h][c])*C0);
					FLOPS++;
                //have to consider no D0,N0,nor D1 for weighted average
                   if(c==0){
                      neighbor = getNeighbor(true,false,true,false,h,c);
                      diag =  getDiag(true,false,true,false,h,c);
                   }else if(c==C-1){
					   neighbor = getNeighbor(false,false,true,true,h,c);
                   diag =  getDiag(false,false,true,true,h,c);
				   }else{
                      neighbor = getNeighbor(false,false,true,false,h,c);
                    diag = getDiag(false,false,true,false,h,c);
				   }
                   newTemp = round((newTemp + neighbor + diag)); //New Temperature
					FLOPS++;
                    cyl[h][c] = newTemp;       
				}//END LAST ROW SPECIAL CASE
					else if(h<H-1){
                   newTemp = round(newTemp+(origcyl[h][c])*C0);
					FLOPS++;
                //have to consider no D0,N0,nor D1 for weighted average
                   if(c==0){
                neighbor = getNeighbor(true,false,false,false,h,c);
                      diag =  getDiag(true,false,false,false,h,c);
                   }else if(c==C-1){
					   neighbor = getNeighbor(false,false,false,true,h,c);
                   diag =  getDiag(false,false,false,true,h,c);
				   }else{
                      neighbor = getNeighbor(false,false,false,false,h,c);
                    diag = getDiag(false,false,false,false,h,c);
				   }
                   newTemp = round((newTemp + neighbor + diag)); //New Temperature
					FLOPS++;
                    cyl[h][c] = newTemp;          
                }
                }//end fixed boolean
            }//End array inner for loop for Col
        }//End outt array for loop for Row  
}

void ExecTime(double begin){
    ofstream resultf;
    resultf.open("results.txt",fstream::app);
    double CPUtime =((double)clock() / CLOCKS_PER_SEC)-begin;
    cout<<"Total Execution Time of Simulation: "<<CPUtime<<" clock ticks"<<endl;
   resultf << "Total Execution Time of Simulation: " << CPUtime << " clock ticks"<<endl;
    resultf.close();
}

void Initialize(int part){
    if(part == 1){
         cout<<"Choose the height of the cylinder:"<<endl;
         cin>>H;
         cout<<"Choose the circumference of the cylinder:"<<endl;
         cin>>C;
            cout<<"Choose default temperature value for cells:"<<endl;
         cin>>tinit;
                cout<<"Choose max iteration value for cells:"<<endl;
         cin>>MAX;
         cout<<"\nYour cylinder has the height of: "<<H<<" and a circumference of: "<<C<<endl;
         cout<<"Default temperature value is set to: "<<tinit<<"\n"<<endl;
    }
}

int FindRand(int max){
    int pos;
    pos = rand() % max;
    return pos;
}

double doubleRand(){
    double f = (double)rand() / 100;
    return -100 + f * (100 - (-100));
}

void SetCells(int N){
cyl[H/2][C/2] = 40;
arr.push_back(int2string(H/2)+int2string(C/2));
cyl[H/4][C/4] = 0;
arr.push_back(int2string(H/4)+int2string(C/4));
cyl[H-3][C-3] = 0;  
arr.push_back(int2string(H-3)+int2string(C-3));
}

void InitializePrev(){
	//cout<<"Cylinder: "<<endl;
           for(int h=0;h<H;h++){
            for(int c=0;c<C;c++){
            origcyl[h][c] = cyl[h][c];
        //  cout<<cyl[h][c]<<" ";
            }
		  // cout<<" "<<endl;
        }
}

void getFLOPS(){
	ofstream resultf;
    resultf.open("results.txt",fstream::app);
    cout<<"Number of Estimated FLOPS: "<<FLOPS<<endl;
   resultf << "Number of Estimated FLOPS: "<<FLOPS<<endl;
    resultf.close();
}