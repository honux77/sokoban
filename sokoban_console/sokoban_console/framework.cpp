#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <list>
#include <Windows.h>
#include <ctime>
#include "framework.h"


namespace glib {	


	//global instance
	Framework* Framework::mInstance=NULL;	
	int seq = 0;

	void Scene::fillScene(char c) {
		Array2 <char> *vram = this->getVRAM();
		for (int i = 0; i < vram->getSize(); i++) {
			(*vram)(i) = c;
		}
	}
	void Scene::set(const char  *str, int y, int x) {
		Array2<char> *vram = getVRAM();
		int idx = 0;
		while (str[idx] != '\0') {
			(*vram)(y, x + idx) = str[idx];
			idx++;
		}
	}

	// depth comparison for Scene
	bool compScene(const Scene &s1, const Scene &s2)
	{
		return s1.getDepth() < s2.getDepth();
	}	
	
	Scene::Scene(const char * name, int rowPos, int colPos, int width, int height, int depth, char c) :
		mWidth(width), mHeight(height), mRow(rowPos), mCol(colPos), mDepth(depth), mShow(true) {
		mArray = new Array2 <char>(width, height);
		mID = ++seq;
		fillScene(c);
		strcpy_s(mName, name);
	}	

	void Framework::init(int w, int h)	
	{		
		if(mInstance != NULL)
			return;
		mInstance = new Framework();
		mInstance->mWidth = w;
		mInstance->mHeight = h;
		mInstance->display = new Array2<char> (mInstance->mWidth, mInstance->mHeight);	
		mInstance->delay = DELAY;
		mInstance->fpsScene = mInstance->createScene("fps",1, 1, 9, 1, 100);
	}

	Scene *Framework::createScene(const char* name, int rowPos, int colPos, int width, int height, int depth) {
		mSceneList.push_front(Scene(name, rowPos, colPos,width, height, depth, ' '));
		return  &*mSceneList.begin();		
		
	};
	//for debug
	Scene* Framework::createScene(const char* name, int rowPos, int colPos, int width, int height, int depth, char c) {
		mSceneList.push_front(Scene(name, rowPos, colPos, width, height, depth, c));
		return &*mSceneList.begin();
	};

	Scene* Framework::findScene(const char* name)
	{		
		std::list <Scene>::iterator itor;
		
		for (itor = mSceneList.begin() ; itor != mSceneList.end(); itor++) {				
			if(strcmp(itor->Name(),name) == 0)
				return &*itor;			
		}
		//can't find
		return NULL;
	};
	
	void Framework::drawScene(const Scene& s) {
		int r = s.rowPos();
		int w = s.colPos();
		Array2 <char> *vram = s.getVRAM();
		for (int i = 0; i < s.getHeight(); i ++)
		for (int j = 0; j < s.getWidth(); j++)
			(*display)(r + i, w + j) = (*vram)(i, j);
	}

	void Framework::draw() {
		char str[10];
		long start, end;
		double dfps;
		start = clock();
		system("cls");
		mSceneList.sort(compScene);
		std::list <Scene>::iterator itor;

		//copy each scene to display
		for (itor = mSceneList.begin(); itor !=mSceneList.end(); itor++) 
			if (itor->isShow())
				drawScene(*itor);
		
		int len = display->getSize();
		char *buf = display->Array1();		
			
		fwrite(buf, len, 1, stdout);
		fflush(stdout);
		Sleep(delay);
		end = clock() - start;
		dfps = 1000.0 / end;
		sprintf_s(str, "fps:%4.2f", dfps);
		if (dfps > FPS)
			delay+=10;
		else if (dfps < (FPS - 1) && delay > 0 )
			delay-=10;
		fpsScene->set(str, 0, 0);
		
	}	
	int Framework::getInput() {
		int ret;
		std::list <int>::iterator i;
		for (i = mKeyList.begin(); i != mKeyList.end(); i++) {
			//if ((ret = GetAsyncKeyState(*i) )& 0x8000)
			if (ret = GetAsyncKeyState(*i))
				return *i;
		}
		//can't find
		return -1;
	}
	void Framework::addKey(int key) {
		mKeyList.push_back(key);			
	}

		void updateGame();		
};


