#pragma once
#include "common.h"
#include "Page.h"

class DiskFile{

public:
	vector<Page> data;
	int totalPages;
	int size; //in terms of number of entries
	
	DiskFile(){
		totalPages = 0;
		size = 0;
	}
	
	DiskFile(int s){
		size = 0;
		totalPages = s;
		data.resize(totalPages);
	}


	void readDiskFile();
	void writeDiskFile();
	void DiskFileCopy( DiskFile &f, int startPage, int endPage);
};