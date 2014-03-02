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
	void setScene(Scene *src, const char* dst, int y, int x) {
		Array2<char> *vram = src->getVRAM();
		int idx = 0;
		while (dst[idx] != '\0') {
			(*vram)(y, x + idx) = dst[idx];
			idx++;
		}
	}

	// depth comparison for Scene
	bool compScene(const Scene &s1, const Scene &s2)
	{
		return s1.getDepth() < s2.getDepth();
	}	
	
	Scene::Scene(int rowPos, int colPos, int width, int height, int depth, char c) :
		mWidth(width), mHeight(height), mRow(rowPos), mCol(colPos), mDepth(depth), mShow(true) {
		mArray = new Array2 <char>(width, height);
		mID = ++seq;
		fillScene(c);
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
		mInstance->fpsScene = mInstance->createScene(1, 1, 9, 1, 100);
	}

	Scene *Framework::createScene(int rowPos, int colPos, int width, int height, int depth) {
		SceneList.push_front(Scene(rowPos, colPos,width, height, depth, ' '));
		return  &*SceneList.begin();		
		
	};
	//for debug
	Scene* Framework::createScene(int rowPos, int colPos, int width, int height, int depth, char c) {
		SceneList.push_front(Scene(rowPos, colPos, width, height, depth, c));
		return &*SceneList.begin();
	};

	Scene* Framework::findScene(int id)
	{		
		std::list <Scene>::iterator itor;
		
		for (itor = SceneList.begin() ; itor != SceneList.end(); itor++) {				
			if (itor->getID() == id)
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
		SceneList.sort(compScene);
		std::list <Scene>::iterator itor;

		//copy each scene to display
		for (itor = SceneList.begin(); itor != SceneList.end(); itor++) 
			if (itor->isShow())
				drawScene(*itor);
		
		int len = display->getSize();
		char* buf = new char[len];		
		
		for (int i = 0; i < len; i++) 			
			buf[i] = (*display)(i);
			
		fwrite(buf, len, 1, stdout);
		fflush(stdout);
		Sleep(delay);
		end = clock() - start;
		dfps = 1000.0 / end;
		sprintf(str, "fps:%4.2f", dfps);
		setScene(fpsScene, str, 0, 0);
		delete[] buf;
	}	
		int getInput();
		void updateGame();		
	
	bool StageData::readMap(const char * mapfile) {
		std::ifstream maptext(mapfile, std::ifstream::binary);
		if(!maptext.good())
			return false;
		maptext >> std::dec >> mWidth;
		maptext >> std::dec >> mHeight;
		return true;
	}
};

