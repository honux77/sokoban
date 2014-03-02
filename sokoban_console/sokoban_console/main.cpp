/***********************************
Very simple sokoban implementation
by Hobytes
1993. 8
(2013.10: some bug fix)
************************************/
#include <Windows.h>
#include "framework.h"
#include "sokoban.h"
#include <time.h>


//it does not need anymore
/*
HWND GetConsoleHwnd(void)
{
	HWND hwndFound;         
	// Fetch current window title.
	wchar_t wtext[32];
	LPWSTR wptr = wtext;	

	wsprintf(wtext, L"Sokoban Game:%d/%d",
		GetTickCount(),
		GetCurrentProcessId());

	SetConsoleTitle(wtext);

	// Ensure window title has been updated.
	Sleep(40);

	// Look for NewWindowTitle.
	hwndFound = FindWindow(NULL, wtext);

	return(hwndFound);
}
*/

#define TIME_START(a) a=clock()
#define TIME_END(b,a) b = (clock() - a)/1000
#define CONVERT(T, S, M, H) do {\
	S = T % 60; \
	M = (T / 60) % 60; \
	H = (T / 60 / 60); } while (0);

//global variables
long start, end;
int dx, dy;


int main() {	
	using namespace glib;	
	
	Framework::init(WIDTH, HEIGHT);
	Framework* f = Framework::instance();
	//set delay, defalut is 100;
	//f->delay=50;
	
	//set scene
	Scene *bg = f->createScene("bg",0, 0, WIDTH, HEIGHT, 1);	
	Scene *stage = f->createScene("stage",1, 30, 10, 1, 2);	
	Scene *menu = f->createScene("menu",10, 5, 40, 3, 15);
	menu->Hide();	
	Scene *map = f->createScene("map",5, 20, 20, 10, 10);		
	Scene *time = f->createScene("time",4, 62, 15, 1, 15);	
	Scene *cmd = f->createScene("cmd",20, 63, 1, 1, 15);
	Scene *score = f->createScene("score", 2, 62, 15, 1, 15);

	//draw scene
	bg->set(MAINSCREEN, 0, 0);
	stage->set("STAGE 1", 0, 0);
	menu->set(RESET, 0, 0);		

	//HWND cwnd = GetConsoleHwnd();
	//add key
	f->addKey('W');
	f->addKey('A');
	f->addKey('S');
	f->addKey('D');	
	f->addKey('R');
	f->addKey('V');
	f->addKey('L');
	f->addKey('Q');
	f->addKey('Y');
	f->addKey('N');

	TIME_START(start);
	while (1) {
		int key = f->getInput();		
		//must implement
		f->updateGame(key);
		f->draw();	
	}	
}


void glib::Framework::updateGame(int key) {	
	static Sokoban soko;
	char timestr[16];	
	int ss, mm, hh;
	TIME_END(end, start);
	CONVERT(end, ss, mm, hh);	
	sprintf_s(timestr, "TIME   %02d:%02d:%02d", hh, mm, ss);	

	enum MENUFLAG {NONE=0X0, RFLAG=0x1, QFLAG=0x2, SFLAG=0x4, LFLAG=0x8};
	static MENUFLAG mflag;
	static Scene *cmd = findScene("cmd");
	static Scene *menu = findScene("menu");
	static Scene *time = findScene("time");
	static Scene *map = findScene("map"); 	
	
	time->set(timestr, 0, 0);

	char keystr[2];
	if (cmd == NULL) {
		printf("UPDATE GAME ERROR");
		return;
	}
	if (key > 0 && !(mflag)) {
		keystr[0] = key;
		keystr[1] = '\0';
		cmd->set(keystr, 0, 0);
	}
		
	switch (key) {
	case 'W': dx = 0; dy = -1; break;
	case 'A':dx = -1; dy = 0; break;
	case 'S':dx = 0; dy = 1; break;
	case 'D':  dx = 1; dy = 0; break;
	case 'R':
		mflag = RFLAG;		
		menu->set(RESET, 0, 0);		
		menu->Show();
		break;
	case 'Q':
		mflag = QFLAG;
		menu->set(QUIT, 0, 0);
		menu->Show();		
		break;
	case 'L':
		mflag = LFLAG;
		menu->set(LOAD, 0, 0);
		menu->Show();
		break;
	case 'V':
		mflag = SFLAG;
		menu->set(SAVE, 0, 0);
		menu->Show();
		break;
	case 'Y':
		switch (mflag) {
		case QFLAG:
			system("cls");
			std::cout << "THANK YOU FOR PLAYING.." << std::endl;
			exit(0);
			break;
		case RFLAG:
			soko.resetMap();
			menu->Hide();
			mflag = NONE;
			break;
		}
	case 'N':
		menu->Hide();
		mflag = NONE;
		break;
	default:
		dx = dy = 0;
	}

	//update map
	soko.updateMap(dx, dy);
	dx = dy = 0;

}
/*

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
