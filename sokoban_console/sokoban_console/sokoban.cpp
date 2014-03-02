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
	stage = 1;

}
glib::Array2<char> *Sokoban::Video() {
	return mCurrentMap;
}

void Sokoban::readMap(int stage) {
	char fname[16];
	char c;
	sprintf_s(fname, "%s%d%s", PREFIX, stage, EXT);

	left = 0;
	std::ifstream maptext(fname, std::ifstream::binary);
	maptext >> std::dec >> mapw;
	maptext >> std::dec >> maph;
	mStartMap = new glib::Array2 <char>(mapw, maph);
	mCurrentMap = glib::Framework::instance()->findScene("map")->getVRAM();
	for (int i = 0; i < mapw; i++)
	for (int j = 0; j < maph; j++)
	{
		maptext >> c;
		switch (c) {
		case '#': (*mStartMap)(i,j) = WALL; break;
		case 'O': (*mStartMap)(i,j) = BALL; left++;  break;
		case 'P': (*mStartMap)(i, j) = PLAYER; px = opx = j; py = opy = i;  break;
		case '-': (*mStartMap)(i,j) = SPACE; break;
		case 'X': (*mStartMap)(i,j) = EXIT; break;
		}
	}
	copyMap(mStartMap, mCurrentMap, mapw, maph);

}

void Sokoban::copyMap(glib::Array2 <char> *src, glib::Array2 <char> *dst, int w, int h) {
	for (int i = 0; i < w; i++)
	for (int j = 0; j < h; j++)
		(*dst)(i, j) = (*src)(i, j);
}

void Sokoban::resetMap() {
	copyMap(mStartMap, mCurrentMap, mapw, maph);
	px = opx;
	py = opy;
}

void Sokoban::updateMap(int dx, int dy) {
	int bx, by;
	int fx = px + dx;
	int fy = py + dy;
	switch ((*mCurrentMap)(fy, fx)) {
	case WALL:
		return;
	case SPACE:		
	case EXIT:				
		break;
	case BALL:
		bx = px + 2 * dx;
		by = py + 2 * dy;
		if ((*mCurrentMap)(by, bx) == SPACE) {
			(*mCurrentMap)(by, bx) = BALL;
			(*mCurrentMap)(fy, fx) = SPACE;
		} else if ((*mCurrentMap)(by, bx) == EXIT) {
			(*mCurrentMap)(by, bx) = BL_ON_EX;
			(*mCurrentMap)(fy, fx) = SPACE;
		} else
			return;
		break;
	case BL_ON_EX:
		bx = px + 2 * dx;
		by = py + 2 * dy;
		if ((*mCurrentMap)(by, bx) == SPACE) {
			(*mCurrentMap)(by, bx) = BALL;
			(*mCurrentMap)(fy, fx) = EXIT;			
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
	(*mCurrentMap)(py, px) == PLAYER ? (*mCurrentMap)(py, px) = SPACE : (*mCurrentMap)(py, px) = EXIT;
	px = fx;
	py = fy;
	(*mCurrentMap)(py, px) == EXIT ? (*mCurrentMap)(py, px) = PL_ON_EX : (*mCurrentMap)(py, px) = PLAYER;

}