#pragma once
#include "common.h"

class Frame{
public:
	vector<int> arr;
	int validEntries;
	//initializes an empty page with invalid entries i.e. INT_MAX
	Frame()
	{
		arr.resize(MEM_FRAME_SIZE, INT_MAX);
		validEntries = 0;
	}
	
	//fill Frame with given vector
	void fillFrame(vector<int> &v);
	void printFrame();
};