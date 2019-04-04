#pragma once
#include "common.h"
#include "DiskFile.h"
#include "Page.h"
#include "Frame.h"
#include "MainMemory.h"

class ExtMergeSort{
public:
	int runSize; // size of run in terms of number of pages
	int totalPass; // number of passes performed
	int totalRuns;

	ExtMergeSort(){
		runSize = 0;
		totalPass = 0;
		totalRuns = -1;
	}

	void firstPass(DiskFile &inputFile, MainMemory &memory);
	void sortFrame(MainMemory &memory, int f);
	void merge(DiskFile &inputFile, MainMemory &memory, int leftStart, int mid, int rightEnd);
	void twoWaySort(DiskFile &inputFile, MainMemory &memory);
	void Bmerge(DiskFile &inputFile, MainMemory &memory, int leftStart, int B);
	void BWaySort(DiskFile &inputFile, MainMemory &memory, int B);
	void firstPass(DiskFile &inputFile, MainMemory &memory, int B);

};