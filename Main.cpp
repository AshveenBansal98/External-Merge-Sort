#include "common.h"
#include "MainMemory.h"
#include "DiskFile.h"
#include "ExtMergeSort.h"
// #include "MainMemory.cpp"
// #include "DiskFile.cpp"
// #include "ExtMergeSort.cpp"

int MEM_FRAME_SIZE;
int DISK_PAGE_SIZE;

using namespace std;

int main()
{
	cout << "ENTER PAGE/FRAME SIZE" << endl;
	cin >> MEM_FRAME_SIZE ;
	DISK_PAGE_SIZE = MEM_FRAME_SIZE;
	cout  << "ENTER PARAMETER B, MUST BE EVEN" << endl;
	int b;
	cin >> b;
	cout << "ENTER MEMORY SIZE" << endl;
	int mem_size;
	cin >> mem_size;
	cout << "ENTER NUMBERS" << endl;
	
	MainMemory mm(mem_size);
	DiskFile f;
	f.readDiskFile();
	cout << "File is stored as" << endl;
	f.writeDiskFile();

	ExtMergeSort e;
	
	//call 2 way externalmerge sort
	e.BWaySort(f,mm, b);

	//output file by using cout
	f.writeDiskFile(); 

}
