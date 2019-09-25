#include <iostream>
#include <stdlib.h>
#include <math.h>
#include<stdlib.h>
#include<time.h>
#include<stdio.h>
#include <fstream>
#include <cstring>

using namespace std;


void delay (){
	// Literally to delay the code
	for (int i = 0; i<=1167; i++){
		for (int j = 0; j<= 32767; j++){
		}
	}
	//cout << "yes" << endl;
}
ofstream logFile;

int WIDTH = 8;
int HEIGHT = 8;
int score = 0;

enum COLOUR {BLACK, PURPLE, ORANGE, RED, GREEN, YELLOW, SILVER, BLUE, WHITE, HYPERCUBE};
enum CHAINS{ UP, DOWN, LEFT, RIGHT, DONE};
enum LOGGING {NONE, USER, TESTING, FATAL};
LOGGING logState = NONE;



class Jewel{
	//A class for the tiles. Just a way to keep everything together
	private:
		int col;
		int row;
		bool select;
		COLOUR clr;
	public:
		Jewel();
		Jewel(int colIn, int rowIn, bool selectIn);
		~Jewel();
		int getCol();
		int getRow();
		bool getSelect();
		COLOUR getClr();
		void setRow(int rowIn);
		void setCol(int colIn);
		void setClr(int clrIn);
		void setSelect(bool selectIn);
		bool switch_up(Jewel** other);
		bool switch_down(Jewel** other);
		bool switch_left(Jewel** other);
		bool switch_right(Jewel** other);
		char print();
};

Jewel::Jewel(){
	col = -1;
	row = -1;
	select = false;
	clr = static_cast<COLOUR>(rand()%7+1);
}

Jewel::Jewel(int rowIn, int colIn, bool selectIn){
	col = colIn;
	row = rowIn;
	select = selectIn;
	clr = static_cast<COLOUR>(rand()%7+1);
}
Jewel::~Jewel(){
}
int Jewel::getCol(){
	return col;
}
int Jewel::getRow(){
	return row;
}
bool Jewel::getSelect(){
	return select;
}
COLOUR Jewel::getClr(){
	return clr;
}
void Jewel::setRow(int rowIn){
	row = rowIn;
}
void Jewel::setCol(int colIn){
	col = colIn;
}
void Jewel::setClr(int clrIn){
	clr = static_cast<COLOUR>(clrIn);
}

Jewel* findJewel(Jewel** jewelList, int row, int col){
	// finds a Jewel based on it's row and column
	for (int i = 0; i<WIDTH*HEIGHT; i++){
		if (jewelList[i] != NULL){
			if (jewelList[i]->getRow() == row && jewelList[i]->getCol() == col)
				return jewelList[i];
		}
	}
}

bool ifFindJewel(Jewel** jewelList, int row, int col){
	//cerr << "if: if: if: " << endl;
	for (int i = 0; i<WIDTH*HEIGHT; i++){
		if(jewelList[i] != NULL){
			if (jewelList[i]->getRow() == row && jewelList[i]->getCol() == col){
				return true;
		}
	}
	}
	return false;
}
		


bool Jewel::switch_up(Jewel** jewelList){
	if (row == 0)
		return false;
	Jewel* tmp = findJewel(jewelList, row-1, col);
	tmp->row +=1;
	row -=1;
	return true;
}

char Jewel::print(){
	switch(clr){
		case BLACK:
			return '0' ;
			
		case PURPLE:
			return '1';

		case ORANGE:
			return '2';

		case RED:
			return '3';

		case GREEN:
			return '4';

		case YELLOW:
			return '5';

		case SILVER:
			return '6';

		case BLUE:
			return '7';

		case WHITE:
			return '8';

		case HYPERCUBE:
			return '9';

	}
}

void printJewels(Jewel** jewelList);


void chain(Jewel** jewelList, Jewel* start, Jewel* current, int& ver, int& hor, CHAINS move, Jewel** delHor, Jewel** delVer, int& delHorIndex, int& delVerIndex, int holes[][8], int& numHoles){
	//A recursive function that checks for Jewels of the same colour around the Jewel start. It first looks above, then to the right, then down, then left. Basically everytime the jewel above has the same colour, it will call the function in the same direction
	if (move == UP){
		if (current->getRow() != 0){
			if (current->getClr() == (findJewel(jewelList, current->getRow()-1, current->getCol()))->getClr()){
				current = findJewel(jewelList,current->getRow()-1, current->getCol());
				if (logState == TESTING ||logState == FATAL)
					logFile << "Time: " <<time(0) << " Line: 171" << "Current Coor: " << current->getRow() << current->getCol() << " Colour: " << current->getClr() << endl;
				//cout << "UP " << current->getRow() << current->getCol() << endl;
				ver +=1;
				delVer[delVerIndex] = current;
				delVerIndex++;
				chain(jewelList, start, current, ver, hor, UP, delHor, delVer, delHorIndex, delVerIndex, holes, numHoles);
			}
			else{
				if (start->getCol() != 7){
					current = start;
					if (logState == FATAL)
						logFile << "Time: " <<time(0) << " Line: 182" << "Current Coor: " << current->getRow() << current->getCol() << " Colour: " << current->getClr() << endl;

					move = RIGHT;
					//cout << "RIGHT"<<endl;
					chain(jewelList, start, current, ver, hor, move, delHor, delVer, delHorIndex, delVerIndex, holes, numHoles);
				}
				else if (start->getRow() != 7){
					//cerr << "DOWN" << endl;
					current = start;
					if (logState == FATAL)
						logFile << "Time: " <<time(0) << " Line: 192" << "Current Coor: " << current->getRow() << current->getCol() << " Colour: " << current->getClr() << endl;

					move = DOWN;
					chain(jewelList, start, current, ver, hor, move, delHor, delVer, delHorIndex, delVerIndex, holes, numHoles);
				}
				else if (start->getCol() != 0){
					current = start;
					move = LEFT;
					if (logState == FATAL)
						logFile << "Time: " <<time(0) << " Line: 201" << "Current Coor: " << current->getRow() << current->getCol() << " Colour: " << current->getClr() << endl;

					chain(jewelList, start, current, ver, hor, move, delHor, delVer, delHorIndex, delVerIndex, holes, numHoles);
				}
				else{
					move = DONE;
					if (logState == FATAL)
						logFile << "Time: " << time(0) << " Line: 208 Move: DONE" << endl;
					chain(jewelList, start, current, ver, hor, move, delHor, delVer, delHorIndex, delVerIndex, holes, numHoles);
				}
			}
		}
		else {
			if (start->getCol() != 7){
				current = start;
				if (logState == FATAL)
						logFile << "Time: " <<time(0) << " Line: 217" << "Current Coor: " << current->getRow() << current->getCol() << " Colour: " << current->getClr() << endl;

				move = RIGHT;
				chain(jewelList, start, current, ver, hor, move, delHor, delVer, delHorIndex, delVerIndex, holes, numHoles);
			}
			else if (start->getRow() != 7){
				current = start;
				if (logState == FATAL)
						logFile << "Time: " <<time(0) << " Line: 225" << "Current Coor: " << current->getRow() << current->getCol() << " Colour: " << current->getClr() << endl;

				move = DOWN;
				chain(jewelList, start, current, ver, hor, move, delHor, delVer, delHorIndex, delVerIndex, holes, numHoles);
			}
			else if (start->getCol() != 0){
				current = start;
				if (logState == FATAL)
						logFile << "Time: " <<time(0) << " Line: 233" << "Current Coor: " << current->getRow() << current->getCol() << " Colour: " << current->getClr() << endl;

				move = LEFT;
				chain(jewelList, start, current, ver, hor, move, delHor, delVer, delHorIndex, delVerIndex, holes, numHoles);
			}
			else{
				move = DONE;
				chain(jewelList, start, current, ver, hor, move, delHor, delVer, delHorIndex, delVerIndex, holes, numHoles);
			}
		}
	}
	else if (move == RIGHT){
		//cout << current->getCol() << endl;
		if (current->getCol() != 7){
			
			if (current->getClr() == (findJewel(jewelList, current->getRow(), current->getCol()+1))->getClr()){
				
				current = findJewel(jewelList, current->getRow(), current->getCol()+1);
				if (logState == TESTING ||logState == FATAL)
					logFile << "Time: " <<time(0) << " Line: 232" << "Current Coor: " << current->getRow() << current->getCol() << " Colour: " << current->getClr() << endl;

				
				//cout << "RIGHT " << current->getRow() << current->getCol() << endl;
				hor +=1;
				//cout << current->getCol() << endl;
				delHor[delHorIndex] = current;
				delHorIndex++;
				chain(jewelList, start, current, ver, hor, RIGHT, delHor, delVer, delHorIndex, delVerIndex, holes, numHoles);
			}
			else{
				if (start->getRow() != 7){
					//cout << 1 << endl;
					current = start;
					if (logState == FATAL)
						logFile << "Time: " <<time(0) << " Line: 267" << "Current Coor: " << current->getRow() << current->getCol() << " Colour: " << current->getClr() << endl;

					move = DOWN;
					chain(jewelList, start, current, ver, hor, move, delHor, delVer, delHorIndex, delVerIndex, holes, numHoles);
				}
				else if (start->getCol() != 0){
					//cout << 2 << endl;
					current = start;
					if (logState == FATAL)
						logFile << "Time: " <<time(0) << " Line: 276" << "Current Coor: " << current->getRow() << current->getCol() << " Colour: " << current->getClr() << endl;

					move = LEFT;
					chain(jewelList, start, current, ver, hor, move, delHor, delVer, delHorIndex, delVerIndex, holes, numHoles);
				}
				else{
					//cout << 3 << endl;
					move = DONE;
					
					chain(jewelList, start, current, ver, hor, move, delHor, delVer, delHorIndex, delVerIndex, holes, numHoles);
				}
			}
		}
	
		else{
			if (start->getRow() != 7){
				current = start;
				if (logState == FATAL)
						logFile << "Time: " <<time(0) << " Line: 294" << "Current Coor: " << current->getRow() << current->getCol() << " Colour: " << current->getClr() << endl;

				move = DOWN;
				//cout << "DOWN" << endl;
				chain(jewelList, start, current, ver, hor, move, delHor, delVer, delHorIndex, delVerIndex, holes, numHoles);
			}
			else if (start->getCol() != 0){
				current = start;
				if (logState == FATAL)
						logFile << "Time: " <<time(0) << " Line: 303" << "Current Coor: " << current->getRow() << current->getCol() << " Colour: " << current->getClr() << endl;

				move = LEFT;
				chain(jewelList, start, current, ver, hor, move, delHor, delVer, delHorIndex, delVerIndex, holes, numHoles);
			}
			else{
				move = DONE;
				chain(jewelList, start, current, ver, hor, move, delHor, delVer, delHorIndex, delVerIndex, holes, numHoles);
			}
		}
	}
			
	else if (move == DOWN){
		//cerr << current->getRow() << current->getCol() << endl;
		if (current->getRow() != 7){
			//cerr << 1 << endl;
			if (current->getClr() == (findJewel(jewelList, current->getRow()+1, current->getCol()))->getClr()){
				current = findJewel(jewelList, current->getRow()+1, current->getCol());
				if (logState == TESTING ||logState == FATAL)
					logFile << "Time: " <<time(0) << " Line: 289" << "Current Coor: " << current->getRow() << current->getCol() << " Colour: " << current->getClr() << endl;

				ver +=1;
				delVer[delVerIndex] = current;
				delVerIndex++;
				chain(jewelList, start, current, ver, hor, DOWN, delHor, delVer, delHorIndex, delVerIndex, holes, numHoles);
			}
			else {
				if (start->getCol() != 0){
					current = start;
					
					if (logState == FATAL)
						logFile << "Time: " <<time(0) << " Line: 300" << "Current Coor: " << current->getRow() << current->getCol() << " Colour: " << current->getClr() << endl;

					move = LEFT;
					chain(jewelList, start, current, ver, hor, move, delHor, delVer, delHorIndex, delVerIndex, holes, numHoles);
				}
				else {
					move = DONE;
					chain(jewelList, start, current, ver, hor, move, delHor, delVer, delHorIndex, delVerIndex, holes, numHoles);
				}
			}
		}
		else{
			if (start->getCol() != 0){
				current = start;
				move = LEFT;
				if (logState == FATAL)
						logFile << "Time: " <<time(0) << " Line: 351" << "Current Coor: " << current->getRow() << current->getCol() << " Colour: " << current->getClr() << endl;

				chain(jewelList, start, current, ver, hor, move, delHor, delVer, delHorIndex, delVerIndex, holes, numHoles);
			}
			else {
				move = DONE;
				chain(jewelList, start, current, ver, hor, move, delHor, delVer, delHorIndex, delVerIndex, holes, numHoles);
			}
		}
		
	}
	else if (move == LEFT){
		//cout << "yup"<<endl;
		if (current->getCol() != 0){
			if (current->getClr() == (findJewel(jewelList, current->getRow(), current->getCol()-1))->getClr()){
				current = findJewel(jewelList, current->getRow(), current->getCol()-1);
				if (logState == TESTING ||logState == FATAL)
					logFile << "Time: " <<time(0) << " Line: 327" << "Current Coor: " << current->getRow() << current->getCol() << " Colour: " << current->getClr() << endl;

				hor +=1;
				delHor[delHorIndex] = current;
				delHorIndex++;
				chain(jewelList, start, current, ver, hor, LEFT, delHor, delVer, delHorIndex, delVerIndex, holes, numHoles);
			}
			else{
				//cout << 2<<endl;
				move = DONE;
				chain(jewelList, start, current, ver, hor, move, delHor, delVer, delHorIndex, delVerIndex, holes, numHoles);
			}
		}
		else{
			move = DONE;
			chain(jewelList, start, current, ver, hor, move, delHor, delVer, delHorIndex, delVerIndex, holes, numHoles);
		}
	}
	else if (move == DONE){
		if (hor >=2 && ver>=2){
			// if there's two or more Jewel's of the same colour not including the starting one in the horizontal and vertical direction
			delHor[delHorIndex] = start;
			delHorIndex++;
			hor++;
			ver++;
			if (logState == TESTING || logState == FATAL){
				logFile << "Time: " << time(0) << " DelHor: " << endl;
				for (int i = 0; delHor[i] != NULL; i++){
					logFile << delHor[i]->getCol() << delHor[i]->getRow() << endl;
				}
				logFile << "delVer: " << endl;
				for (int i = 0; delVer[i] != NULL; i++){
					logFile << delVer[i]->getCol() << delVer[i]->getRow() << endl;
				}
			}

			
		}
		else if (hor>=2){
			// if there's two or more Jewel's of the same colour not including the starting one in the horizontal direction
			//cout << 1 << endl;
			hor++;
			delHor[delHorIndex] = start;
			delHorIndex++;
			for (int i = 0; i<=delVerIndex; i++){
				delVer[i] = NULL;
			}
			if (logState == TESTING || logState == FATAL){
				logFile << "Time: " << time(0) << " DelHor: " << endl;
				for (int i = 0; delHor[i] != NULL; i++){
					logFile << delHor[i]->getCol() << delHor[i]->getRow() << endl;
				}
				logFile << "delVer: " << endl;
				for (int i = 0; delVer[i] != NULL; i++){
					logFile << delVer[i]->getCol() << delVer[i]->getRow() << endl;
				}
			}
			delVerIndex = 0;
			
			ver = 0;
		}
		else if (ver>=2){
			// if there's two or more Jewel's of the same colour not including the starting one in the vertical direction
			//cout << 2<< endl;
			ver++;
			delVer[delVerIndex] = start;
			delVerIndex++;
			for(int i = 0; i<=delHorIndex; i++){
				delHor[i] = NULL;
			}
			if (logState == TESTING || logState == FATAL){
				logFile << "Time: " << time(0) << " DelHor: " << endl;
				for (int i = 0; delHor[i] != NULL; i++){
					logFile << delHor[i]->getCol() << delHor[i]->getRow() << endl;
				}
				logFile << "delVer: " << endl;
				for (int i = 0; delVer[i] != NULL; i++){
					logFile << delVer[i]->getCol() << delVer[i]->getRow() << endl;
				}
			}
			delHorIndex = 0;
			hor = 0;
		}
			
		else{
			//cout << 3 << endl;
			for (int i = 0; i<8; i++){
				delVer[i] = NULL;
				delHor[i] = NULL;
			}
			if (logState == TESTING || logState == FATAL){
				logFile << "Time: " << time(0) << " DelHor: " << endl;
				for (int i = 0; delHor[i] != NULL; i++){
					logFile << delHor[i]->getCol() << delHor[i]->getRow() << endl;
				}
				logFile << "delVer: " << endl;
				for (int i = 0; delVer[i] != NULL; i++){
					logFile << delVer[i]->getCol() << delVer[i]->getRow() << endl;
				}
			}
			delHorIndex = 0;
			delVerIndex = 0;
			ver = 0;
			hor = 0;
		
		}
	}

}

void dropDown(Jewel** jewelList, int holes[][8], int& numHoles){
	// looks for places where there's no jewel and moves the one above below. if its at the top, it creates a new jewel.
	while(numHoles){
		//cerr << numHoles << endl;
		for (int y = 0; y<8; y++){
			for (int x = 0; x<8; x++){
				if (holes[y][x] == 1){
					if (y>0){
						//cerr <<"if: " << y << x << endl;
						if (ifFindJewel(jewelList, y-1, x)){
							//cerr << "if: if: " << y << x << endl;
							findJewel(jewelList, y-1,x)->setRow(y);
							holes[y][x] = 0;
							holes[y-1][x] = 1;
							if (logState == FATAL){
								logFile << "Time: " << time(0) << "Row: " << y << " Holes: " << endl;
								for (int y = 0; y<8; y++){
									for (int x = 0; x<8; x++){
										logFile << holes[y][x] << " ";
									}
									logFile << " " << endl;
								}
							}
						}
						
						//cout<<"yup"<<endl;
					}
					else{
						//cerr << "else: " << y << x << endl;
						Jewel* newJewel = new Jewel(0, x, false);
						bool stop = false;
						int i = 0;
						if (logState == FATAL){
							logFile << "Time: " << time(0) << "Row: " << y << " Holes: " << endl;
							for (int y = 0; y<8; y++){
								for (int x = 0; x<8; x++){
									logFile << holes[y][x] << " ";
								}
								logFile << " " << endl;
							}
						}
						
						while (!stop){
							if (jewelList[i] == NULL){
								//cout<< "new"<<endl;
								jewelList[i] = newJewel;
								numHoles--;
								//cout <<"numHoles: " << numHoles<<endl;
								holes[y][x] = 0;
								stop = true;
							}
							i++;
							if (i == 64){
								stop = true;
							}
						}
					}
				}
			}
		}
	}
	
}
		
void deleteJewel(Jewel** jewelList, Jewel** del, int numDel, int holes[][8], int& numHoles){
	// deallocates memory of a jewel we no longer need (those that have been in a chain).
	while (numDel){
		Jewel* curr = del[numDel-1];
		int i = 0;
		bool done = false;
		//cout << "t" <<endl;
		
		while(!done){
			//out << i << endl;
			if(jewelList[i] != NULL){
			if (jewelList[i]->getRow() == curr->getRow() && jewelList[i]->getCol() == curr->getCol()){
				jewelList[i] = NULL;
				holes[curr->getRow()][curr->getCol()] = 1;
				numHoles++;
				delete curr;
				done = true;
				numDel--;
				i++;
				score++;
				if (logState == FATAL){
					logFile << "Time: " << time(0) << " Line: 558 NullJewels: " << endl;
					for (int i = 0; i<64; i++){
						if (jewelList[i] == NULL){
							logFile << i << endl;
						}
					}
				}
			}
			else
				i++;
		}
		else
			i++;
		}
	}
}

char intToChar(int x){
	switch(x){
		case 0:
			return '0';
		case 1:
			return '1';
		case 2:
			return '2';
		case 3:
			return '3';

		case 4:
			return '4';
		case 5:
			return '5';
		case 6:
			return '6';
		case 7:
			return '7';
		case 8:
			return '8';
		case 9:
			return '9';



	}
}


void printJewels(Jewel** jewelList, int coorX, int coorY){
	// sends the data to the arduino via a system command
	char send[11];
	char sending1[] = "echo \"";
	char sending2[] = "\" > /dev/ttyS1";
	char finalSend[9+7+14+1+2];
	
	if (logState == FATAL){
		logFile << "Time: " << time(0) << " Line: 612" << "Jewels: " << endl;
	}
	// sends the data row by row, and waits in between each row so that the arduino has time to get ready for the next line of data
	for (int y = 0; y<8; y++){
		for (int x = 0; x<8; x++){
			for (int i = 0; i<64; i++){
				if(jewelList[i] != NULL){
				if (jewelList[i]->getRow() == y && jewelList[i]->getCol() == x){
					send[x] = jewelList[i] ->print();
					if (logState == TESTING || logState == FATAL){
						logFile << jewelList[i]->getClr() << "  ";
					}

					//cout << jewelList[i]->getClr() << "  ";
					//cout << finalSend << "  ";
					//system(finalSend);
					
				}
				}
			}
		}
		send[8] = intToChar(coorX);
		send[9] = intToChar(coorY);
		send[10] = 0;
		strcpy(finalSend, sending1);
		strcat(finalSend, send);
		strcat(finalSend, sending2);
		delay();
		system(finalSend);

		if (logState == FATAL){
			logFile << "" << endl;
		}

	}
}

void screenChains(Jewel** jewelList, int& ver, int& hor,  Jewel** delHor, Jewel** delVer, int& delHorIndex, int& delVerIndex, int holes[][8], int& numHoles, bool& chains){
	//looks for chains on the entire board, and doesn't stop until they are all found
	
	bool clean = false;
	while (!clean){
		bool clean1 = false;
		//cerr << 1 << endl;
		for (int i = 0; i<WIDTH*HEIGHT; i++){
			//cerr << i << " " << jewelList[i]->getRow() << jewelList[i]->getCol() << endl;
			chain(jewelList, jewelList[i], jewelList[i], ver, hor, UP, delHor, delVer, delHorIndex, delVerIndex, holes, numHoles);
			//cerr << 2 << endl;
			if (hor >=3 && ver >= 3){
				//cerr << 1 << endl;
				chains = true;
				clean1 = true;
				deleteJewel(jewelList, delHor, delHorIndex, holes, numHoles);
				deleteJewel(jewelList, delVer, delVerIndex, holes, numHoles);
				ver = 0;
				hor = 0;
				for (int d = 0; d<8; d++){
					delHor[d] = NULL;
					delVer[d] = NULL;
				}
				delHorIndex = 0;
				delVerIndex = 0;
				dropDown(jewelList, holes, numHoles);
				numHoles = 0;
			}
			else if (hor>=3){
				//cerr << 2 << endl;
				chains = true;
				clean1 = true;
				deleteJewel(jewelList, delHor, delHorIndex, holes, numHoles);
				ver = 0;
				hor = 0;
				for (int d = 0; d<8; d++){
					delHor[d] = NULL;
					delVer[d] = NULL;
				}
				delHorIndex = 0;
				delVerIndex = 0;
				dropDown(jewelList, holes, numHoles);
				numHoles = 0;
			}
			else if (ver >= 3){
				//cerr << 3 << endl;
				chains = true;
				clean1 = true;
				deleteJewel(jewelList, delVer, delVerIndex, holes, numHoles);
				ver = 0;
				hor = 0;
				for (int d = 0; d<8; d++){
					delHor[d] = NULL;
					delVer[d] = NULL;
				}
				delHorIndex = 0;
				delVerIndex = 0;
				dropDown(jewelList, holes, numHoles);
				numHoles = 0;
			}
		}
		if (!clean1)
			clean = true;
	}		
}	


void stringToIntHelper(const char x, int& value){
	if(x == '0'){
		value = value*10 + 0;
		return;
	}
	else if (x == '1'){
		value = value*10+ 1;
		return;
	}
	else if (x == '2'){
		value = value*10+ 2;
		return;
	}
	else if (x == '3'){
		value = value*10+ 3;
		return;
	}
	else if (x == '4'){
		value = value*10+ 4;
		return;
	}
	else if (x == '5'){
		value = value*10+ 5;
		return;
	}
	else if (x == '6'){
		value = value*10+ 6;
		return;
	}
	else if (x == '7'){
		value = value*10+ 7;
		return;
	}
	else if (x == '8'){
		value = value*10+ 8;
		return;
	}
	else if (x == '9'){
		value = value*10+ 9;
		return;
	}
}	

void parseLine(char line[], int& max_score, int& avg_score, int& avg_time){
	int score= 0;
	int time = 0;
	bool score_done = false;
	for (int i = 0; line[i] != 0; i++){
		if (line[i] == ' '){
			score_done = true;
		}
		else{
			if (!score_done){
				stringToIntHelper(line[i], score);
			}
			else if (score_done){
				stringToIntHelper(line[i], time);
			}
		}
	}
	if (score > max_score)
		max_score = score;
	avg_score += score;
	avg_time += time;
}

void statistics(int score, time_t t1, time_t t2){
	// first adds the current game's data to the file then reads the file to calculate Highscore, average score and average time
	ofstream myfile;
	myfile.open("stats.txt", ios::app);
	myfile  << score << " " << (t2-t1)/60 << char(10);
	myfile.close();
	//cout << int('\r\n') << endl;
	
	int max_score = 0;
	int avg_score = 0;
	int avg_time = 0;
	if (logState == TESTING || logState == FATAL){
		logFile << "Time: " << time(0) << " Line: 790 ReadLines: " << endl;
	}
	
	ifstream infile;
	infile.open("stats.txt");
	int fileLineNumber = 0;
	const int maxLineLength = 100;
	char line[maxLineLength];
	if (!infile.is_open()){
		cout << "unable to compute statistics" << endl;
		return;
	}
	bool done = false;
	while (!done){
		++fileLineNumber;
		if (!infile.getline(line, maxLineLength)){
			if (infile.eof()){
				done = true;
			}
			else {
				cout << "unable to compute statistics" << endl;
				return;
			}
		}
		if (logState == FATAL || logState == TESTING){
			logFile << line << endl;
		}
		//cout << line << endl;
		parseLine(line, max_score, avg_score, avg_time);
		for (int i = 0; i<maxLineLength; i++)
			line[i] = 0;
	}
	avg_score = avg_score/fileLineNumber;
	avg_time = avg_time/fileLineNumber;
	
	cout << "Time: " << (t2-t1)/60 << " min" << endl;
	cout << "Score: " << score << " points" << endl;
	cout << "Highscore: " << max_score << " points" << endl;
	cout << "Average Score: " << avg_score << " points" << endl;
	cout << "Average Time: " << avg_time << " min" << endl;
}
				

int main(){
	
	logFile.open("bej.log", ios::app);
	if (logState == TESTING){
		logFile << "Starting Logging; State: TESTING" << endl;
	}
	
	if (logState == FATAL){
		logFile << "Starting Logging; State: FATAL" << endl;
	}
	
	time_t t1 = time(0);
	time_t t2 = time(0);
	
	int coorX = 0;
	int coorY = 0;
	bool exit_flag;
	srand(time(NULL));
	int holes[8][8];
	for (int i = 0; i<8;i++){
		for (int j = 0; j<8;j++){
			holes[i][j] = 0;
		}
	}
	int numHoles = 0;
	Jewel* jewelList[WIDTH*HEIGHT];
	int filler = 0;
	for (int y = 0; y<HEIGHT; y++){
		for (int x = 0; x<WIDTH; x++){
		jewelList[filler] = new Jewel(y,x,false);
		filler++;
		}
	}
	int ver = 0;
	int hor = 0;
	Jewel* delHor[8];
	for (int i = 0;i<8;i++){
		delHor[i] = NULL;
	}
	Jewel* delVer[8];
	for (int i = 0; i<8;i++){
		delVer[i] = NULL;
	}
	
	int delHorIndex = 0;
	int delVerIndex = 0;
	bool clean = false;
	
	//printJewels(jewelList);

	while (!clean){ // makes sure that there are no already made chains before the game begins
		bool clean1 = false;
		//cerr << 1 << endl;
		for (int i = 0; i<WIDTH*HEIGHT; i++){
			//cerr << i << " " << jewelList[i]->getRow() << jewelList[i]->getCol() << endl;
			chain(jewelList, jewelList[i], jewelList[i], ver, hor, UP, delHor, delVer, delHorIndex, delVerIndex, holes, numHoles);
			//cerr << 2 << endl;
			if (hor >=3 && ver >= 3){
				cerr << 1 << endl;
				clean1 = true;
				deleteJewel(jewelList, delHor, delHorIndex, holes, numHoles);
				deleteJewel(jewelList, delVer, delVerIndex, holes, numHoles);
				ver = 0;
				hor = 0;
				for (int d = 0; d<8; d++){
					delHor[d] = NULL;
					delVer[d] = NULL;
				}
				delHorIndex = 0;
				delVerIndex = 0;
				dropDown(jewelList, holes, numHoles);
				numHoles = 0;
			}
			else if (hor>=3){
				//cerr << 2 << endl;
				clean1 = true;
				deleteJewel(jewelList, delHor, delHorIndex, holes, numHoles);
				ver = 0;
				hor = 0;
				for (int d = 0; d<8; d++){
					delHor[d] = NULL;
					delVer[d] = NULL;
				}
				delHorIndex = 0;
				delVerIndex = 0;
				dropDown(jewelList, holes, numHoles);
				numHoles = 0;
			}
			else if (ver >= 3){
				//cerr << 3 << endl;
				clean1 = true;
				deleteJewel(jewelList, delVer, delVerIndex, holes, numHoles);
				ver = 0;
				hor = 0;
				for (int d = 0; d<8; d++){
					delHor[d] = NULL;
					delVer[d] = NULL;
				}
				delHorIndex = 0;
				delVerIndex = 0;
				dropDown(jewelList, holes, numHoles);
				numHoles = 0;
			}
		}
		if (!clean1)
			clean = true;
	}
	//cout << 6 << endl;
	printJewels(jewelList, coorX, coorY);
	cout << "Begin" << endl;
	score = 0;
	char user = 0;
	bool chains = false;
	exit_flag = false;
	while (!exit_flag){
		cout << "" << endl;
		cin >> user; // Using this because they Joystick won't work :/
		if (user == 'q'){
			t2 = time(0);
			//cerr << "What the fuck " << endl;
			exit_flag = true;
		}
		else if (user == 'w'){
			if (coorY != 0){
				coorY -=1;
			}
		}
		else if (user == 'd'){
			if (coorX != 7){
				coorX+=1;
			}
		}
		else if (user == 's'){
			if (coorY != 7)
				coorY += 1;
		}
		else if (user == 'a'){
			if (coorX != 0)
				coorX -= 1;
		}
		else if (user == 'f'){
			//cout << "Buffering" << endl;
			//printJewels(jewelList, coorX, coorY);
			//cout << coorY << coorX << endl;
			//cout << "Procceed" << endl;
			cin >> user;
			if (user == 'w'){
				if (coorY!= 0){
					Jewel* curr = findJewel(jewelList, coorY, coorX);
					Jewel* tmp = findJewel(jewelList, coorY-1, coorX);
					curr->setRow(coorY-1);
					tmp->setRow(coorY);
					screenChains(jewelList, ver, hor, delHor, delVer, delHorIndex, delVerIndex,holes, numHoles, chains);
					if (!chains){
						cout << "No Chain Made" << endl;
						curr->setRow(coorY);
						tmp->setCol(coorY-1);
					}
					else{
						cout << "Chain Made!" << endl;
					}
					chains = false;
				}
				else
					cout << "You cannot do that." << endl;
			}
			else if (user == 'd'){
				if (coorX != 7){
					Jewel* curr = findJewel(jewelList, coorY, coorX);
					Jewel* tmp = findJewel(jewelList, coorY, coorX+1);
					curr->setCol(coorX+1);
					tmp->setCol(coorX);
					screenChains(jewelList, ver, hor, delHor, delVer, delHorIndex, delVerIndex,holes, numHoles, chains);
					if (!chains){
						cout << "No Chain Made"<< endl;
						curr->setCol(coorX);
						tmp->setCol(coorX+1);
					}
					else{
						cout << "Chain Made!" << endl;
					}
					chains = false;
				
				
				}
				else
					cout << "You cannot do that." << endl;
			}
			else if (user == 's'){
				if (coorY != 7){
					Jewel* curr = findJewel(jewelList, coorY, coorX);
					Jewel* tmp = findJewel(jewelList, coorY+1, coorX);
					curr-> setRow(coorY +1);
					tmp->setRow(coorY);
					screenChains(jewelList, ver, hor, delHor, delVer, delHorIndex, delVerIndex,holes, numHoles, chains);
					if (!chains){
						cout << "No Chain Made" << endl;
						curr->setRow(coorY);
						tmp->setRow(coorY+1);
					}
					else{
						cout << "Chain Made!" << endl;
					}
					chains = false;
				}
				
				else
					cout << "You cannot do that." << endl;
			}
			else if (user == 'a'){
				if (coorX != 0){
					Jewel* curr = findJewel(jewelList, coorY, coorX);
					Jewel* tmp = findJewel(jewelList, coorY, coorX-1);
					curr->setCol(coorX-1);
					tmp->setCol(coorX);
					screenChains(jewelList, ver, hor, delHor, delVer, delHorIndex, delVerIndex,holes, numHoles, chains);
					if (!chains){
						cout << "No Chain Made" << endl;
						curr->setCol(coorX);
						tmp->setCol(coorX-1);
					}
					else{
						cout << "Chain Made!" << endl;
					}
					chains = false;
				
				
				}
				else
					cout << "You cannot do that." << endl;
			}

		}
		else{
			cout << "Invalid Input" << endl;
		}
		if (!exit_flag){
			cout << "Buffering" << endl;
			printJewels(jewelList, coorX, coorY);
			//cout << coorY << coorX << endl;
			cout << "Proceed" << endl;
		}
		
	}
	
	statistics(score, t1, t2);
	
	
	return 0;
}
	
	
	
	
	
