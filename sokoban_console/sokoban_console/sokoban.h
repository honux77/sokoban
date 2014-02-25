#pragma once
#include <list> 
#include <algorithm>
#include <iostream>

#define KEY_ESC 27
#define KEY_ARROW 224
#define KEY_UP 72
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_DOWN 80
#define KEY_RESET 114

#define MAINSCREEN \
	"忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式式成式式式式式式式忖"\
	"弛                                                          弛SCORE   00000 弛"\
	"戍式式式式式式式式式式式式式式式式式式式式式式式式式式式式式扣LEFT    00000 弛"\
	"弛                                                          弛TURN    00000 弛"\
	"弛                                                          戍式式式式式式式扣"\
	"弛                                                          弛WASD: MOVE    弛"\
	"弛                                                          弛ESC : RESET   弛"\
	"弛                                                          弛Q   : QUIT    弛"\
	"弛                                                          弛L   : LOAD    弛"\
	"弛                                                          弛S   : SAVE    弛"\
	"弛                                                          戍式式式式式式式扣"\
	"弛                                                          弛              弛"\
	"弛                                                          弛              弛"\
	"弛                                                          弛              弛"\
	"弛                                                          弛              弛"\
	"弛                                                          弛WHAT DO YOU   弛"\
	"弛                                                          弛WANT?         弛"\
	"弛                                                          弛              弛"\
	"弛                                                          弛>             弛"\
	"戌式式式式式式式式式式式式式式式式式式式式式式式式式式式式式扛式式式式式式式旭"

#define RESET \
	"忙式式式式式式式式式式式式式式式式式式忖"\
	"弛 DO YOU REALLY WANT TO RESET (Y/n)? 弛"\
	"戌式式式式式式式式式式式式式式式式式式旭"

namespace glib {
	//for map data
	enum MTYPE{ SPACE, WALL, PLAYER, BALL, EXIT, PL_ON_EX, BL_ON_EX };

	//for dynamic 2d array
	template <typename T> class Array2 {
	public:
		Array2(int width, int height) :
			mWidth(width), mHeight(height) {
			mArr = new T[mWidth * mHeight];
		}
		~Array2() {
			delete[] mArr;
			mArr = 0;
		}
		int getWidth() { return mWidth; }
		int getHeight() { return mHeight; }
		int getSize() { return mWidth * mHeight; }
		T& operator() (int idx) { return mArr[idx]; }
		const T& operator() (int idx) const { return mArr[idx]; }
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

	class Scene {
		friend class Framework;
	public:
		bool changeDepth(int id, int new_depth);
		Array2<char> *getVRAM() const { return mArray; }
		int getWidth() const { return mWidth; }
		int getHeight() const { return mHeight; }
		int getID() { return mID; };
		int getDepth() const { return mDepth; }
		int rowPos() const { return mRow; }
		int colPos() const { return mCol; }
		void fillScene(char c);
		bool isShow() { return mShow; }
		void Show() { mShow = true; }
		void Hide() { mShow = false; }

	private:
		//private constructor 
		Scene(int rowPos, int colPos, int width, int height, int depth);
		Scene(int rowPos, int colPos, int width, int height, int depth, char c);
		Array2<char>* mArray;
		const int mWidth, mHeight, mRow, mCol, mDepth;
		int mID;
		bool mShow;
	};
	void setScene(Scene *src, const char* dst, int y, int x);


	class Framework {
	public:
		static void init(int w, int h);
		static Framework* instance() { return mInstance; }
		int getInput();
		void updateGame();
		Scene *createScene(int rowPos, int colPos, int width, int height, int depth);
		//for debug
		Scene *createScene(int rowPos, int colPos, int width, int height, int depth, char c);
		Scene* findScene(int id);
		void draw();

	private:
		static Framework* mInstance;
		Array2<char> *display;
		int mWidth;
		int mHeight;
		std::list <Scene> SceneList;
		void drawScene(const Scene& s);
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