#ifndef SOKOBAN_H
#define SOKOBAN_H
#include <conio.h>
#include <iostream>
#include "map.h"

#define KEY_ESC 27
#define KEY_ARROW 224
#define KEY_UP 72
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_DOWN 80
#define KEY_RESET 114
#define LEFT KEY_LEFT
#define RIGHT KEY_RIGHT
#define UP KEY_UP
#define DOWN KEY_DOWN


int getInput();
void updateGame(int key, MapData& map);

#endif