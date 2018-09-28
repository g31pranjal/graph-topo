#include "adjList.h"
#include "csr.h"
#include <iostream>
#include <array>
#include <string.h>
#include <chrono>

using namespace std;


int main() {

	csr ob;
	
	int t;
	cin >> t;
	int a = t;

	auto tb = chrono::steady_clock::now();
	for(int i=1;i<=t;i++) {
		int s, d;
		cin >> s >> d;
		ob.insert(s, d);
		if(i%500 == 0) {
			auto ta = chrono::steady_clock::now();
			auto diff = ta - tb;
			tb = ta;
			// cout << i << ", " << t <<  "\n";
			cout << chrono::duration <double, milli> (diff).count()*1000 << "\n";
		
		}
	}

}
