#include "common.h"
#include "MainMemory.h"
#include "DiskFile.h"
#include "ExtMergeSort.h"
// #include "MainMemory.cpp"
// #include "DiskFile.cpp"
// #include "ExtMergeSort.cpp"

using namespace std;

int main()
{
	int x;
	//reads size of main memory in terms of number of frames available
	// cin >> x;

	// //create main memory of x frames
	// MainMemory mm(x);


	// //create a file by taking input from cin
	// DiskFile f;
	// f.readDiskFile();
	// f.writeDiskFile();

	// ExtMergeSort e;
	
	// //call 2 way externalmerge sort
	// e.twoWaySort(f,mm);

	// //output file by using cout
	// f.writeDiskFile(); 


	
	//reads size of main memory in terms of number of frames available
	cin >> x;

	//create main memory of x frames
	MainMemory mm1(x);


	//create a file by taking input from cin
	DiskFile f1;
	f1.readDiskFile();
	f1.writeDiskFile();

	ExtMergeSort e1;
	
	//call 2 way externalmerge sort
	e1.BWaySort(f1,mm1, 3);

	//output file by using cout
	f1.writeDiskFile(); 

}
