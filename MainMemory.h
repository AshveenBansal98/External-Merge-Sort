#pragma once
#include "common.h"
#include "DiskFile.h"
#include "Frame.h"
#include "Page.h"

class MainMemory{

public:
	vector<Frame> data;
	int totalFrames;
	vector<bool> valid;

	MainMemory(int s){
		totalFrames = s;
		data.resize(s);
		valid.resize(s, false);
		cout << "Mem created" << endl;
	}

	int loadPage(DiskFile &f, int n);
	int getEmptyFrame();
	int getValidEntries(int f);
	int getVal(int f, int i);
	void setVal(int f, int i, int val);
	void writeFrame(DiskFile &inputFile, int f, int p);
	void freeFrame(int f);
	void invalidate(int f, int i);
};