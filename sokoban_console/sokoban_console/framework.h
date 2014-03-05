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

#define SHOWFPS 1
//dfault delay
#define DELAY 100
//LIMIT FPS
#define FPS 8

namespace glib {	

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
		int Y(int i) { return i / mWidth; }
		int X(int i) { return i % mWidth; }
		int getWidth() { return mWidth; }
		int getHeight() { return mHeight; }
		int getSize() { return mWidth * mHeight; }
		T* Array1() { return mArr; }
		T& operator() (int idx) { return mArr[idx]; }
		const T& operator() (int idx) const { return mArr[idx]; }
		T& operator() (int row, int col) {
			return mArr[row * mWidth + col];
		}
		const T& operator() (int row, int col) const {
			return mArr[row * width + col];
		}
		void fill(T t) {
			for (int i = 0; i < mWidth * mHeight; i++)
				mArr[i] = t;
		}
	private:
		T* mArr;
		int mWidth;
		int mHeight;

	};

	class Scene {
		friend class Framework;
	public:		
		Array2<char> *getVRAM() const { return mArray; }
		Array2<char> *getVRAM() { return mArray; }
		int getWidth() const { return mWidth; }
		int getHeight() const { return mHeight; }
		int getID() { return mID; };
		int getDepth() const { return mDepth; }
		int rowPos() const { return mRow; }
		int colPos() const { return mCol; }
		void moveScene(int delta_row, int delta_col, int delta_depth) {
			mRow += delta_row;
			mCol += delta_col;
			mDepth += delta_depth;
		}
		const char* Name() { return mName; }
		void fillScene(char c);
		void set(const char *str, int y, int x);
		void set(const char *str) { set(str,0, 0); }
		bool isShow() { return mShow; }
		void Show() { mShow = true; }
		void Hide() { mShow = false; }


	private:
		//private constructor 		
		Scene(const char *name, int rowPos, int colPos, int width, int height, int depth, char c);
		Array2<char>* mArray;
		const int mWidth, mHeight;
		int mID, mRow, mCol, mDepth;
		bool mShow;
		char mName[16];
	};
	


	class Framework {

	public:
		static void init(int w, int h);
		static Framework* instance() { return mInstance; }
		int getInput();
		void updateGame();
		Scene *createScene(const char* name, int rowPos, int colPos, int width, int height, int depth);
		//for debug
		Scene *createScene(const char* name, int rowPos, int colPos, int width, int height, int depth, char c);
		Scene* findScene(const char *name);
		void updateGame(int key);
		void draw();
		int delay;
		void addKey(int listenkey);
	private:
		static Framework* mInstance;
		Array2<char> *display;
		Scene *fpsScene;
		int mWidth;
		int mHeight;
		std::list <Scene> mSceneList;
		std::list <int> mKeyList;		
		void drawScene(const Scene& s);
	};	
};