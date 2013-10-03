#include "map.h"


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
MapData::MapData(const char* filename) 
{
	mapfile = filename;
}