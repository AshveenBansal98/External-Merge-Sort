#include <bits/stdc++.h>
using namespace std;

int main (){
	int prv;
	cin >> prv;
	for(int i = 1; i < 10000; i++){
		int next;
		cin >> next;
		if (next < prv)
			cout << "ERROR" << endl;
		prv = next;
	}
	

}
