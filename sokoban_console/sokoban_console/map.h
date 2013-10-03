#ifndef MAP_H
#define MAP_H
class Mapdata {
private:
	int num_ball;
	int w;
	int h;
	char* map;
public:
	Mapdata();
	openMap();
	closeMap();
	readMap();
	saveMap();
}
#endif
