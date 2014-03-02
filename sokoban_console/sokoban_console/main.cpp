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


int main() {	
	long start, end;
	int ss, mm, hh;
	char timestr[16];
	using namespace glib;
	Framework::init(WIDTH, HEIGHT);
	Framework* f = Framework::instance();
	//background
	Scene *bg = f->createScene(0, 0, WIDTH, HEIGHT, 1);
	// for stage display
	Scene *stage = f->createScene(1, 30, 10, 1, 2);
	// reset menu
	Scene *menu = f->createScene(10, 5, 40, 3, 15);	
	// map
	Scene *map = f->createScene(5, 10, 20, 10, 10, '#');	
	//time  TIME   00:00:00
	Scene *time = f->createScene(4, 62, 15, 1, 15);
	//cmd
	Scene *cmd = f->createScene(20, 63, 1, 1, 15, '@');

	//draw scene
	setScene(bg, MAINSCREEN, 0, 0);
	setScene(stage, "STAGE 1", 0, 0);
	setScene(menu, RESET, 0, 0);	
	setScene(time, "TIME   00:00:00", 0, 0);

	HWND cwnd = GetConsoleHwnd();

	
	start = clock();
	while (1) {
		end = (clock()  -  start) /1000;
		ss = end % 60;
		mm = (end / 60) % 60;
		hh = (end / 60 / 60);
		sprintf_s(timestr, "TIME   %02d:%02d:%02d", hh, mm, ss);
		setScene(time, timestr, 0, 0);

		f->draw();
		if (GetAsyncKeyState('W'))
			setScene(cmd, "W", 0, 0);
		else if (GetAsyncKeyState('A'))
			setScene(cmd, "A", 0, 0);
		else if (GetAsyncKeyState('S'))
			setScene(cmd, "S", 0, 0);
		else if (GetAsyncKeyState('D'))
			setScene(cmd, "D", 0, 0);

	}
	
	//system("pause");
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