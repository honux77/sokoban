#pragma once
#include "framework.h"
#define OPENING \
	"+-----------------------------------------------------------------------------+\n"\
	"|         Once upon a time in the terminal land,                              |\n"\
	"|         There were boxes that want to go back to their positions.           |\n"\
	"|         Let it move..                                                       |\n"\
	"|                                                                             |\n"\
	"|                                                                             |\n"\
	"|                                                                             |\n"\
	"|                                                                             |\n"\
	"|                                                                             |\n"\
	"|                                                                             |\n"\
	"|                                                                             |\n"\
	"|                                                                             |\n"\
	"|                                                                             |\n"\
	"|                                                                             |\n"\
	"|                                                                             |\n"\
	"|                                                                             |\n"\
	"|                                                                             |\n"\
	"|                                                                             |\n"\
	"|                                                                             |\n"\
	"|                                                                             |\n"\
	"|                                                                             |\n"\
	"|                                                                             |\n"\
	"|                                                                             |\n"\
	"+-----------------------------------------------------------------------------+\n"\
	


#define MAINSCREEN \
	"+-----------------------------------------------------------------------------+\n"\
	"|                        STAGE                              | RESET           |\n"\
	"+-----------------------------------------------------------+ TURN            |\n"\
	"|                                                           | LEFT            |\n"\
	"|                                                           | SCORE           |\n"\
	"|                                                           | TIME            |\n"\
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

#define CLEAR \
	"+--------------------------------------+"\
	"|  STAGE CLEAR!  GO TO NEXT STAGE!!    |"\
	"+--------------------------------------+"

#define WIDTH 80
#define HEIGHT 24
#define END 4


//map file name
#define PREFIX "stage"
#define EXT ".txt"

//save file name
#define SAVEFILE "save.dat"

//for map data
enum MTYPE{ SPACE='-', WALL='#', PLAYER='P', BALL='O', EXIT='X', PL_ON_EX='p', BL_ON_EX='o' };

struct record {
	int stage;
	int mapw;
	int maph;
	int turn;
	int left;
	int reset;
	int px, py, opx, opy; //player position
	int ss, mm, hh;
	int score;
};
class Sokoban {
public:
	record r;
	Sokoban();
	void readMap(int stage);
	void Load(void);
	void Save(void); 	
	void resetMap();
	void updateMap(int dx, int dy);
	void end();
	void clear();
private:
	void copyMap(glib::Array2 <char> *src, glib::Array2 <char> *dst, int w, int h);	
	glib::Array2<char> *mCurrentMap;
	glib::Array2<char> *mStartMap;
};
