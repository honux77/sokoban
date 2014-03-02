#pragma once

#define MAINSCREEN \
	"+-----------------------------------------------------------------------------+\n"\
	"|                                                           | SCORE     00000 |\n"\
	"+-----------------------------------------------------------+ LEFT      00000 |\n"\
	"|                                                           | TURN      00000 |\n"\
	"|                                                           |                 |\n"\
	"|                                                           +-----------------+\n"\
	"|                                                           | WASD : MOVE     |\n"\
	"|                                                           | ESC  : RESET    |\n"\
	"|                                                           | Q    : QUIT     |\n"\
	"|                                                           | L    : LOAD     |\n"\
	"|                                                           | S    : SAVE     |\n"\
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
	"+-----------------------------------------------------------��-----------------+\n"

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


class Sokoban {
public:
	void readMap(const char *mapfile);
};