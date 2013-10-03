#ifndef MAP_H
#define MAP_H

/**
map.h
load & save mapdata
**/

#include <fstream>

#define WMAX 20;
#define HMAX 20;
#define MAGIC 0xABCDDCBA

class MapData {
private:
	int w;
	int h;		
	char map[HMAX][WMAX];


public:
	MapData(const char* filename);
	void loadMap(int stage);
	void saveMap(const MapData& map);	
	void setMap(int w, int h, char* map[]);
	//for debug
	void printtMap();
};

#endif