#include "Frame.h"

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