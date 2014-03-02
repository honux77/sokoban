#pragma once
#include "framework.h"

#define MAINSCREEN \
	"+-----------------------------------------------------------------------------+\n"\
	"|                                                           | RESET     00000 |\n"\
	"+-----------------------------------------------------------+ TURN      00000 |\n"\
	"|                                                           |                 |\n"\
	"|                                                           |                 |\n"\
	"|                                                           +-----------------+\n"\
	"|                                                           | WASD : MOVE     |\n"\
	"|                                                           | ESC  : RESET    |\n"\
	"|                                                           | Q    : QUIT     |\n"\
	"|                                                           | L    : LOAD     |\n"\
	"|                                                           | V    : SAVE     |\n"\
	"|                                                           +-----------------+\n"\
	"|                                                           |                 |\n"\
	"|                                                           |                 |\n"\
	"|                                                           |                 |\n"\
	"|                                                           |                 |\n"\
	"|                                                           |                 |\n"\
	"|                                                           |                 |\n"\
	"|                                                           |WHAT DO YOU WANT?|\n"\
	"|                                                           |                 |\n"\
	"|                                                           |>                |\n"\
	"|                                                           |> _              |\n"\
	"|                                                           |                 |\n"\
	"+-----------------------------------------------------------¦ª-----------------+\n"

#define RESET \
	"+--------------------------------------+"\
	"|  DO YOU REALLY WANT TO RESET (Y/n)?  |"\
	"+--------------------------------------+"

#define SAVE \
	"+--------------------------------------+"\
	"|  DO YOU REALLY WANT TO SAVE (Y/n)?   |"\
	"+--------------------------------------+"

#define LOAD \
	"+--------------------------------------+"\
	"|  DO YOU REALLY WANT TO LOAD (Y/n)?   |"\
	"+--------------------------------------+"

#define QUIT \
	"+--------------------------------------+"\
	"|  DO YOU REALLY WANT TO QUIT (Y/n)?   |"\
	"+--------------------------------------+"


#define WIDTH 80
#define HEIGHT 24


//map file name
#define PREFIX "stage"
#define EXT ".txt"

//save file name
#define SAVEFILE "save.dat"

//for map data
enum MTYPE{ SPACE='-', WALL='#', PLAYER='P', BALL='O', EXIT='X', PL_ON_EX='p', BL_ON_EX='o' };

class Sokoban {
public:
	long etime; //total elapsed time
	int stage;
	int mapw;
	int maph;
	int turn;
	int left;
	int reset;
	int px, py, opx, opy; //player position

	Sokoban();
	void readMap(int stage);
	void Load(void);
	void Save(void); 
	glib::Array2<char> *Video();
	void resetMap();
	void updateMap(int dx, int dy);

private:
	void copyMap(glib::Array2 <char> *src, glib::Array2 <char> *dst, int w, int h);	
	glib::Array2<char> *mCurrentMap;
	glib::Array2<char> *mStartMap;
};
