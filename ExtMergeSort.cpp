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
	// cout << "First Pass Performed" << endl;
	// inputFile.writeDiskFile(); //print file to cout
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
					memory.freeFrame(frames[frames.size()-1]);
					frames.resize(frames.size() - 1);
					continue;
				}
				memory.setVal(frames[j], memory.getValidEntries(frames[j]), temp);
				memory.invalidate(frames[frames.size()-1], 0);
			}
			
		}
		int num = 0;
		int validentries = (frames.size()-1)*MEM_FRAME_SIZE + memory.getValidEntries(frames[frames.size() -1]);
		// cout << validentries << endl;
		while(num < validentries - 1){
			int minindex = 0;
			int minm = INT_MAX;
			for(int j = num; j < validentries; j++){
				if (memory.getVal(frames[j/MEM_FRAME_SIZE], j%MEM_FRAME_SIZE) < minm){
					minm = memory.getVal(frames[j/MEM_FRAME_SIZE], j%MEM_FRAME_SIZE);
					minindex = j;
				}
			}
			// cout << minm << " ";
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


void ExtMergeSort :: Bmerge(DiskFile &inputFile, MainMemory &memory, int leftStart, int B){
	int finalRunSize = ((B-2)*this->runSize)/2;
	DiskFile tempFile(finalRunSize);
	int currPage = 0;
	// int resFrame = memory.getEmptyFrame();
	vector < vector <int> > frames(B/2, vector <int> (2));
	for(int i = 0; i < (B - 2)/2; i++){
		if (leftStart + i*this->runSize < inputFile.totalPages)
			frames[i][0] = memory.loadPage(inputFile, leftStart + i*this->runSize);
		else
			frames[i][0] = memory.getEmptyFrame();
		if (leftStart + i*this->runSize + 1 < inputFile.totalPages)
			frames[i][1] = memory.loadPage(inputFile, leftStart + i*this->runSize+1);
		else
			frames[i][1] = memory.getEmptyFrame();
	}
	frames[B/2 - 1][0] = memory.getEmptyFrame();
	frames[B/2 - 1][1] = memory.getEmptyFrame();
	vector <int> active(B/2, 0);
	vector <int> indexes(B/2, 0);
	int minm = INT_MAX;
	int minindex = 0;
	// cout << "frame  " <<  frames.size()<< endl;
	for(;;){
		minm = INT_MAX;
		for(int i = 0; i < frames.size()-1; i++){
			int index = indexes[i];
			if (index%MEM_FRAME_SIZE >= memory.getValidEntries(frames[i][active[i]]))
				continue;
			if (minm > memory.getVal(frames[i][active[i]], index%MEM_FRAME_SIZE)){
				minm = memory.getVal(frames[i][active[i]], index%MEM_FRAME_SIZE);
				minindex = i;
				// cout << minm << " " << i << " " << active[i] << endl;
			}
		}
		// cout << minm << endl;
		if (minm == INT_MAX)
			break;
		memory.setVal(frames[B/2-1][active[B/2-1]], indexes[B/2-1], minm);
		indexes[minindex]++;
		if (indexes[minindex]%MEM_FRAME_SIZE == 0){
			memory.freeFrame(frames[minindex][active[minindex]]);	
			if (leftStart + minindex*this->runSize + indexes[minindex]/MEM_FRAME_SIZE + 1 >= inputFile.totalPages){
				frames[minindex][active[minindex]] = memory.getEmptyFrame();
			}
			else if (indexes[minindex]/MEM_FRAME_SIZE + 1 >= this->runSize)
				frames[minindex][active[minindex]] = memory.getEmptyFrame();
			else{		
				frames[minindex][active[minindex]] = memory.loadPage(inputFile,leftStart + minindex*this->runSize + indexes[minindex]/MEM_FRAME_SIZE+1);
			}
			active[minindex] = (active[minindex]+1)%2;
		}
		indexes[B/2-1]++;
		if (indexes[B/2-1] == MEM_FRAME_SIZE){
			memory.writeFrame(tempFile, frames[B/2-1][active[B/2-1]], currPage);
			currPage++;
			memory.freeFrame(frames[B/2-1][active[B/2-1]]);
			frames[B/2-1][active[B/2-1]] = memory.getEmptyFrame();
			active[B/2-1] = (active[B/2-1]+1)%2;
			indexes[B/2-1] = 0;			
		}	
	}
	if (indexes[B/2-1]){
		memory.writeFrame(tempFile, frames[B/2-1][active[B/2-1]], currPage);
	}
	inputFile.DiskFileCopy(tempFile, leftStart, leftStart + finalRunSize - 1);
	for(int i = 0; i < frames.size(); i++){
			memory.freeFrame(frames[i][0]);
			memory.freeFrame(frames[i][1]);
		
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
	cout << "Pass1 done" << endl;
	inputFile.writeDiskFile();
	int leftStart;
	for(this->runSize = B; this->runSize < inputFile.totalPages; this->runSize *= (B/2-1)){
		// cout << "runSize: " << this->runSize << endl;
		for(leftStart = 0; leftStart < inputFile.totalPages; leftStart += (B/2-1)*this->runSize){
			this->Bmerge(inputFile, memory, leftStart,B);
		}
		totalPass++;
	}
	// cout << runSize << endl;
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
