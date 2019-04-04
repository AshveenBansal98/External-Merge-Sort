#include"MainMemory.h"

//loads nth page of disk file f to an empty frame if available
int MainMemory :: loadPage(DiskFile &f, int n){

	if(n >= f.totalPages)
		return -1; // invalid page

	for(int i = 0; i < this->totalFrames; i++){
		if(!this->valid[i]){
			this->data[i].fillFrame(f.data[n].arr);
			this->valid[i] = true;
			return i;
		}
	}
	return -1; //no empty frame is available
}


//returns an empty frame if available
int MainMemory :: getEmptyFrame(){

	vector<int> v(MEM_FRAME_SIZE, INT_MAX);

	for(int i = 0; i < this->totalFrames; i++){
		if(!this->valid[i]){
			this->data[i].fillFrame(v);
			this->valid[i] = true;
			return i;
		}
	}
	return -1; //no empty frame is available

}

//returns number of valid entries in frame f
int MainMemory :: getValidEntries(int f){
	if(f >= this->totalFrames || !this->valid[f])
		return -1;
	return this->data[f].validEntries;
}


// returns value stored at location i in frame f
int MainMemory :: getVal(int f, int i){
	if(!this->valid[f] || i >= MEM_FRAME_SIZE)
		return -1;
	return this->data[f].arr[i];
}

//assigns value val to ith location of frame f
void MainMemory :: setVal(int f, int i, int val){
	if(!this->valid[f] || i >= MEM_FRAME_SIZE){
		cout << "accessing invalid address" << endl;
		exit(1);
	}

	this->data[f].arr[i] = val;
	if(this->data[f].validEntries == i)
		this->data[f].validEntries = i+1;
}

void MainMemory :: invalidate(int f, int i){
	if (i >= data[f].validEntries || i > MEM_FRAME_SIZE)
		return;
	data[f].arr[i] = INT_MAX;
	swap(data[f].arr[i], data[f].arr[data[f].validEntries - 1]);
	data[f].validEntries--;
	
}

//write Frame f to file at page p
void MainMemory :: writeFrame(DiskFile &inputFile, int f, int p){
	inputFile.data[p].validEntries = getValidEntries(f);

	for(int i = 0; i <	MEM_FRAME_SIZE; i++){
		inputFile.data[p].arr[i] = this->data[f].arr[i];
	}	
}

//clears frame f
void MainMemory :: freeFrame(int f){
	if(f < totalFrames)
		this->valid[f] = false;
}	

