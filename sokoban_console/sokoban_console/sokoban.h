#ifndef SOKOBAN_H
#define SOKOBAN_H
#include <conio.h>
#include <iostream>

#define KEY_ESC 27
#define KEY_ARROW 224
#define KEY_UP 72
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_DOWN 80
#define KEY_RESET 114

namespace glib {	
	//for map data
	enum MTYPE{SPACE, WALL,PLAYER, BALL, EXIT, PL_ON_EX, BL_ON_EX};
	template <typename T> class Array2 {
	public:
		Array2(int width, int height):
		  mWidth(width), mHeight(height) {
			  mArr = new T[mWidth * mHeight];
		  }
		  ~Array2() {
			  delete[] mArr;
			  mArr = 0;
		  }
		  int getWidth() {return mWidth;}
		  int getHeight() {return mHeight;}
		  T& operator() (int row, int col) {
			  return mArr[row * mWidth + col];
		  }
		  const T& operator() (int row, int col) const {
			  return mArr[row * width + col];
		  }
	private:
		T* mArr;
		int mWidth;
		int mHeight;

	};
	
	class Framework {
	public:		
		static void init(int w, int h);
		static Framework* instance() {return mInstance; }
		//int getInput();
		//void updateGame();
		Array2<char>* getDisplay() {return display;}
		void draw();
	
	private:
		static Framework* mInstance;
		Array2<char> *display;
		int mWidth;
		int mHeight;
	};

	

	class StageData {
	public:		
		bool readMap(const char* mapfile);
		int getLeft();
		int getTurn();
		void getMap(Array2<MTYPE> *map, int *width, int* height);

	private:		
		int mLeft;	
		int mTurn;		
		int mPos; //player position
		int mWidth, mHeight; //map width & height
		Array2 <MTYPE> mMap;
	};
};



#endif