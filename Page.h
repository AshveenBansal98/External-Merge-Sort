#pragma once
#include "common.h"

class Page{
public:
	vector<int> arr;
	int validEntries;
	//initializes an empty page with invalid entries i.e. INT_MAX
	Page(){
		arr.resize(DISK_PAGE_SIZE, INT_MAX);
		validEntries = 0;
	}

	void writePage();
	void fillPage(vector<int> &v);
};