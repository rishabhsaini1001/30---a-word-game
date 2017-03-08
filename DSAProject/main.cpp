//
//  main.cpp
//  DSAProject
//
//  Created by sushil on 05/05/16.
//  Copyright © 2016 rishabh. All rights reserved.
//

//
//  Project_Exp.cpp
//  hashing
//
//  Created by sushil on 22/04/16.
//  Copyright © 2016 rishabh. All rights reserved.
//

#include <stdlib.h>     // srand, rand
#include <time.h>       // time
#include<iostream>      // cin , cout
#include<string>
#include<fstream>
#include<list>

using namespace std;

//__________________the HASH function in use____________________________________________
static unsigned int FNVHash(string str) {
    const unsigned int fnv_prime = 0x811C9DC5;
    unsigned int hash = 0;
    unsigned int i = 0;
    unsigned int len = str.length();
    for (i = 0; i < len; i++)
    {
        hash *= fnv_prime;
        hash ^= (str[i]);
    }
    return hash%1000;
}
//__________________hashing- implementing collisions through chaining____________________
//......................insertion of a key.....................
void insertH(list<string> (&H)[1000],string v)
{
    H[FNVHash(v)].push_back(v);
}
//......................searching a key.....................
int searchH(list<string> (&H)[1000],string v)
{
    for(list<string>::iterator i = H[FNVHash(v)].begin();i!=H[FNVHash(v)].end();i++)
    {
        if(*i == v)
        {
            //cout<<"found\n";
            return 1;
        }
    }
    //cout<<"not found\n";
    return 0;
}
//......................deleting a key.....................
void deleteH(list<string> (&H)[1000],string v)
{
    H[FNVHash(v)].remove(v);
}

int main ()
{
    int iSecret;
    clock_t projectStart,projectStop;
    projectStart=clock();
    // initialize random seed:
    srand (time(NULL));
    int levelOfDifficulty=5;
    cout<<"Welcome to the arena!!! \n";
    cout<<"This is a word based timed game\n";
    cout<<"\n___________________________________________________________\n";
    cout<<"......................HOW TO PLAY...................\n";
    cout<<"1. Form English words of length >= 3\n  *in horizontal,\n  *vertical and \n  *diagonal directions\n2. Score >= 5 to win!!\n";
    
    //cout<<"Enter 1 to accept\n";
    int passkey;
    //cin>>passkey;
    //if(passkey==1)
    do
    {
        //the main thing !!!:D
        int order=5;
        cout<<"Enter the order of the grid \noptions available: (5,6,7,8,9)\n";
        cin>>order;
        while(order<5 || order>10)
        {
            cout<<"Please enter a valid option (5,6,7,8,9)\n";
            cin>>order;
        }
        cout<<"Enter the difficulty level\n";
        cout<<"Enter 1 for easy\n";
        cout<<"Enter 2 for moderate\n";
        cout<<"Enter 3 for difficult\n";
        cin>>levelOfDifficulty;
        switch(levelOfDifficulty)
        {
            case 3:
                levelOfDifficulty=order;
                break;
            case 2:
                levelOfDifficulty=order+order/2;
                break;
            case 1:
                levelOfDifficulty=order+order;
                break;
        }
        //levelOfDifficulty=(4-levelOfDifficulty)*order;
        char matrix[order][order];
        
        //throwing random alphabets on the order*order matrix
    b:
        int flag =1;
        list<string> gameArray,correctGuesses;
        do
        {
            string S[5000];
            int n=0;
            for (int i=0;i<order;i++)
            {
                for(int j=0;j<order;j++)
                {
                    iSecret = rand() % 25 ;
                    matrix[i][j]=(char)(iSecret + 97);
                    //cout<<matrix[i][j]<<" ";
                }
                //cout<<endl;
            }
            
            //reading all possible word combinatons from the matrix
            for(int r=0;r<order;r++)
                for(int i=0;i<order;i++)
                {
                    //the element itself i.e. matrix[r][i]
                    S[n++] = matrix[r][i];
                    //horizontal options
                    for(int j=0;j<order;j++)
                    {
                        if(j<i)
                        {
                            for(int k=i;k>=j;k--)
                                S[n]+=matrix[r][k];
                            n++;
                        }
                        else if(j>i)
                        {
                            for(int k=i;k<=j;k++)
                                S[n]+=matrix[r][k];
                            n++;
                        }
                    }
                    //vertical options
                    for(int j=0;j<order;j++)
                    {
                        if(j<r)
                        {
                            for(int k=r;k>=j;k--)
                                S[n]+=matrix[k][i];
                            n++;
                        }
                        else if(j>r)
                        {
                            for(int k=r;k<=j;k++)
                                S[n]+=matrix[k][i];
                            n++;
                        }
                    }
                    //diagonal options
                    //for(int j=0;j<order;j++)
                    for(int j=r-1;j<order && j<=r+1;j+=2)
                    {
                        if(j<r)
                        {
                            for(int k=0;k<order;k++)
                            {
                                if(k<i)
                                {if(i-k<=r)
                                {
                                    for(int z=i;z>=k;z--)
                                        S[n]+=matrix[r-(i-z)][z];
                                    n++;
                                }}
                                else if(k>i)
                                    if(k-i<=r)
                                    {
                                        for(int z=i;z<=k;z++)
                                            S[n]+=matrix[r-(z-i)][z];
                                        n++;
                                    }
                                
                            }
                        }
                        else if(j>r)
                        {
                            for(int k=0;k<order;k++)
                            {
                                if(k<i)
                                {if(i-k<=order-r)
                                {
                                    for(int z=i;z>=k;z--)
                                        S[n]+=matrix[r+(i-z)][z];
                                    n++;
                                }}
                                else if(k>i)
                                    if(k-i<=order-r)
                                    {
                                        for(int z=i;z<=k;z++)
                                            S[n]+=matrix[r+(z-i)][z];
                                        n++;
                                    }
                                
                            }
                        }
                        
                    }
                }
            
            fstream file;
            //file.open("ScrabbleSource.txt",ios::in);
            file.open("wordList.txt",ios::in);
            string temp;
            int k=0;
            //putting the matrix combinations in the hash table
            list<string> HashTable[1000];
            for(int i=0;i<n;i++)
            {
                if(searchH(HashTable,S[i])==0)
                    insertH(HashTable,S[i]);
            }
            //now comparing with the dictionary..................
            //////taking 1000 inputs at a time
            
            int possibleWordCount = 0;
        a:
            while(!file.eof() && k<1000)
            {
                k++;
                file >> temp;
                int length = temp.length();
                if(length>=3)
                {
                    int value = searchH(HashTable,temp);
                    if(value==1)
                    {
                        gameArray.push_front(temp);
                        possibleWordCount++;
                    }
                }
                if(length>order)
                    break;
            }
            if(!file.eof())
            {
                k=0;
                goto a;
            }
            if(possibleWordCount<levelOfDifficulty || possibleWordCount>levelOfDifficulty+order)
            {
                //goto b;
                flag = 0;
                while(!gameArray.empty())
                {
                    gameArray.clear();
                }
            }
            else{
                flag = 1;
            }
            file.close();
        }while(flag == 0);
        ////displaying the grid..................
        cout<<endl<<"---------------------------------------------------------------------\n\n\t\t\t\t\t";
        for (int i=0;i<order;i++)
        {
            for(int j=0;j<order;j++)
            {
                cout<<matrix[i][j]<<" ";
            }
            cout<<endl<<"\t\t\t\t\t";
        }
        cout<<endl<<"---------------------------------------------------------------------\n";
        cout<<"Do you want a different grid?\n";
        cout<<"Enter y for different grid\nEnter n to continue\n";
        char retry;
        cin>>retry;
        if(retry == 'y')
        {
            while(!gameArray.empty())
            {
                gameArray.clear();
            }
            goto b;
        }
        //initialising timer-------------------------------------------------
        int time_elapsed = 0;
        int score = 0;
        string guess;
        time_t now = time(0);
        tm *ltm = localtime(&now);
        int sec1 = 1 + ltm->tm_sec;
        cout<<"timer has been initiated\n30s left\n";
        
        //-----------------------------MAIN GAMEPLAY--------------------------------//
        //*************************************************************/
        
        while(time_elapsed<=30 && time_elapsed>=0)
        {
            cin>>guess;
            //check if the guess is correct
            for(list<string>::iterator i=gameArray.begin();i!=gameArray.end();i++)
                if(*i == guess)
                {
                    score++;
                    correctGuesses.push_front(*i);
                }
            //maintaining timer
            now = time(0);
            tm *ltm1 = localtime(&now);
            int sec2 = 1 + ltm1->tm_sec;
            //updating time_elapsed
            if(sec2<30 && sec1>30)
                time_elapsed = sec2 + (60 -sec1);
            else
                time_elapsed = sec2 - sec1;
            if(time_elapsed<30)
                cout<<30 - time_elapsed<<"s left\n"<<endl;
            //case for handling overshoot time
            else
            {
                cout<<"times up\n"<<endl;
                break;
            }
            
        }
        cout<<"your score is "<<score<<endl;
        //---------------------defeat case--------------------------------
        if(score<5)
            cout<<"You lost...better luck next time!!!!\n";
        //-----------------------victory case!!!--------------------------
        else
            cout<<"You WON..kiddos!!!\n";
        //________________showing all the correct guesses____________________
        cout<<"The answers you guessed correctly are:\n";
        for(list<string>::iterator i=correctGuesses.begin();i!=correctGuesses.end();i++)
            cout<<*i<<endl;
        //________________showing all the possible solutions__________________
        cout<<"All the possible solutions were\n";
        for(list<string>::iterator i=gameArray.begin();i!=gameArray.end();i++)
        {
            cout<<*i<<endl;
        }
        cout<<"\n___________________________________________________________\n";
        cout<<"\nDo you wanna play again!!\nEnter 1 to play again\\nHit any other key to exit";
        cin>>passkey;
    }while(passkey==1);
    return 0;
}