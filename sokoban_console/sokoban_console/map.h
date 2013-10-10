#ifndef MAP_H
#define MAP_H

/**
map.h
load & save mapdata
**/

#include <fstream>
#define MAGIC 0xBAB01209
#define MAX 64

typedef enum MTYPE{SPACE, WALL,PLAYER, BALL, EXIT, PL_ON_EX, BL_ON_EX};

class MapData {
private:		
	void copyMap(MTYPE **src, MTYPE **dst);
	MTYPE **map_bak;
	int turn;
	int left;
	int ox, oy;

public:
	int w;
	int h;
	MTYPE **map;	

	//void loadMap(int stage);
	//void saveMap(const MapData& map);	
	//void setMap(int w, int h, char* map[]);
	int leftBall();
	void initMap();
	void printMap();
	void freeMap();
	void resetMap(int &x, int &y);

	//for debug or test
	bool readMapFromText(const char *text, int& x, int& y);
	
};

#endif