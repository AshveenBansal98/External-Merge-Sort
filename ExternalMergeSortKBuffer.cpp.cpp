#include <bits/stdc++.h>
using namespace std;
int DISK_PAGE_SIZE;
int MEM_FRAME_SIZE;

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


	void readDiskFile(int x);
	void writeDiskFile();
	void DiskFileCopy( DiskFile &f, int startPage, int endPage);
};


class MainMemory{

public:
	vector<Frame> data;
	int totalFrames;
	vector<bool> valid;

	MainMemory(int s){
		totalFrames = s;
		data.resize(s);
		valid.resize(s, false);
		//cout << "Mem created" << endl;
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
	void Bmerge(DiskFile &inputFile, MainMemory &memory, int leftStart, int B, int k);
	void BWaySort(DiskFile &inputFile, MainMemory &memory, int B, int k);
	void firstPass(DiskFile &inputFile, MainMemory &memory, int B);

};


//writes page to cout
void Page :: writePage(){

	for(int i = 0; i < DISK_PAGE_SIZE; i++){
		if(this->arr[i] == INT_MAX)
			break;
		cout << this->arr[i] << " ";
	}
	cout << endl;
}

//fills page with contents from vector v
void Page :: fillPage(vector<int> &v){
	
	if(v.size() < DISK_PAGE_SIZE){
		cout << "vector should contain atleast as many entries as a page" << endl;
		exit(1);
	}
	int i = 0;
	this->validEntries = 0;
	for(i; i < v.size() && i < DISK_PAGE_SIZE; i++){
		if(v[i] == INT_MAX){
			this->validEntries = i;
		}
		this->arr[i] = v[i];
	}
}



//fills frame with data from vector v
void Frame :: fillFrame(vector<int> &v){
	int i = 0;
	this->validEntries = 0;
	while(i < v.size() && i < MEM_FRAME_SIZE){
		this->arr[i] = v[i];
		if(v[i] != INT_MAX){
			this->validEntries++;
		}
		i++;
	}
	while(i < MEM_FRAME_SIZE){
		this->arr[i] = INT_MAX;
		i++;
	}
	
}

//void Frame :: recalculatevalid(){
//	this->validEntries = 0;
//	for(int i = 0; i < this->arr.size(); i++)
//		if (arr[i] != INT_MAX)
//			this->validEntries++;
//}

//Prints all valid entries of a frame
void Frame :: printFrame()
{
	for(int i = 0; i < this->validEntries; i++)
	{
		cout << this->arr[i] << endl;
	}
}




//creates DiskFile by reading input file through cin
void DiskFile :: readDiskFile(int x){
	
	int i = 0;
	this->size  = 0;
	this->totalPages = 0; 
	Page t;
	vector<int> temp(DISK_PAGE_SIZE,INT_MAX);
	bool flag = true;
	int y;
	for(int j = 0; j < x; j++){
		cin >> y;
		flag = false;
		temp[i++] = y;
		this->size++;
		
		if(i == DISK_PAGE_SIZE){
			t.fillPage(temp);
			this->data.push_back(t);
			flag = true;
			this->totalPages++;
			i = 0;
		}
	}

	if(!flag){
		
		while(i != DISK_PAGE_SIZE){
			temp[i++] = INT_MAX;	
		}
		t.fillPage(temp);
		this->data.push_back(t);
		this->totalPages++;
	}
}

//writes disk file to console
void DiskFile :: writeDiskFile(){
	cout << "Contents of Disk File: " <<endl;
	for(int  i = 0; i < totalPages; i++){
		cout << "Page: " << i << ": ";
		this->data[i].writePage();
	}
}

//copies contents of disk file f into destination disk file from startPage to endPage
void DiskFile :: DiskFileCopy(DiskFile &f, int startPage, int endPage){
	int j = 0; //start of source file
	int i = startPage; //start of destination file

	while(i <= endPage && j < f.totalPages && i < this->totalPages){
		this->data[i].validEntries = f.data[j].validEntries;
		for(int k = 0; k < DISK_PAGE_SIZE; k++){
			this->data[i].arr[k] = f.data[j].arr[k];
		}
		i++;
		j++;
	}
}




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


void ExtMergeSort :: Bmerge(DiskFile &inputFile, MainMemory &memory, int leftStart, int B, int k){
	int finalRunSize = (((B-k)/k)*this->runSize);
	DiskFile tempFile(finalRunSize);
	int currPage = 0;
	// int resFrame = memory.getEmptyFrame();
	vector < vector <int> > frames((B-k)/k, vector <int> (k));
	for(int i = 0; i < (B - k)/k; i++){
		for(int j = 0; j < k; j++){ 
			if (leftStart + i*this->runSize + j < inputFile.totalPages && j < this->runSize)
				frames[i][j] = memory.loadPage(inputFile, leftStart + i*this->runSize + j);		
			else
				frames[i][j] = memory.getEmptyFrame();
		}
		
	}
	//cout << "HELLO" << endl;
	vector <int> res;
	for(int i = ((B-k)/k)*k; i < B; i++)
		res.push_back(memory.getEmptyFrame());
	//frames[B/2 - 1][0] = memory.getEmptyFrame();
	//frames[B/2 - 1][1] = memory.getEmptyFrame();
	vector <int> active(B/2, 0);
	vector <int> indexes(B/2, 0);
	int minm = INT_MAX;
	int minindex = 0;
	// cout << "frame  " <<  frames.size()<< endl;
	for(;;){
		//cout << "X";
		minm = INT_MAX;
		for(int i = 0; i < frames.size(); i++){
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
		memory.setVal(res[active[B/2-1]], indexes[B/2-1], minm);
		indexes[minindex]++;
		if (indexes[minindex]%MEM_FRAME_SIZE == 0){
			memory.freeFrame(frames[minindex][active[minindex]]);	
			if (leftStart + minindex*this->runSize + indexes[minindex]/MEM_FRAME_SIZE + k - 1 >= inputFile.totalPages){
				frames[minindex][active[minindex]] = memory.getEmptyFrame();
			}
			else if (indexes[minindex]/MEM_FRAME_SIZE + k -1 >= this->runSize)
				frames[minindex][active[minindex]] = memory.getEmptyFrame();
			else{		
				frames[minindex][active[minindex]] = memory.loadPage(inputFile, leftStart + minindex*this->runSize + indexes[minindex]/MEM_FRAME_SIZE + k-1);
			}
			active[minindex] = (active[minindex]+1)%k;
		}
		indexes[B/2-1]++;
		if (indexes[B/2-1] == MEM_FRAME_SIZE){
			memory.writeFrame(tempFile, res[active[B/2-1]], currPage);
			currPage++;
			memory.freeFrame(res[active[B/2-1]]);
			res[active[B/2-1]] = memory.getEmptyFrame();
			active[B/2-1] = (active[B/2-1]+1)%(res.size());
			indexes[B/2-1] = 0;			
		}	
	}
	if (indexes[B/2-1]){
		memory.writeFrame(tempFile, res[active[B/2-1]], currPage);
	}
	inputFile.DiskFileCopy(tempFile, leftStart, leftStart + finalRunSize - 1);
	for(int i = 0; i < frames.size(); i++){
		for(int j = 0; j < k; j++)
			memory.freeFrame(frames[i][j]);
			//memory.freeFrame(frames[i][1]);
		
	}
	for(int i = 0; i <res.size(); i++)
		memory.freeFrame(res[i]);
	
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

void ExtMergeSort :: BWaySort(DiskFile &inputFile, MainMemory &memory, int B, int k){
	vector <int> arr1;
	vector <int> arr2;
	int x;
	cin >> x;
	while( x != -1){
		arr1.push_back(x);
		cin >> x;
		arr2.push_back(x-1);
		cin >> x;
	}
	vector < vector <int> > ans(arr1.size());
	
	this->firstPass(inputFile, memory, B);
	//cout << "Pass1 done" << endl;
	//inputFile.writeDiskFile();
	//return;
	int leftStart;
	totalPass = 1;
	for(int i = 0; i < arr1.size(); i++){
		if (arr1[i] == totalPass){
			if (arr2[i] < inputFile.totalPages){
				ans[i] = inputFile.data[arr2[i]].arr;
			}
		}
	}
	for(this->runSize = B; this->runSize < inputFile.totalPages; this->runSize *= (B-k)/k ){
		// cout << "runSize: " << this->runSize << endl;
		for(leftStart = 0; leftStart < inputFile.totalPages; leftStart += ((B-k)/k)*this->runSize){
			this->Bmerge(inputFile, memory, leftStart,B, k);
		}
		totalPass++;
		for(int i = 0; i < arr1.size(); i++){
			if (arr1[i] == totalPass){
				if (arr2[i] < inputFile.totalPages)
					ans[i] = inputFile.data[arr2[i]].arr;
			}
		}
		//cout << endl << endl;
		//inputFile.writeDiskFile();  
		//cout << endl << endl;
		//break;
	}
	for(int i = 0; i < arr1.size(); i++){
		cout << "Pass " << arr1[i] << ", Page " << arr2[i] + 1 << ": ";
		if (ans[i].size() == 0){
			if (arr1[i] > totalPass)
				cout << "Invalid pass number" << endl;
			else
				cout << "Invalid page number" << endl;
		}
		else{
			for(int j = 0; j < ans[i].size() && ans[i][j] != INT_MAX; j++)
				cout << ans[i][j] << " ";
			cout << endl;
		}
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

int main()
{
	//cout << "ENTER PAGE/FRAME SIZE" << endl;
	cin >> MEM_FRAME_SIZE ;
	DISK_PAGE_SIZE = MEM_FRAME_SIZE;
	
	//cout << "ENTER MEMORY SIZE" << endl;
	int mem_size;
	cin >> mem_size;
	//cout << "ENTER NUMBERS" << endl;
	
	//cout  << "ENTER PARAMETER k" << endl;
	int k;
	cin >> k;

	MainMemory mm(mem_size);
	DiskFile f;
	int x;
	cin >> x;
	f.readDiskFile(x);
	//cout << "File is stored as" << endl;
	//f.writeDiskFile();

	ExtMergeSort e;
	
	//call 2 way externalmerge sort
	e.BWaySort(f,mm,mem_size, k);

	//output file by using cout
	//f.writeDiskFile(); 

}


