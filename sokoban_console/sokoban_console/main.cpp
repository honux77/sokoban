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

	Scene *stage = f->createScene("stage",1, 31, 2, 1, 2,'0');	
	Scene *menu = f->createScene("menu",10, 5, 40, 3, 15);	
	Scene *map = f->createScene("map",5, 20, 15, 15, 10);		
	Scene *reset = f->createScene("reset", 1, 70, 8, 1, 15,'0');
	Scene *turn = f->createScene("turn", 2, 70, 8, 1, 15, '0');
	Scene *left = f->createScene("left", 3, 70, 8, 1, 15, '0');
	Scene *score = f->createScene("score", 4, 70, 8, 1, 15,'0');
	Scene *time = f->createScene("time",5, 70, 8, 1, 15);	
	Scene *cmd = f->createScene("cmd",20, 63, 1, 1, 15);
	
	
	


	//opening
	bg->set(OPENING);
	bg->moveScene(0, 0, 100);
	f->draw();
	system("pause");
	bg->moveScene(0, 0, -100);

	//draw scene
	bg->set(MAINSCREEN, 0, 0);
	menu->Hide();

	//comment, no need to get handle
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
	static Sokoban soko(start);
	char strbuf[32];	
	char keystr[2];	
	

	enum MENUFLAG {NONE=0X0, RFLAG=0x1, QFLAG=0x2, SFLAG=0x4, LFLAG=0x8};
	static MENUFLAG mflag;
	static Scene *cmd = findScene("cmd");
	static Scene *menu = findScene("menu");
	static Scene *time = findScene("time");	
	static Scene *reset = findScene("reset");
	static Scene *turn = findScene("turn");
	static Scene *left = findScene("left");
	static Scene *stage = findScene("stage");
	static Scene *ss = findScene("score");
	
		
	//set time
	TIME_END(end, soko.r.starttime);
	CONVERT(end, soko.r.ss, soko.r.mm, soko.r.hh);
	sprintf_s(strbuf, "%02d:%02d:%02d", soko.r.hh, soko.r.mm, soko.r.ss);
	time->set(strbuf);

	sprintf_s(strbuf, "%02d", soko.r.stage);
	stage->set(strbuf);	

	sprintf_s(strbuf, "%08d", soko.r.left);
	left->set(strbuf);
	
	
	keystr[1] = '\0';	
	keystr[0] = key;
	if (key > 0) cmd->set(keystr);	
		
	switch (key) {
	case 'W': dx = 0; dy = -1; soko.r.turn++; break;
	case 'A': dx = -1; dy = 0; soko.r.turn++; break;
	case 'S': dx = 0; dy = 1; soko.r.turn++; break;
	case 'D': dx = 1; dy = 0; soko.r.turn++; break;
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
			system("pause");
			exit(0);
			break;
		case RFLAG:
			soko.r.reset++;
			sprintf_s(strbuf, "%08d", soko.r.reset);
			reset->set(strbuf);
			soko.resetMap();
			menu->Hide();
			mflag = NONE;
			break;
		case SFLAG:
			soko.Save();			
			menu->Hide();
			mflag = NONE;
			break;
		case LFLAG:
			soko.Load();
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

	sprintf_s(strbuf, "%08d", soko.r.turn);
	turn->set(strbuf);
	sprintf_s(strbuf, "%08d", soko.r.left);
	left->set(strbuf);	
	
	int dscore = (soko.r.turn + soko.r.reset) /10 +1;
	soko.r.score -= dscore;		
	
	if (soko.r.score <= (soko.r.stage * 1000))
		soko.r.score = soko.r.stage *1000;
	sprintf_s(strbuf, "%08d", soko.r.score);
	ss->set(strbuf);
	 
	if (soko.r.left == 0)
		soko.clear();

}