#include <Windows.h>
#include <iostream>
#include <fstream>
#include "sokoban.h"
#include "framework.h"

/*
class Sokoban {
public:
Sokoban();
void readMap(int stage);
void Load();
void Save();
private:
Record *record;
MTYPE *mStartMap;
MTYPE *mCurrentMap;
};
*/
Sokoban::Sokoban() {
	readMap(1);
	r.stage = 1;
}

void Sokoban::Save() {
	std::ofstream savefile(SAVEFILE, std::ofstream::binary);
	/*
	int stage;
	int mapw;
	int maph;
	int turn;
	int left;
	int reset;
	int px, py, opx, opy; //player position
	int ss, mm, hh;
	int score;
	*/

	//save class
	savefile.write((char*)&r, sizeof(record));
	//save map
	for (int i = 0; i < r.mapw* r.maph; i++)
		savefile << (*mStartMap)(i);
	for (int i = 0; i < r.maph; i++) {
		for (int j = 0; j < r.mapw; j++) {
			savefile << (*mCurrentMap)(i, j);
		}
	}		
	savefile.close();	
}

void Sokoban::Load() {
	delete mStartMap;
	std::ifstream savefile(SAVEFILE, std::ifstream::binary);	
	savefile.read((char*)&r, sizeof(record));	
	mStartMap = new glib::Array2 <char>(r.mapw, r.maph);
	mCurrentMap = glib::Framework::instance()->findScene("map")->getVRAM();
	for (int i = 0; i < r.mapw* r.maph; i++)
		savefile >> (*mStartMap)(i);
	for (int i = 0; i < r.maph; i++) {
		for (int j = 0; j < r.mapw; j++) {
			savefile >> (*mCurrentMap)(i, j);
		}
	}
	savefile.close();
}

void Sokoban::readMap(int stage) {
	char fname[16];
	char c;
	sprintf_s(fname, "%s%d%s", PREFIX, stage, EXT);

	r.left = 0;
	std::ifstream maptext(fname, std::ifstream::binary);
	maptext >> std::dec >> r.mapw;
	maptext >> std::dec >> r.maph;
	mStartMap = new glib::Array2 <char>(r.mapw, r.maph);
	mCurrentMap = glib::Framework::instance()->findScene("map")->getVRAM();
	for (int i = 0; i < r.maph; i++)
	for (int j = 0; j < r.mapw; j++)
	{
		maptext >> c;
		switch (c) {
		case '#': (*mStartMap)(i,j) = WALL; break;
		case 'O': (*mStartMap)(i,j) = BALL; r.left++;  break;
		case 'o': (*mStartMap)(i, j) = BL_ON_EX; break;
		case 'P': (*mStartMap)(i, j) = PLAYER; r.px = r.opx = j; r.py = r.opy = i;  break;
		case '-': (*mStartMap)(i,j) = SPACE; break;
		case 'X': (*mStartMap)(i,j) = EXIT; break;
		}
	}
	copyMap(mStartMap, mCurrentMap, r.mapw, r.maph);

}

void Sokoban::copyMap(glib::Array2 <char> *src, glib::Array2 <char> *dst, int w, int h) {
	for (int i = 0; i < h; i++)
	for (int j = 0; j < w; j++)
		(*dst)(i, j) = (*src)(i, j);
}

void Sokoban::resetMap() {
	copyMap(mStartMap, mCurrentMap, r.mapw, r.maph);
	r.px = r.opx;
	r.py = r.opy;
}

void Sokoban::updateMap(int dx, int dy) {
	int bx, by;
	int fx = r.px + dx;
	int fy = r.py + dy;
	switch ((*mCurrentMap)(fy, fx)) {
	case WALL:
		return;
	case SPACE:		
	case EXIT:				
		break;
	case BALL:
		bx = r.px + 2 * dx;
		by = r.py + 2 * dy;
		if ((*mCurrentMap)(by, bx) == SPACE) {
			(*mCurrentMap)(by, bx) = BALL;
			(*mCurrentMap)(fy, fx) = SPACE;
		} else if ((*mCurrentMap)(by, bx) == EXIT) {
			(*mCurrentMap)(by, bx) = BL_ON_EX; r.left--;
			(*mCurrentMap)(fy, fx) = SPACE;
		} else
			return;
		break;
	case BL_ON_EX:
		bx = r.px + 2 * dx;
		by = r.py + 2 * dy;
		if ((*mCurrentMap)(by, bx) == SPACE) {
			(*mCurrentMap)(by, bx) = BALL;
			(*mCurrentMap)(fy, fx) = EXIT;	r.left++;
		} else if ((*mCurrentMap)(by, bx) == EXIT) {
			(*mCurrentMap)(by, bx) = BL_ON_EX;			
			(*mCurrentMap)(fy, fx) = EXIT;			
			break;
		} else
			return;
		break;
	default:
		return;
	}
	//move player
	(*mCurrentMap)(r.py, r.px) == PLAYER ? (*mCurrentMap)(r.py, r.px) = SPACE : (*mCurrentMap)(r.py, r.px) = EXIT;
	r.px = fx;
	r.py = fy;
	(*mCurrentMap)(r.py, r.px) == EXIT ? (*mCurrentMap)(r.py, r.px) = PL_ON_EX : (*mCurrentMap)(r.py, r.px) = PLAYER;

}void Sokoban::clear() {
	using namespace glib;
	char buf[9];
	if (r.stage == 1)
		r.score = 0;
	r.stage++;
	int cscore = r.stage * 10000 - r.turn * r.reset - r.hh * 3600 - r.mm * 60 - r.ss;
	if (cscore > 0)
		r.score += cscore;
	
	Framework* f = Framework::instance();
	static Scene *menu = f->findScene("menu");	
	static Scene *ss = f->findScene("score");
	menu->set(CLEAR);
	menu->Show();
	f->draw();
	Sleep(2000);
	menu->Hide();
	sprintf_s(buf,"%08d", r.score);
	ss->set(buf);

	//delete startmap
	delete mStartMap;
	readMap(r.stage);

	if (r.stage > END)
		end();
}

void Sokoban::end() {
	char buf[64];
	using namespace glib;
	Framework* f = Framework::instance();
	Scene* bg = f->findScene("bg");
	bg->moveScene(0, 0, 100);
	sprintf_s(buf, "Thank");
	bg->set(OPENING);
	bg->set("All the boxes returns their positions.", 5, 10);
	bg->set("And they lived happily ever after.",6, 10);
	bg->set("Thanks for playing...",7, 10);
	
	sprintf_s(buf, "Turn: %d", r.turn);
	bg->set(buf, 9, 10);

	sprintf_s(buf, "Time: %02d:%02d:%02d", r.hh, r.mm, r.ss);
	bg->set(buf, 10, 10);	

	sprintf_s(buf, "Final Score: %08d", r.score);
	bg->set(buf, 11, 10);
	
	bg->set("Thanks for playing...", 13, 10);
	bg->set("Programmed by Hoyoung Jung", 14, 10);
	bg->set("Special Thanks to: Prof. sm9, Prof. C2H5OH, and YOU.", 15, 10);
	f->draw();
	system("pause");
	exit(0);
	
}