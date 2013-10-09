#include <iostream>
#include <fstream>

//#define NDEBUG
#include <assert.h>

#include "map.h"
#include "sokoban.h"

/*
class MapData {
private:
int w;
int h;	
char map[HMAX][WMAX];
string 

public:
MapData(const char* filename);	
void loadMap(int stage);
void saveMap(const MapData& map);	
void setMap(int w, int h, char* map[]);
//for debug
void printtMap();
};
*/

void MapData::initMap()
{	
	int i;
	map = new MTYPE*[h];
	for(i = 0; i < h; i++)
		map[i] = new MTYPE[w];
}
void MapData::readMapFromText(const char *text)
{
	unsigned int magic;
	int i;
	char buf[64];
	std::ifstream maptext(text, std::ifstream::binary);
	maptext >> std::hex >> magic;
	assert(magic == MAGIC);
	maptext >> std::dec >> w;
	maptext >> std::dec >> h;

	initMap();	
	for (i = 0; i < h;  i++) {
		maptext.read(map[i],w);
		
			maptext >>c;
			switch(c) {
			case '#':
				map[x][y] = WALL;
				break;
			case 'O':
				map[x][y] = BALL;
				break;
			case 'P':
				map[x][y] = PLAYER;
			case 'X':
				map[x][y] = EXIT;
				break;
			default:
				map[x][y] = SPACE;				
			}
		}
	}
}
void MapData::printtMap() {
	int x, y;
	for (y = 0; y < h; y++) {
		for( x = 0; x < w; x++) {
			switch (map[x][y]) {			
			case WALL:
				std::cout << "#";
				break;
			case PLAYER:
			case (PLAYER | EXIT):
				std::cout << "P";
				break;
			case BALL:
				std::cout << "O";
				break;
			case (BALL | EXIT):
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
	for(i = 0; i < h; i++)
		delete[] map[i];
	delete[] map;
}