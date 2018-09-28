#include "adjList.h"
#include "csr.h"
#include <iostream>
#include <array>
#include <string.h>
#include <chrono>

using namespace std;


int main() {

	csr ob;
	
	int startNode, endNode, edges;
	cin >> startNode >> endNode >> edges;
	
	int a = edges;

	auto tb = chrono::steady_clock::now();
	
	for(int i = 1; i <= edges; i++) {
		int s, d;
		cin >> s >> d;
		ob.insert(s, d);
		if(i%100 == 0) {
			auto ta = chrono::steady_clock::now();
			auto diff = ta - tb;
			tb = ta;
			cout << chrono::duration <double, milli> (diff).count()*1000 << "\n";
		}
	}

}
