#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include "ourvector.h"
using namespace std;


//-----------------------------------------------------------------------
//Functions declarations


void loadspamlist(ourvector <string>& V);
void displayspamlist(ourvector <string>& V);
void splitemail(string& userInputEmail, string& part_1,string& part_2, char character);
bool binarySearch( string currentUsername, string currentDomain, ourvector <string>& V);
bool checklist(string userInputEmail, ourvector <string>& V, string &currentUsername, string &currentDomain, string command);
void filterlist(string command, ourvector <string>& V, string &username, string &domainname, string userInputEmail);


//-----------------------------------------------------------------------
//Function that loads a particular file, input by user.


void loadspamlist(ourvector <string>& V)
{
     
   V.clear();               //clearing the existing data in ourvector
   ifstream inFS;
   string fileword;
   string filename;
    
   cin >> filename;        // input filename
   int i=0;                // to keep track of number of spams in spamlist loaded
   
   inFS.open(filename);    //Open file
   getline(inFS,fileword);
   
    if(!inFS.is_open())
    {
      cout<<"**Error, unable to open '"<<filename<<"'"<<endl<<endl;
    }


    else 
    {
        //While loop to pushback filewords(spamemails) in vector and count number of entries in vector
        while(!inFS.eof())
        {
        V.push_back(fileword);
        i++;
        getline(inFS,fileword);
        }
        cout << "Loading '" << filename << "'" << endl;
        cout << "# of spam entries: " << i << endl << endl;
    }
}


//-----------------------------------------------------------------------
//Function that display the file loaded by the user


void displayspamlist(ourvector <string>& V)
{
     //for loop to print contents stored in vector V
     for(int j=0;j<V.size();j++)
    {
        cout<<V[j]<<endl;
    }
    cout<<endl;
}


//-----------------------------------------------------------------------
//Funtion use to split emails in 2 parts at a particular char


void splitemail(string& userInputEmail, string& part_1,string& part_2, char character)
{
    int found;   //variable to store index of char from where to split
    found = userInputEmail.find(character);
    part_1 = userInputEmail.substr(0,found);
    part_2 = userInputEmail.substr(found+1,userInputEmail.length());
}


//-----------------------------------------------------------------------


bool binarySearch( string currentUsername, string currentDomain, ourvector <string>& V)
{
   int mid = 0;
   int low = 0;
   int high = V.size() - 1;
   string spamUsername="";
   string spamDomain="";
   
   while (high >= low) 
   {
     mid = (high + low) / 2;
     splitemail(V[mid], spamDomain, spamUsername, ':');


        if (spamDomain == currentDomain)
        {
            if(spamUsername == currentUsername) 
            {
                return true;
            }
            else if(spamUsername < currentUsername)
            {
                low = mid + 1;
            }
            else
            {
                high = mid - 1;
            }
        }
        else if(spamDomain > currentDomain) 
        {
            high = mid - 1;
        }
        else
        {
            low = mid + 1;
        }
   } //end of the while loop
  return false;
}


//-----------------------------------------------------------------------
//Function that checks whether the email is spam / non-spam email


bool checklist(string userInputEmail, ourvector <string>& V, string &currentUsername, string &currentDomain, string command)
{
    
    
    //calling splitemail in order to split the email
    char character = '@';
    splitemail(userInputEmail, currentUsername, currentDomain, character);
   
    //calling binarySearch to check if email is spam or not
    if (binarySearch( currentUsername, currentDomain, V) || binarySearch( "*", currentDomain, V)) {
        if (command == "check") {
            cout << userInputEmail << " is spam" << endl; 
        }
        return false;
    }
    else
    {
        if (command == "check") {
            cout << userInputEmail << " is not spam" << endl;
        }
        return true;
    }
}


//-----------------------------------------------------------------------
//Function use to filter emails in one file by comparing every email in that file with spam emails file


void filterlist(string command, ourvector <string>& V, string &username, string &domainname, string userInputEmail)
{
    int num1 = 0;        //num counting number of emails processed
    int num2 = 0;        //num counting number of non-spam emails 
    ifstream inFS;
    string filename;
   
    string number;       //starting number of file line
    string email;        //email on the filename
    string otherstring;  //rest of the stuff on file line
   
    cin >> filename;     // input filename
    inFS.open(filename); //Open file1
    
    ofstream outFS;
    string filename2;    //output filename2
    
    cin >> filename2;      //input output filename2
    outFS.open(filename2); //open file2 
 
    
    if(!inFS.is_open()){
      cout<<"**Error, unable to open '"<<filename<<"'"<<endl<<endl;
      return;
    }
    
    else
    {
        while(!inFS.eof())
        {
            otherstring = "";
            inFS>>number;
            inFS>>email;


            getline(inFS, otherstring);
            if (!inFS.fail()) 
            {
                num1++;


                if( checklist(email, V, username, domainname, command) && otherstring != "")
                {
                    outFS<<number<<" "<<email<<" "<<otherstring<<endl;
                    num2++;
                } 
            }
        }//end of while loop
    }
     cout<<"# emails processed: "<<num1<<endl;
     cout<<"# non-spam emails: "<<num2<<endl<<endl;
    
    inFS.close();
    outFS.close();
}

//-----------------------------------------------------------------------
int main()
{
    ourvector <string> V;
    cout<<"** Welcome to spam filtering app **"<<endl<<endl;
    string command;         //input for the command to be executed
    string userInputEmail;  //user input of email to check the email
    char character;
    
    //declaration for 2 parts of email
    string currentUsername;
    string currentDomain;
    
    //variable that stores Concatenated domainname+username;
    string concatenate1;
    string concatenate2;
   
    do{
        cout<<"Enter command or # to exit> ";
        cin>>command;     //input command from user


        if(command=="load")
        {
            loadspamlist(V);
        }
        
        else if(command=="display")
        {
            displayspamlist(V);
        }
        else if(command=="check")
        {
            //User input for email check
            cin>>userInputEmail;
            
            checklist(userInputEmail, V, currentUsername, currentDomain, command);
        }
        else if(command=="filter")
        {
            filterlist(command, V, currentUsername, currentDomain, userInputEmail);
        }
        
        else
        {
            if(command != "#"){
                  cout<<"**invalid command"<<endl<<endl;
            }
        }


     } while(command!="#");
    
}