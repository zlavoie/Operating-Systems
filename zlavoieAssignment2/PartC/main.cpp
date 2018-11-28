        #include "main.h"

        using namespace std;

        //Returns Number of words in a string by separating the line with a delimiter that is sent in
        int NumOfStrings(string input,char delim) {
        int count = 1;
           for (int x=0;x<input.length()+1;x++) 
           { 
               if (input[x] == delim) 
               { 
                   count = count +1;
               } 
           }
            return count;
                }

        //Splits the word on a space until we reach the number word we want (designated by find)
        string SplitWord(string input, int find) 
        { 
           int count = 0;
           string word = ""; 
           for (int x=0;x<input.length()+1;x++) 
           { 
               if (input[x] == ' ') 
               { 
                   count = count +1;
                   if(find==count){
                       return word;
                   } 
                   word = ""; 
               } 
               else
               { 
                   word = word + input[x]; 
               }
           }
            return word;
        } 

        //Converts to cstring
        char* LineToCstring(string input, int NumWord,bool FoundPipe) {
            if(FoundPipe == false){
            string conversion = SplitWord(input,NumWord);
            char * cstring = new char [conversion.length()+1];
            strcpy (cstring, conversion.c_str());
            return cstring;
            }

            string conversion = SplitWord(input,NumWord);
            char * cstring = new char [conversion.length()+1];
            strcpy (cstring, conversion.c_str());
            return cstring;   

            }

        //Finds next directory in the PATH variable list
        string NextDirectory(string FullPath,int count){
        //Search Through Directories
              string directory = "";
              int NumCol = 0;
               for(int a=0;a<FullPath.length();a++){
            if(FullPath[a]!= ':'){
              directory = directory + FullPath[a];
            }else{
              NumCol=NumCol+1;
              if(NumCol!=count){
                directory = "";
              }else{
                break;}
            }
              }
              return directory;
        }

        //FInds the Full Path Name for the Parameter being sent in
        string FullPathName(char* Path) {
            string FullPath = "";

            if(Path[0]=='/' || (Path[0]=='.')&&(Path[1]=='/') || (Path[0]=='.')&&(Path[1]=='.')&&(Path[2]=='/')){
                FullPath = Path;
                ifstream files (FullPath.c_str());
                  if(!files){
                      cout<<"Cannot open absolute path file"<<endl;
                      FullPath = "";
                  }
            //Already Have Path
            }else{

              //Find PATH eviornment variable information
              int i = 0;
              while(environ[i]){
            FullPath=environ[i];

            if(!FullPath.find("PATH")){
              FullPath=FullPath.substr(5,FullPath.length());
              break;
            }
            i++;
              }
              //Parse PATH string to go into each directory in the list to look for file specified in argv[0]
              int count = 1;
              string directory = "";
              bool FileFound = false;
              string temp(Path);//from command line that the user entered
              int numDir = NumOfStrings(FullPath,':');

              while(FileFound == false){
                if(count<=numDir){
            directory = NextDirectory(FullPath,count);
                 ifstream files ((directory+"/"+temp).c_str());
                  if(!files){
                      cout<<"Cannot open file: "<<directory+"/"+temp <<endl; 
                       count=count+1; //Get next directory to work in
                  }else{
                      FullPath = directory+"/"+temp;
                      files.close();
                      FileFound=true;
                      cout<<"File Found: "<<FullPath<<endl;
                  }
              }else{
                    cout<<"FILE DOES NOT EXIST IN ANY DIRECTORY"<<endl;
                    FullPath = "";
                    FileFound=true;
                }
              }
            }
            return FullPath;
        }

        int checkRedirect(char* argv[],int argc){
            for(int i=0;i<argc;i++){
                if(*argv[i] == '>' || *argv[i] == '<'){
                    return i+1;
                }
            }
        }

        void ExecuteRedirect(char* argv[], string NameRedirect,int redirect){
                     int in,out;
            if(NameRedirect == "StdOut"){

	      //cout<<"IN: "<<argv[redirect-2]<<endl;
                        in = open(argv[redirect-2], O_RDONLY);
                            if(in<0){
                                cout<<"Failed To Open"<<endl;
                            }
			    //        cout<<"OUT: "<<argv[redirect]<<endl;
                        out = open(argv[redirect],O_WRONLY|O_CREAT,S_IRWXU|S_IRWXG|S_IRWXO);   
                         if(out<0){
                                cout<<"Failed To Open"<<endl;
                            }   

                        } else{ //StdIn -- redirect stdin
	      //  cout<<"IN: "<<argv[redirect-2]<<endl;
                        in = open(argv[redirect], O_RDONLY);
			//   cout<<"OUT: "<<argv[redirect]<<endl;
                        out = open(argv[redirect-2],O_WRONLY|O_CREAT,S_IRWXU|S_IRWXG|S_IRWXO);    
                        }
                        dup2(in, 0);
                        dup2(out, 1);    
                        close(in);
                        close(out);
        }

        string TypeRedirect(char* argv[],int argc){
            for(int i=0;i<argc;i++){
                if(*argv[i] == '<'){ 
                     cout<<"Redirect Type: "<<"StdIn"<<endl;
                    return "StdIn";
                }else if(*argv[i] == '>'){
                    cout<<"Redirect Type: "<<"StdOut"<<endl;
                    return "StdOut";
                }
            }
            return "";
        }

        extern char **environ;
        int main(void) {
            //Declarations
            int pid;
            string path, line;
            int argc; //Number of args in command line
            char* argv[100]; //Declare arguement cstring pointer array

            int thePipe[2]; //Declare pipe
            int prevout,previn; //To set StdIn and StdOut back to normal after redirect
            do {
                label2:
                    char* PartA[100]; //half of pipe arguements
                    char* PartB[100]; //other hald of pipe arguements
                    char PathA[100];
                    char PathB[100];
                    char FullPath[100];
                    bool Hide = false;
                    bool redirect = false;
                    bool redirect2 = false;
                    bool next =false;
                
            bool FoundPipe = false;
                
                  //Print command line prompt
                cout << "Zoe's Shell > ";
                //Get command line
                getline(cin, line);
                cout<<" "<<endl;
                //cout << "Command was: " << line << endl;

                if ((line == "exit") || line=="^D")
                {
                    exit(0);
                }

                if(line.find("|")!= std::string::npos){
		  FoundPipe = true; //There is a pipe in the command line, handle accordingly
                    Hide = true;
                    pipe(thePipe);
                }else if(line.find('>')!= std::string::npos && line.find('>')!= std::string::npos){
                  redirect2 =true;//there are both < and > in the command line  
                }else if(line.find('>')!= std::string::npos || line.find('>')!= std::string::npos){
		  redirect=true;//there is either < or ? in the command line
                }  
                
                  if(line[line.length()-1]=='&'){
                        Hide = true;
                   if(line[line.length()-2]==' '){
                       line = line.substr(0,line.length()-2);
                   } else{
                       line = line.substr(0,line.length()-1);
                   }
                }

                argc=NumOfStrings(line,' '); //Finds Number of Strings in the line

                int countA=0;
                int countB=0;
                for(int i=0;i<argc;i++){
                char * temp=LineToCstring(line, i+1,FoundPipe); //Converts to Cstring and Has an array of pointers point to cstrings
                    if(FoundPipe == true){
		      if(*temp == '|'){ //handle pipe
                        PartA[countA]=NULL;
                        next = true;
                    }else if(next == false){
                              PartA[countA] = temp;  
                              countA++;
                    }else if(next==true){
                        PartB[countB] = temp;
                        countB++;
                    }   
                    }
                    else if(redirect2 == true){ // handle < and >
                       if((*temp != '<')&&(*temp != '>')){
                         argv[countA] = temp;
                        argv[countA+1]=NULL; 
                           countA++;
                           }
                           
                    }else{
                        argv[i] = temp;
                        argv[argc]=NULL; //Sets spot after cstrings to NULL in pointers
                    }
                }
                 PartB[countB] = NULL;

                if(FoundPipe==true){
                   string B = FullPathName(PartB[0]);
                   string A = FullPathName(PartA[0]);
                if(A==""||B==""){ //Failed to find path
                    cout<<"- - - Failed to Find Designated File In Any Directory.- - -"<<endl;
                    goto label2;
                }
                    strcpy(PathA, A.c_str()); //take path string and conver   
                    strcpy(PathB, B.c_str()); //take path string and conver   
                }else{
                //Find Full PathName For File
                    path = FullPathName(argv[0]);
                    
                cout<<"HERE"<<endl;
                if(path==""){ //Failed to find path
                    cout<<"- - - Failed to Find Designated File In Any Directory.- - -"<<endl;
                    goto label2;
                }
                    strcpy(FullPath, path.c_str()); //take path string and convert to char array to be able to send it into execvp   
                }
                if(redirect == true){ //handle that ther is either < or >
                    prevout=dup(1);
                    previn = dup(0);
                    string NameRedirect = TypeRedirect(argv,argc);
                     //If we want to run a redirect
                    int redirect = checkRedirect(argv,argc);
                    ExecuteRedirect(argv,NameRedirect,redirect);
                }else if(redirect2==true){
                    prevout=dup(1);
                    previn = dup(0);
                    int in,out;
                   in = open(argv[countA-2], O_RDONLY);
                    argv[countA-2]=NULL;
                    out = open(argv[countA-1], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
                    argv[countA-1]=NULL;
                    countA = countA-2;
		    dup2(in, 0);
		    dup2(out, 1);
		    close(in);
		    close(out);
                }
                
                pid=fork();
                if (pid == 0)
                { //Fork successful, run child
                   // cout<<"First Child"<<endl;
                    if(FoundPipe!=true){
                        execvp(FullPath, argv);
                        exit(0);
                    }else{
                        dup2(thePipe[1],1); //replace input with pipe input
                        //cout<<"here1"<<endl;
                        close(thePipe[1]);
                        close(thePipe[0]);
                        execvp(PathA,PartA);
                    }
                }
                else
                {
                    //cout<<"Parent of First Child"<<endl;
                    if(FoundPipe==true){
                        pid = fork();
                        if(pid == 0){
                        //cout<<"Second Child"<<endl;
                         dup2(thePipe[0],0);
                            cout<<"here"<<endl;
                            close(thePipe[0]);
                        close(thePipe[1]);
                        execvp(PathB,PartB);
                    }else{
                          //  cout<<"Second Parent"<<endl;
                        }
                    }
                }
                if(FoundPipe==true){
                         close(thePipe[1]);
                        close(thePipe[0]);
                 for(int status=0;status<2;status++){
                        wait(&status);
                 }
                }else{
                    if(!Hide){
                        //Doing Parent Stuff if we want the parent to wait for the child
                        int status=0;
                        wait(&status);

                        if(redirect == true || redirect2 == true){ 
                        //If we had a redirect....
                        fflush(stdout); //Flush out the rest of stdout
                        dup2(previn,0); //Reset StdIn back to original
                        close(previn);  //Close previn since not in use
                        dup2(prevout,1); //Reset StdIn back to original
                        close(prevout); //Close prevout since not longer in use
                        }
                        cout<<"\n"<<endl;
                        cout << "Child exited with status of " << status << endl;
                    }
                }
            } while ((line != "^D") && (line != "exit"));

            return 0;
        }
