//
//  main.cpp
//  lizzards
//
//  Created by Hooman Haji on 9/16/17.
//  Copyright © 2017 Hooman Haji. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <time.h>       /* time */
#include <math.h>
#define MAXTEMP 1000
using namespace std;

    struct point{
        int x,y;
    };

int countThreats(point lizzards[], point walls[], int numLiz, int numwalls);

int main(int argc, const char * argv[]) {
    // insert code here...
    int numLiz=0, boardSize=0;
    srand (time(NULL));


    ifstream stream1;
    ofstream stream2("output.txt");

   
    
    
    stream1.open("input.txt");
    if(!stream1.is_open())
        cout << "unable to open file";
    string alg;
    
    
    //getline(stream1,alg);


    stream1 >> boardSize;
    stream1 >> numLiz;

    
    
    
    point walls[100];
    point lizzards[100];

    
    int i=0;
    int numwalls=0;
    
    for (;i < boardSize; i ++){
        std::string temp;
        stream1 >> temp;
        int j=0;
        for(; j <  boardSize; j ++){
            char c = temp[j];
            if (c == '2'){
                walls[numwalls].x = i;
                walls[numwalls].y = j;
                numwalls ++;
            }
            
        
        }
    
    
    }
 //   for(int i =0;i < numwalls;i++)
 //   	cout << "wall "<< walls[i].x << " " << walls[i].y<<endl;
    
    // initialze the board
    for (i = 0;i < numLiz ; i++)
    {
        lizzards[i].x = rand()%boardSize;
        lizzards[i].y = rand()%(boardSize);
        //detect if on top of a wall
        for(int k=0;k<numwalls; k++){
            if(walls[k].x==lizzards[i].x && walls[k].y==lizzards[i].y){
                i--;
                break;
            }
        }
        //detect if collides with another Lizzard
        for (int k =0;k < i; k++){
            if(lizzards[k].x == lizzards[i].x && lizzards[k].y == lizzards[i].y){
            
                i--;
                break;
            }
        }
    }
    

    
    int threats = countThreats(lizzards,walls,numLiz,numwalls);
    if(threats == 0){
        std::cout  << "	SUCCESS\n";
    }
    
    //simulated annealing
    int success = 0;
    for(int temp = 1; temp < MAXTEMP; temp ++){
    
   		//pick a successor 
    	int changingNode = rand()%numLiz , flag =1; 
		//randomly change it
		point tmp;
	
		while(flag){
			flag = 0;
			tmp.x = rand()%boardSize;
    		tmp.y = rand()%(boardSize);
    
	
			//detect if on top of a wall
    		for(int k=0;k<numwalls; k++){
        		if(walls[k].x==tmp.x && walls[k].y==tmp.y){
            		flag = 1;
        			break;
        		}
    		}
        	//detect if collides with another Lizzard
    		for (int k =0;k < numLiz; k++){
        		if(lizzards[k].x == tmp.x && lizzards[k].y == tmp.y){
                	flag = 1;
               	 	break;
            	}
        	}
		}
			
	point oldLizzard;
	oldLizzard.x = lizzards[changingNode].x;
	oldLizzard.y = lizzards[changingNode].y;
		
	lizzards[changingNode].x = tmp.x;	
	lizzards[changingNode].y = tmp.y;
	
	
    int newThreats = countThreats(lizzards,walls,numLiz,numwalls);
    if(newThreats == 0){
        std::cout  << "	SUCCESS\n";
        success = 1;
		break;
	}
	else{
		cout << newThreats<< endl;
	}
    //compare the results for the successor and the original
	int e = newThreats - threats;
	if(e < 0){
		threats = newThreats;
	}
	else {
		int coinToss = rand()%100;
		int chance = 100*exp(double(-e/temp));
		if(coinToss > chance){
			threats = newThreats;
		}
		else{
			lizzards[changingNode].x = oldLizzard.x;
			lizzards[changingNode].y = oldLizzard.y;
		}
	}
    
    }
    
    if(!success)
        stream2 << "FAIL" << endl;
   
 	else{//output the result
 		stream2 << "OK" << endl;
		for (int i=0;i < boardSize; i ++){
			for(int j=0;j< boardSize;j++){
				int mark =0;
				for(int k =0; k < numLiz; k++)
					if(lizzards[k].x == i && lizzards[k].y == j){
						stream2 << "1";
						mark = 1;
						break;
					}
				if(!mark){
					for(int k =0;k<numwalls; k++)
						if(walls[k].x == i && walls[k].y ==j){
							stream2 << "2";
							mark =1;
							break;
						}
				}	
				if(!mark)
					stream2 << "0";
				
			}
		stream2 << endl;
		}
 		
	}   
    
    return 0;
}



int countThreats(point lizzards[], point walls[], int numLiz, int numwalls){
	int threats = 0, i =0,j=0;
    // count threats
    for (i = 0; i < numLiz; i ++){
        for(int j = 0; j < i; j++){
            int coll = 0;
            if(lizzards[i].x == lizzards[j].x)
                coll = 3;
            else if(lizzards[i].y == lizzards[j].y)
                coll = 4;
            
            
            else if( lizzards[i].x - lizzards[j].x == lizzards[i].y - lizzards[j].y)
                    coll =1;
        	else if(lizzards[i].x-lizzards[j].x == lizzards[j].y - lizzards[i].y)
                    coll =2;
            
          
            
            
            //see if wall in the way
            for(int k=0;k<numwalls; k++){
				if (coll == 3)
                	if (walls[k].x == lizzards[i].x){
                		int a = walls[k].y -lizzards[i].y;
						int b =  walls[k].y - lizzards[j].y;
                		
                		if( ((a > 0)&&(b<0))  || ((a <0)&&(b>0)))
                        	coll =0;
    				}
                if(coll == 4)
                	if(walls[k].y == lizzards[i].y){
						int a = walls[k].x -lizzards[i].x;
						int b =  walls[k].x - lizzards[j].x;
                        
						if( ((a > 0)&&(b<0))  || ((a <0)&&(b>0)))
							coll =0;

    				}
                if(coll == 1){
                        if(walls[k].x - lizzards[i].x == walls[k].y - lizzards[i].y){
                        	int a = walls[k].x -lizzards[i].x;
							int b =  walls[k].x - lizzards[j].x;
                        	
                        	if(((a > 0)&&(b<0))  || ((a <0)&&(b>0)))
                            	coll = 0;
                        }
                    
                    }
                if(coll == 2){
                		if(walls[k].x - lizzards[i].x == lizzards[i].y - walls[k].y){
                			int a = walls[k].x -lizzards[i].x;
							int b =  walls[k].x - lizzards[j].x;
                        	
                        	if(((a > 0)&&(b<0))  || ((a <0)&&(b>0)))
                            	coll = 0;
                			
						}
                	
				}
                
                                        
            }
            
            if(coll > 0)
                threats ++;   
            

        }
    
    }
return threats;
	
}


