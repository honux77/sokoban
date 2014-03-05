#include <Windows.h>
#include <iostream>
#include <fstream>
#include <ctime>
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
Sokoban::Sokoban(long start) {
	r.score = 0;
	r.starttime = start;
	mCurrentMap = glib::Framework::instance()->findScene("map")->getVRAM();
	readMap(1);	
	
}

void Sokoban::Save() {
	std::ofstream savefile(SAVEFILE, std::ofstream::binary);

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
	
	
	std::ifstream savefile(SAVEFILE, std::ifstream::binary);	
	if (savefile.fail())
		return;
	savefile.read((char*)&r, sizeof(record));	
	//Delete current mapArray because map size will change.
	delete mStartMap;
	mStartMap = new glib::Array2 <char>(r.mapw, r.maph);	
	mCurrentMap->fill(' ');
	for (int i = 0; i < r.mapw* r.maph; i++)
		savefile >> (*mStartMap)(i);
	for (int i = 0; i < r.maph; i++) {
		for (int j = 0; j < r.mapw; j++) {
			savefile >> (*mCurrentMap)(i, j);
		}
	}
	savefile.close();
	//recalculate start time
	r.starttime = clock() - (r.hh * 3600 + r.mm * 60 + r.ss) * 1000;
	
}

void Sokoban::readMap(int stage) {
	char fname[16];
	char c;
	sprintf_s(fname, "%s%d%s", PREFIX, stage, EXT);

	r.oleft = 0;
	std::ifstream maptext(fname, std::ifstream::binary);
	if (maptext.fail())
		end();
	r.score += stage * 10000;
	r.stage = stage;

	maptext >> std::dec >> r.mapw;
	maptext >> std::dec >> r.maph;
	mCurrentMap->fill(' ');
	mStartMap = new glib::Array2 <char>(r.mapw, r.maph);	
	
	for (int i = 0; i < r.maph; i++)
	for (int j = 0; j < r.mapw; j++)
	{
		maptext >> c;
		switch (c) {
		case '#': (*mStartMap)(i,j) = WALL; break;
		case 'O': (*mStartMap)(i,j) = BALL; r.oleft++;  break;
		case 'o': (*mStartMap)(i,j) = BL_ON_EX; break;
		case 'P': (*mStartMap)(i,j) = PLAYER; r.px = r.opx = j; r.py = r.opy = i;  break;
		case '-': (*mStartMap)(i,j) = SPACE; break;
		case 'X': (*mStartMap)(i,j) = EXIT; break;
		}
	}
	r.left = r.oleft;
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
	r.left = r.oleft;
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
	r.stage++;
	
	Framework* f = Framework::instance();
	static Scene *menu = f->findScene("menu");	
	menu->set(CLEAR);
	menu->Show();
	f->draw();
	Sleep(1000);
	menu->Hide();	

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