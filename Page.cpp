#include "Page.h"
#include "common.h"


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
