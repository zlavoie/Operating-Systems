 #include <bits/stdc++.h>
#include <main.h>
#include <time.h>
using namespace std; 
  
  
// struct for node 
struct node { 
    int info; 
    node *right, *down, *up , *left; 
}; 
  
//create new node with number in matrix
node* creation(int newNum)
{
  //cout<<"Num : "<<newNum<<endl;
  //Create new node for information being sent in
  node* temp = new node;
  //Insert number to node
    temp->info = newNum;
    //cout<<"Created Node: "<<temp->info<<endl;
    temp->right = NULL;
    temp->left = NULL;
    temp->up = NULL;
    temp->down = NULL;
    //Set pointers for node to NULL

    //Return new node
    return temp;
}

//Found the shuffle of array method from: stackoverflow.com/questions/6127503/shuffle-array-in-c
void shuffle(int *arr, int n)
{
    if (n > 1)
    {
        int i;
        srand(time(NULL));
        for (i = 0; i < n - 1; i++)
        {
          int j = i + rand() / (RAND_MAX / (n - i) + 1);
          int t = arr[j];
          arr[j] = arr[i];
          arr[i] = t;
        }
    }
}

//Reset the pointers of deleted node to NULL so we do not have a memory leak
void nullify(node *reset){
  reset->right = NULL;
  reset->left=NULL;
  reset->down=NULL;
  reset->up=NULL;
}

//Delete The Randomly Selected Node
node* DeleteNode(node **start, int position) 
{
  //Next node to the right of the deleted node
  struct node *next;

  //set temp as the first of the list of the row
  struct node *temp = *start;
  
  //Set temp to the head of the list that the nodeis in
  struct node *previous;

  //previous gives the node to the left of the node if there is one
   //It is first in the linked list row
    if(temp->info == position){
      //      cout<<"FIRST OF LIST"<<endl;
        if(temp->right != NULL){
	  previous = temp;
	  //Set previous: node to the left of node to be deleted
	  temp = temp->right;
	  //set temp to the right of the node to be deleted (first in the list)
	  *start=temp; //Reset the head
	  next = temp; 
	  nullify(previous); //Reset pointers to NULL
	  delete previous; //Delete node
	  previous = NULL; //Set the deleted Node to NULL
	}
        else{
	  //Only node left in row....nowhere to the right to link head.
	  *start = NULL; //No head of row
	    //Delete Lone Node
	  nullify(temp);//Reset pointers of node to be deleted to NULL
	  delete temp; // Delete Node
	  temp = NULL; //set deleted node to NULL
        }
        return *start; //Return new head o flist if it has one for the linked list
    }

    //Find Previous Node Before the Node You Are Deleting
      while(temp!=NULL && (temp->right->info) != position){
         temp=temp->right; 
      }

      //cout<<"PREVIOUS: "<<temp->info<<endl;
      // If the right of the node being deleted is not NULL and has a Node to connect to
    if(temp->right->right != NULL){
    next = temp->right->right; 
    //cout<<"NEXT: "<<next->info<<endl;
    }else{
      // No Node to connect the right pointer to adter deletion
      next=NULL;
    }
    previous = temp->right;
    //Remove node from list
    nullify(previous);
    delete(previous);
    // Free memory 
    previous = NULL;
    temp->right = next;  // Unlink the deleted node from list 
    //Create new pointers and return (new) head
  return *start;  

}     
 
 // print linked list by first node in the row
void GridView(node* start)
{
  node *rp = start;

  if(rp !=NULL){
  while(rp != NULL){
       cout<<rp->info<<" ";
       rp = rp->right;
  }
  }
  cout<<""<<endl;
}

//Check to see if a node has NO EDGES attached to it
//If No Edges: then delete the node at its position
void checkEdges(node* start)
{
  //  cout <<"IN CHECK EDGES METHOD"<<endl;
  node *rp = start;

 if(rp !=NULL){
 while(rp != NULL){
   //cout <<"Node: "<<rp->info<<" right:"<<rp->right->info<<endl;
   if(rp->right == NULL && rp->left==NULL && rp->up==NULL && rp->down==NULL){
             	int pos = rp->info;
		//		cout <<"NO EDGES FOUND ON THIS NODE"<<endl;
		DeleteNode(&start, pos);
      }

      rp = rp->right;
 }
 }
}

//Create grid linked list 
void LinkedGrid()
{   
    int FillNums = 0; //Numbers to fill as info for the grid
    int table[10][10]; //Declare Multi-Dimensional Grid  (10x10)
    for(int y = 0; y<10 ; y++){
        for(int x=0; x<10;x++ ){
            table[y][x] = FillNums; //Fill Grid With Numbers 0-99 (100 Nums, 10x10)
            //cout<<"grid y:0"<<y<<" x: "<<x<<"--Num: "<<Grid[y][x]<<endl;
            FillNums++;
        }
    }
    //stores the head of linked lists of each row
    node* StartList[10];
    // stores the head of the linked list
    // stores the pointers for nodes
    node *ptrnext, *previousptr;
   
   // Firstly, we create m linked lists
    // by setting all the right nodes of every row
    for (int i = 0; i < 10; i++) { 
  
        // initially set the head of ith row as NULL 
    StartList[i] = NULL; 
        for (int j = 0; j < 10; j++) {
	  ptrnext = creation(table[i][j]);

	  //Got the idea for linking the lists from www.stackoverflow.com/questions/3544337/grid-data-structure
	  
	  //cout <<newptr->info<<" ";
            if (!StartList[i]){
                StartList[i] = ptrnext; 
                previousptr =  ptrnext;
            }else{
                previousptr -> right =  ptrnext;
                 ptrnext -> left = previousptr; 
                //cout<<"Right Point: row: "<<j<<" Col:"<<i<<"--"<<newptr->left->info<<endl;
                previousptr =  ptrnext; 
            }
        }
        //cout<<""<<endl;
    }  
    node *liststart;
    node *connect;
    
        // Set down pointers
    for (int i = 0; i < 9; i++) { 
      liststart = StartList[i];
      connect = StartList[i + 1]; 
        while (liststart && connect) { 
            liststart->down = connect; 
            //cout<<"Down Point: for row:"<<i<<": "<<temp1->down->info<<endl;
            liststart = liststart->right; 
            connect = connect->right; 
        } 
    } 
    
    //Set up pointers
        for (int i = 9; i >0 ; i--) { 
        connect =  StartList[i - 1];
	liststart = StartList[i];
	while (liststart && connect) { 
            liststart->up = connect; 
            //cout<<"Up Point: for row:"<<i<<": "<<temp3->up->info<<endl;
            liststart = liststart->right; 
            connect = connect->right; 
        } 
    } 
   
    int position; //position of node we are deleting
    int Row; //Row of grid node is listed in
    int randnum[100];//array of ints 0-99 to shuffle to "randomize"

    for(int t = 0;t<100;t++){
      randnum[t]=t;//fill array sequentially
    }

    shuffle(randnum, 100); //shuffle array to make numbers random

    for(int count=0;count<100;count++){
        position = randnum[count];
        cout<<"position:"<<position<<endl;
	Row = position/10; //gives row the position is in
	//Delete the node that the random number corresponds to
    StartList[Row] = DeleteNode(&StartList[Row], position);

    //Check for any nodes with no edges
    for(int g=0;g<10;g++){
        checkEdges(StartList[g]);
        }

    //display grid
    for(int g=0;g<10;g++){
    GridView(StartList[g]);
    }
     }
}
  
//MAIN METHOD
int main() 
{ 
	LinkedGrid();
	return 0; 
} 
