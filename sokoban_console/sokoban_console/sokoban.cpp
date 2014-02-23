#include <Windows.h>
#include <iostream>
#include <fstream>
#include "sokoban.h"

namespace glib {	
	Framework* Framework::mInstance=NULL;
	void Framework::init(int w, int h)	
	{		
		if(mInstance != NULL)
			return;
		mInstance = new Framework();
		mInstance->mWidth = w;
		mInstance->mHeight = h;
		mInstance->display = new Array2<char> (mInstance->mWidth, mInstance->mHeight);
	}

	void Framework::draw() {
		system("cls");
		for(int i = 0; i < mHeight; i ++) {
			for(int j = 0; j < mWidth; j++)
				std::cout << (*display)(i,j);
		std::cout<<std::endl;
		}
	}
	
	
		int getInput();
		void updateGame();
		
		
	
	bool StageData::readMap(const char * mapfile) {
		std::ifstream maptext(mapfile, std::ifstream::binary);
		if(!maptext.good())
			return false;
		maptext >> std::dec >> mWidth;
		maptext >> std::dec >> mHeight;
		return true;
	}
};

/*
void MapData::initMap()
{	
	int i;
	map = new MTYPE*[h];
	map_bak = new MTYPE*[h];
	for(i = 0; i < h; i++) {
		map[i] = new MTYPE[w];
		map_bak[i] = new MTYPE[w];
	}
}
bool MapData::readMapFromText(const char *text, int &px, int& py)
{
	unsigned int magic;
	int i, j;
	char c;
	

	maptext >> std::hex >> magic;
	assert(magic == MAGIC);
	maptext >> std::dec >> w;
	maptext >> std::dec >> h;
	initMap();
	for (i = 0; i < h;  i++) {		
		for( j = 0; j < w; j++) {			
			maptext >> c;
			switch(c) {
			case '#':
				map[i][j] = WALL;
				break;
			case 'O':
				map[i][j] = BALL;
				break;
			case 'P':
				map[i][j] = PLAYER;
				px = ox =j;
				py = oy =i;
				break;
			case 'X':
				map[i][j] = EXIT;
				break;
			case '-':
				map[i][j] = SPACE;
				break;
			default:
				map[i][j] = SPACE;				
			}
		}		
	}
	copyMap(map_bak,map);
	turn = 1;
	maptext.close();
	return true;
}
void MapData::printMap() {
	int x, y;
	left = 0;
	for (y = 0; y < h; y++) {
		for( x = 0; x < w; x++) {
			switch (map[y][x]) {			
			case WALL:
				std::cout << "#";
				break;
			case PLAYER:			
				std::cout << "P";
				break;
			case PL_ON_EX:
				std::cout << "p";
				break;
			case BALL:
				left++;
				std::cout << "O";
				break;
			case BL_ON_EX:
				std::cout << "o";
				break;
			case EXIT:
				std::cout << "X";
				break;
			case SPACE:
				std::cout << " ";
				break;
			}
		}
		std::cout << std::endl;
	}
}
void MapData::freeMap() {

	int i;
	for(i = 0; i < h; i++) {
		delete[] map[i];
		map[i] = 0;
		delete[] map_bak[i];
		map_bak[i] = 0;
	}
	delete[] map;
	delete[] map_bak;
	map = 0;
	map_bak =0;
}

void MapData::copyMap(MTYPE **dst, MTYPE **src) {
	int i, j;
	for (i = 0; i < h; i++)
		for (j = 0; j < w; j++)
			dst[i][j] = src[i][j];
		
}

void MapData::resetMap(int &x, int &y) {
	copyMap(map, map_bak);
	turn = 1;
	x = ox;
	y = oy;
}

int MapData::leftBall() { return left; }
*/