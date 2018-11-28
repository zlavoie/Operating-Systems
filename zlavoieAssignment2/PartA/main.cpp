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

        extern char **environ;
        int main(void) {
            //Declarations
            int pid;
            string path, line;
            int argc; //Number of args in command line
            char* argv[100]; //Declare arguement cstring pointer array

            do {
                label2:
                    char FullPath[100];
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

                argc=NumOfStrings(line,' '); //Finds Number of Strings in the line

                for(int i=0;i<argc;i++){
                char * temp=LineToCstring(line, i+1,FoundPipe); //Converts to Cstring and Has an array of pointers point to cstrings
                   
                        argv[i] = temp;
                        argv[argc]=NULL; //Sets spot after cstrings to NULL in pointers
                }
                 
                    path = FullPathName(argv[0]);
                    
                if(path==""){ //Failed to find path
                    cout<<"- - - Failed to Find Designated File In Any Directory.- - -"<<endl;
                    goto label2;
                }
                    strcpy(FullPath, path.c_str()); //take path string and convert to char array to be able to send it into execvp   

               

                pid=fork();
                if (pid == 0)
                { //Fork successful, run child
                        execvp(FullPath, argv);
                        exit(0);
                    
                }
                else
                {
                   
                        //Doing Parent Stuff if we want the parent to wait for the child
                        int status=0;
                        wait(&status);

                        
                        cout<<"\n"<<endl;
                        cout << "Child exited with status of " << status << endl;
                  
                    }            
            } while ((line != "^D") && (line != "exit"));

            return 0;
        }
