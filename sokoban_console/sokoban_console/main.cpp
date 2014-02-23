/***********************************
Very simple sokoban implementation 
by Hobytes
1993. 8
(2013.10: some bug fix)
************************************/
#include "sokoban.h"
#include <Windows.h>

int main() {
	using namespace glib;
	Framework::init(40,20);
	Framework* f = Framework::instance();
	Array2<char> *disp = f->getDisplay();
	int w = disp->getWidth();
	int h = disp->getHeight();
	for (int i =0; i < h; i ++)
		for (int j = 0; j < w; j ++)
			(*disp)(i,j) = '*';
	f->draw();
}

//start data;
//int px, py,turn;

/*
int main() {
	char filename[20];
	using namespace std; 	
	using namespace glib;
	Framework f(20,20);
			
	
	//bool exist;
	cout << "Simple Push Push Game by Hobytes ver. 0.3" << endl;
	cout << "Press space bar to start..." << endl;	
	turn = 1;
	stage = 1;
	key = getInput();

	while (true) {		
		sprintf_s(filename,"stage%d.txt",stage);
		if(!md.readMapFromText(filename, px, py)) break;			
		Sleep(1500);
		cout << endl << endl << "Stage " <<stage << endl;			
		while(true) {			
			system("cls");
			md.printMap();
			if((left = md.leftBall()) == 0) {				
				break;
			}
			cout <<"TURN: " <<turn <<"\tLEFT BALL: " << left << endl;
			cout <<"(R) for Reset, (Esc) for escape! "<< endl;

			key = getInput();
			if (key == KEY_ESC) {
				std::cout << "Thanks for playing!!" << endl;
				return 0;				
			}
			if (key == KEY_RESET) {
				md.resetMap(px, py);
				turn = 1;
				continue;
			}
			updateGame(key, md);		
		}
		//clear 1 stage
		turn = 1;
		md.freeMap();
		std::cout << "Clear Stage " << stage <<endl;
		Sleep(1500);
		stage++;			
	}
	system("cls");
	std::cout << "You cleared All of Games" <<endl;
	std::cout << "Thanks for playing!!" << endl;
	Sleep(1500);
	return 0;
}

int getInput() {
	int key = _getch();		
	if(key == KEY_ARROW )
		key = _getch();
	return key;
}
void updateGame(int key, MapData& md) {

	int dx, dy, tx, ty,bx, by;
	dx = dy = 0;
	turn++;
	switch(key) {
	case KEY_LEFT:
		dx = -1;
		break;
	case KEY_RIGHT:
		dx = 1;
		break;
	case KEY_DOWN:
		dy = 1;
		break;
	case KEY_UP:
		dy = -1;
		break;
	default: //do nothing
		return;
	}

	//next way
	tx = px + dx;
	ty = py + dy;
	bx = tx + dx;
	by = ty + dy;

	if (tx < 0 || tx >= md.w || ty < 0 || ty >= md.h) return; //out of range

	switch (md.map[ty][tx]) {
	case SPACE:
		md.map[ty][tx] = PLAYER;				
		break;
	case EXIT:
		md.map[ty][tx] = PL_ON_EX;	
		break;
	case BALL:		
		if(md.map[by][bx] == SPACE) {
			md.map[by][bx] = BALL;			
		} else if(md.map[by][bx] == EXIT) {
			md.map[by][bx] = BL_ON_EX;					
		} else return;
		md.map[ty][tx] = PLAYER;				
		break;
	case BL_ON_EX:		
		if(md.map[by][bx] == SPACE) {
			md.map[by][bx] = BALL;			
		} else if(md.map[by][bx] == EXIT) {
			md.map[by][bx] = BL_ON_EX;						
		} else return;			
		md.map[ty][tx] = PL_ON_EX;	
		break;
	case WALL: //impossible to move	
	default: return;
	}
	md.map[py][px] == PL_ON_EX ? md.map[py][px] = EXIT: md.map[py][px] = SPACE;	
	px = tx, py = ty;
}

*/