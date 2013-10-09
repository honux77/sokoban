#ifndef MAP_H
#define MAP_H

/**
map.h
load & save mapdata
**/


#include <fstream>
#include "sokoban.h"
#define MAGIC 0xBAB01209
#define MAX 64

class MapData {

private:	
	void initMap();

public:
	int w;
	int h;
	MTYPE **map;
	
	//MapData(const char* filename);
	//void loadMap(int stage);
	//void saveMap(const MapData& map);	
	//void setMap(int w, int h, char* map[]);
	
	//for debug or test
	void readMapFromText(const char *text);
	void printtMap();
	void freeMap();
};

#endif