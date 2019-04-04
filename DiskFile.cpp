#include "DiskFile.h"

//creates DiskFile by reading input file through cin
void DiskFile :: readDiskFile(){
	
	int i = 0;
	this->size  = 0;
	this->totalPages = 0; 
	Page t;
	vector<int> temp(DISK_PAGE_SIZE,INT_MAX);
	bool flag = true;
	int x;
	while(cin >> x){
		flag = false;
		temp[i++] = x;
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