#include "common.h"
#include "ExtMergeSort.h"

//creates initial runs of 1 page size
void ExtMergeSort :: firstPass(DiskFile &inputFile, MainMemory &memory){
	
	int frame = -1;
	// load each page to main memory frame and sort
	for(int i = 0; i < inputFile.totalPages; i++){
		frame = memory.loadPage(inputFile, i);
		this->sortFrame(memory, frame);
		memory.writeFrame(inputFile, frame, i);
		memory.freeFrame(frame);
	}
	
	runSize = 1;
	totalPass = 1;
	totalRuns = inputFile.totalPages;
	cout << "First Pass Performed" << endl;
	inputFile.writeDiskFile(); //print file to cout
}

void ExtMergeSort :: firstPass(DiskFile &inputFile, MainMemory &memory, int B){
	vector <int> frames;
	for(int i = 0; i < inputFile.totalPages; i = i + B){
		frames.clear();
		for(int j = 0; (i + j < inputFile.totalPages) && (j < B); j++){
			frames.push_back(memory.loadPage(inputFile, i + j));
		}
		for(int j = 0; j < frames.size()-1; j++){
			while (j < frames.size() - 1 && memory.getValidEntries(frames[j]) != MEM_FRAME_SIZE){
				int temp = memory.getVal(frames[frames.size() -1], 0);
				if (temp == -1){
					frames.resize(frames.size() - 1);
					continue;
				}
				memory.setVal(frames[j], memory.getValidEntries(frames[j]), temp);
				memory.invalidate(frames[frames.size()-1], 0);
			}
			
		}
		int num = 0;
		int validentries = (frames.size()-1)*MEM_FRAME_SIZE + memory.getValidEntries(frames[frames.size() -1]);
		cout << validentries << endl;
		while(num < validentries - 1){
			int minindex = 0;
			int minm = INT_MAX;
			for(int j = num; j < validentries; j++){
				if (memory.getVal(frames[j/MEM_FRAME_SIZE], j%MEM_FRAME_SIZE) < minm){
					minm = memory.getVal(frames[j/MEM_FRAME_SIZE], j%MEM_FRAME_SIZE);
					minindex = j;
				}
			}
			cout << minm << " ";
			if (minm == INT_MAX)
				break;
			swap(memory.data[frames[num/MEM_FRAME_SIZE]].arr[num%MEM_FRAME_SIZE], memory.data[frames[minindex/MEM_FRAME_SIZE]].arr[minindex%MEM_FRAME_SIZE]);
			num++;
		}
		for(int j = 0; j < frames.size(); j++){
			memory.writeFrame(inputFile, frames[j], i + j);
			memory.freeFrame(frames[j]);
		}
		runSize = B;
		totalPass = 1;
		totalRuns = inputFile.totalPages/B;
	}
}

//sorts each frame
void ExtMergeSort :: sortFrame(MainMemory &memory, int f){
	sort(memory.data[f].arr.begin(), memory.data[f].arr.begin()+memory.getValidEntries(f));
}

//Performs merging of 2 runs
void ExtMergeSort :: merge(DiskFile &inputFile, MainMemory &memory, int leftStart, int mid, int rightEnd){

	int finalRunSize = rightEnd - leftStart + 1;
	DiskFile tempFile(finalRunSize);

	bool flag = false;
	int currPage = 0;
	int l = leftStart;
	int r = mid + 1;

	int leftFrame = memory.loadPage(inputFile, l);
	int rightFrame = memory.loadPage(inputFile, r);
	int resFrame = memory.getEmptyFrame();	//frame to store result
	if(leftFrame == -1 || rightFrame == -1 || resFrame == -1){
		cout << "Can't proceed further due to unavailability of memory or invalid Page access" << endl;
		exit(1); 
	}

	int leftIndex = 0;
	int rightIndex = 0;
	int resIndex = 0;

	while(l <= mid && r <= rightEnd){
		if(leftIndex < memory.getValidEntries(leftFrame) && rightIndex < memory.getValidEntries(rightFrame)){	
			int x = memory.getVal(leftFrame, leftIndex);
			int y = memory.getVal(rightFrame, rightIndex); 
			if( x < y){
				memory.setVal(resFrame, resIndex, x);
				flag = true;
				leftIndex++;
			}
			else{
				flag = true;
				memory.setVal(resFrame, resIndex, y);
				rightIndex++;
			}
			resIndex++;
			if(resIndex == MEM_FRAME_SIZE){
				memory.writeFrame(tempFile, resFrame, currPage);
				flag = false;
				memory.freeFrame(resFrame);
				resFrame = memory.getEmptyFrame();
				currPage++; 
				resIndex = 0;
			}
		}

		if(leftIndex == memory.getValidEntries(leftFrame)){
			memory.freeFrame(leftFrame);
			l++;
			if(l <= mid){
				leftFrame = memory.loadPage(inputFile, l);
				leftIndex = 0;
			}	
		}

		if(rightIndex == memory.getValidEntries(rightFrame)){
			memory.freeFrame(rightFrame);
			r++;
			if(r <= rightEnd){
				rightFrame = memory.loadPage(inputFile, r);
				rightIndex = 0;
			}	
		}
	}
	while(l <= mid){
		int x = memory.getVal(leftFrame, leftIndex);
		memory.setVal(resFrame, resIndex, x);
		flag = true;
		leftIndex++;
		resIndex++;
		if(resIndex == MEM_FRAME_SIZE){
			memory.writeFrame(tempFile, resFrame, currPage);
			flag = false;
			memory.freeFrame(resFrame);
			resFrame = memory.getEmptyFrame();
			currPage++; 
			resIndex = 0;
		}
		if(leftIndex == memory.getValidEntries(leftFrame)){
			memory.freeFrame(leftFrame);
			l++;
			if(l <= mid){
				leftFrame = memory.loadPage(inputFile, l);
				leftIndex = 0;
			}	

		}
	}
	while(r <= rightEnd){
		int x = memory.getVal(rightFrame, rightIndex);
		memory.setVal(resFrame, resIndex, x);
		flag = true;
		rightIndex++;
		resIndex++;
		if(resIndex == MEM_FRAME_SIZE){
			memory.writeFrame(tempFile, resFrame, currPage);
			flag = false;
			memory.freeFrame(resFrame);
			resFrame = memory.getEmptyFrame();
			currPage++; 
			resIndex = 0;
		}
		if(rightIndex == memory.getValidEntries(rightFrame)){
			r++;
			memory.freeFrame(rightFrame);
			if(r <= rightEnd){
				rightFrame = memory.loadPage(inputFile, r);
				rightIndex = 0;
			}	

		}
	}
	if(flag)
		memory.writeFrame(tempFile, resFrame, currPage);
	memory.freeFrame(resFrame);
	memory.freeFrame(leftFrame);
	memory.freeFrame(rightFrame);
	inputFile.DiskFileCopy(tempFile, leftStart, rightEnd);

}

void ExtMergeSort :: Bmerge(DiskFile &inputFile, MainMemory &memory, int leftStart, int B){
	int finalRunSize = (B-1)*this->runSize;
	DiskFile tempFile(finalRunSize);
	int currPage = 0;
	int resFrame = memory.getEmptyFrame();
	vector <int> frames;
	for(int i = leftStart; i < min(leftStart + finalRunSize, inputFile.totalPages); i = i + this->runSize){
			frames.push_back(memory.loadPage(inputFile, i));
		
	}
	vector <int> indexes(B-1, 0);
	int minm = INT_MAX;
	int resIndex = 0;
	int minindex = 0;
	// cout << "frame  " <<  frames.size()<< endl;
	for(;;){
		minm = INT_MAX;
		for(int i = 0; i < frames.size(); i++){
			int index = indexes[i];
			if (index/MEM_FRAME_SIZE >= this->runSize || index%MEM_FRAME_SIZE >= memory.getValidEntries(frames[i]))
				continue;
			if (index%MEM_FRAME_SIZE == 0 && index != 0){
				if (i*this->runSize + index/MEM_FRAME_SIZE >= inputFile.totalPages)
					continue;
				memory.freeFrame(frames[i]);
				frames[i] = memory.loadPage(inputFile, i*this->runSize + index/MEM_FRAME_SIZE);
			}
			if (minm > memory.getVal(frames[i], index%MEM_FRAME_SIZE)){
				minm = memory.getVal(frames[i], index%MEM_FRAME_SIZE);
				minindex = i;
				// cout << minm << " " << i << endl;
			}
		}
		// cout << minm << " ";
		if (minm == INT_MAX)
			break;
		memory.setVal(resFrame, resIndex, minm);
		indexes[minindex]++;
		resIndex++;
		if (resIndex == MEM_FRAME_SIZE){
			memory.writeFrame(tempFile, resFrame, currPage);
			currPage++;
			memory.freeFrame(resFrame);
			resFrame = memory.getEmptyFrame();
			resIndex = 0;			
		}	
	}
	if (resIndex){
		memory.writeFrame(tempFile, resFrame, currPage);
	}
	inputFile.DiskFileCopy(tempFile, leftStart, leftStart + finalRunSize - 1);
	memory.freeFrame(resFrame);
	for(int i = 0; i < frames.size(); i++){
			memory.freeFrame(frames[i]);
		
	}
	
}

//Performs 2 way merge sort on inputFile using memory
void ExtMergeSort :: twoWaySort(DiskFile &inputFile, MainMemory &memory){
	
	if(memory.totalFrames < 3)
		cout << "Error: Two way merge sort requires atleast 3 frames" << endl; 
	
	this->firstPass(inputFile, memory);

	int leftStart;
	
	for(this->runSize = 1; this->runSize < inputFile.totalPages; this->runSize *= 2){
		cout << "runSize: " << this->runSize << endl;
		for(leftStart = 0; leftStart < inputFile.totalPages-1; leftStart += 2*this->runSize){
			int mid = leftStart + this->runSize-1;
			int rightEnd = min(leftStart + 2*this->runSize-1, inputFile.totalPages-1);
			cout << "calling merge for < " << leftStart <<", " << mid << ", " << rightEnd << " >" << endl;
			this->merge(inputFile, memory, leftStart, mid, rightEnd);
		}
		totalPass++;
	}

	cout << "Total Passes required: " << totalPass << endl;
}

void ExtMergeSort :: BWaySort(DiskFile &inputFile, MainMemory &memory, int B){
	this->firstPass(inputFile, memory, B);
	inputFile.writeDiskFile();
	int leftStart;
	for(this->runSize = B; this->runSize < inputFile.totalPages; this->runSize *= (B-1)){
		cout << "runSize: " << this->runSize << endl;
		for(leftStart = 0; leftStart < inputFile.totalPages; leftStart += (B-1)*this->runSize){
			this->Bmerge(inputFile, memory, leftStart,B);
		}
		totalPass++;
	}
	
}

