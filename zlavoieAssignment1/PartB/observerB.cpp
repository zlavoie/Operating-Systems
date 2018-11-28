#include <string>
#include <unistd.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/time.h>

using namespace std;

int BootInfo()
{
  cout <<"------ BOOT INFO -----"<<endl;
 
  ifstream file3 ("/proc/uptime");
  if(!file3){
    cout <<"Cannot open uptime file.\n";
  }

  string nxtLine;

      getline(file3, nxtLine);
      nxtLine = nxtLine.substr(0, nxtLine.find(' '));  

file3.close();
 stringstream geek(nxtLine);
 size_t fieldWidth = 2;

 long time = 0;
 geek >> time;

 //cout<<"TIME: "<<time<<endl;;

 long days, hours, minutes, seconds;
 days = 0;
 hours = 0;
 minutes = 0;
 seconds = 0;
 
 if(time > 86400){
 days = time/86400;
 //cout <<"DAYS: "<<days<<endl;
 time = time - (days*86400);
 }
 if(time > 3600){
 hours = time/3600;
 //cout <<"HOURS: "<<hours<<endl;
 time = time - (hours*3600);
 }
 if(time > 60){
 minutes = time/60;
 //cout <<"MINUTES: "<<minutes<<endl;
 time = time - (minutes*60);
 } 
 seconds = time;
 
 cout <<"Time Since Last Reboot (DD::HH::MM::SS): "<<setfill('0')<<setw(fieldWidth)<<days<<"::"<<setfill('0')<<setw(fieldWidth)<<hours<<"::"<<setfill('0')<<setw(fieldWidth)<<minutes<<"::"<<setfill('0')<<setw(fieldWidth)<<seconds<<endl;
 cout<<" "<<endl;
}

void VersionInfo(string info){
  cout <<"-------- VERSION INFO ---------"<<endl;
  string nxtLine;
  ifstream file2 ("/proc/version");
 if(!file2){
   cout<<"Cannot open version file"<<endl;
   return;
 }
 
 while(file2){
   getline(file2, nxtLine);

   if(nxtLine.find(info) != std::string::npos){
     cout<<nxtLine<<"\n"<<endl;
     return;
   }
 }
 file2.close();
 
return;
}

void CPUInfo(string *info){
  cout <<"--------- CPU INFO ---------"<<endl;
		      
  for(int x=0; x<3;x++){
  if(info[x] != "version"){
  ifstream file ("/proc/cpuinfo");
  if(!file){
    cout <<"Cannot open cpuinfo file.\n";
    return;
  }

  string nxtLine;
  while(file){
    getline(file, nxtLine);
    
    if(nxtLine.find(info[x]) != std::string::npos){
      cout <<nxtLine<<"\n"<<endl;
      break;
    }
  }
file.close();
  }else{
  VersionInfo(info[x]);
  }}
}

//Part C
void ModeTimes(){
  ifstream filemode ("/proc/stat");

  if(!filemode){
    cout<<"Cannot open stat file"<<endl;
  }

  string nxtLine = "";
  int data[10];
  string temp= "";
  int num=0;
  int count = 0;
  string CurrentLine = "";
  
  getline(filemode, nxtLine);
  for(int y=4; y<nxtLine.length(); y++){
     CurrentLine = CurrentLine +nxtLine[y];
     temp = nxtLine[y];
     //   cout <<"Current: "<<CurrentLine<<endl;
    std::istringstream convert (CurrentLine);

    if(temp != " "){
      convert >> num;
    }
    else{
      data[count] = num;
      //        cout <<"Data "<<count<<": "<<data[count]<<endl;
      CurrentLine = "";
      count = count +1;
    }
    
    //   cout <<"temp: "<<temp<<endl;
  }
  filemode.close();
  cout<<"------ MODE TYPE ------"<<endl;
  cout<<"User: "<<(data[1])/60<<" seconds, System: "<<(data[3])/60<<" seconds, Idle: "<<(data[4])/60<<" seconds"<<endl;
  cout<<" "<<endl;
}

void DiskRequests(){
 ifstream fileDR ("/proc/diskstats");

  if(!fileDR){
    cout<<"Cannot open disk stat file"<<endl;
  }

  string nxtLine = "";
  int data[14];
  string temp= "";
  string temp2="";
  int num=0;
  int count = 0;
  string CurrentLine = "";

  while(fileDR){
    getline(fileDR, nxtLine);
    if(nxtLine.find("xvda1")!= std::string::npos){
    for(int y=1;y<nxtLine.length();y++){
     temp = nxtLine[y];
     temp2= nxtLine[y-1];
    if(temp != " "){
       CurrentLine = CurrentLine +nxtLine[y];
       std::istringstream convert (CurrentLine);
       convert >> num;
    }
    else if(temp2!= " " && temp == " "){
      data[count] = num;
      //    cout <<"Data "<<count<<": "<<data[count]<<endl;
      CurrentLine = "";
      count = count +1;
    }//end else
    }//end for
    }
    }//end while

  fileDR.close();
  cout<<"------ Disk Requests ------"<<endl;
  cout<<"Number of Reads: "<<data[3]<<endl;
  cout<<"Number of Writes: "<<data[7]<<endl;
  cout<<"Total Number of Completed Disk Requests: "<<data[3]+data[7]<<"\n"<<endl;
} //end method

void ContextSwitches(){
 ifstream fileCS ("/proc/stat");

  if(!fileCS){
    cout<<"Cannot open stat file"<<endl;
  }

   string nxtLine = "";
   int num=0;
   string word = "";

  while(fileCS){
    getline(fileCS, nxtLine);
    if(nxtLine.find("ctxt")!= std::string::npos){
    for(int y=6;y<nxtLine.length();y++){
       word = word +nxtLine[y];
        std::istringstream toNum (word);
       toNum >> num;
    }//end for
    }
    }//end while
  fileCS.close();
  cout<<"------ CONTEXT SWITHCES ------"<<endl;
  cout<<"Number of Context Swithces: "<<num<<"\n"<<endl; 
}

void NumProcesses(){
 string keyword = "processes";
 ifstream fileP ("/proc/stat");
  if(!fileP){
    cout<<"Cannot open stat file"<<endl;
  }

   string nxtLine = "";
   int num=0;
   string CurrentLine = "";

   while(fileP){
    getline(fileP, nxtLine);
    if(nxtLine.find(keyword)!= std::string::npos){
    for(int y=keyword.length();y<nxtLine.length();y++){
       CurrentLine = CurrentLine +nxtLine[y];
        std::istringstream convert (CurrentLine);
       convert >> num;
    }//end for
    }
    }//end while
   fileP.close();
  cout<<"------ PROCESSES SINCE LAST REBOOT ------"<<endl;
  cout<<"Number of Processes: "<<num<<"\n"<<endl;
}

void MemoryConfig(){
  cout<<"------ MEMORY CONFIGURED ------"<<endl;
       
 ifstream fileMem ("/proc/meminfo");
  if(!fileMem){
    cout<<"Cannot open stat file"<<endl;
  }

   string nxtLine = "";
   int num=0;
   string CurrentLine = "";

    while(fileMem){
    getline(fileMem, nxtLine);
    if(nxtLine.find("MemTotal")!= std::string::npos){
    for(int y=9;y<nxtLine.length();y++){
       CurrentLine = CurrentLine +nxtLine[y];
        std::istringstream convert (CurrentLine);
       convert >> num;
    }//end for
      cout<<"MemTotal: "<<num<<" kB"<<endl;
      CurrentLine = "";
      num = 0;
    }
     if(nxtLine.find("MemFree")!= std::string::npos){
    for(int y=8;y<nxtLine.length();y++){
       CurrentLine = CurrentLine +nxtLine[y];
        std::istringstream convert (CurrentLine);
       convert >> num;
    }//end for
    cout<<"MemFree: "<<num<<" kB \n"<<endl;
    }
    }//end while
    fileMem.close();
}

void LoadAvg(){      
 ifstream Avg ("/proc/loadavg");
  if(!Avg){
    cout<<"Cannot open loadavg file"<<endl;
  }

   string nxtLine = "";
 
    while(Avg){
    getline(Avg, nxtLine);
    cout<<nxtLine<<endl;
    }
     Avg.close();
}

int main(int argc, char* argv[]){

  //Create Greeting To System Status Report
  cout<<" "<<endl;
  cout<<"                            Welcome to the Status Report       \n "<<endl;

  ifstream KernelF ("/proc/sys/kernel/hostname");
 
  struct timeval now;
  gettimeofday(&now, NULL);
  //Report Time of Status Report and Day
  cout << "Status report as of : " << ctime((time_t *) &now.tv_sec) <<endl;

  string s;
  KernelF >> s;
  //Report Machine Name
  cout << "Machine name: " << s <<"\n"<< endl;

  KernelF.close();

  //Start Part B
 string  Info[6];
  
  Info[0]="model";
  Info[1]="model name";
  Info[2]="version";

  cout<<"                         PART B               "<<endl;
  cout<<"******************************************************"<<endl;
  //Finds Model and Type  
  CPUInfo(Info);
  //Gets Time of Last Boot
  BootInfo();


  return 0;
}
