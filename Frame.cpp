#include "Frame.h"

//fills frame with data from vector v
void Frame :: fillFrame(vector<int> &v){
	int i = 0;
	bool flag = false;
	while(i < v.size() && i < MEM_FRAME_SIZE){
		this->arr[i] = v[i];
		if(v[i] == INT_MAX){
			flag = true;
			this->validEntries = i;
			i++;
			break;
		}
		i++;
	}
	while(i < MEM_FRAME_SIZE){
		this->arr[i] = INT_MAX;
		i++;
	}
	if(!flag)
		this->validEntries = MEM_FRAME_SIZE;
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