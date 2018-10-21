#include "adjpma.h"
#include "adjList.h"
#include "csr.h"
#include "pcsr.h"
#include <iostream>
#include <array>
#include <string.h>
#include <chrono>
#include <vector>

using namespace std;

int main() {

	printf("main\n");

	// adjList ob;
	pcsr ob;
	
	int startNode, endNode, edges;
	cin >> startNode >> endNode >> edges;
	
	// int a = edges;

	// printf("inserting 3,4\n");
	// ob.insert(3,4);
	// ob.print();

	// ob.insert(3,4);
	// ob.print();
	// ob.insert(3,5);
	// ob.print();
	// ob.insert(1,8);
	// ob.print();
	// ob.insert(3,2);
	// ob.print();
	// ob.insert(7,11);
	// ob.print();
	// ob.insert(3,1);
	// ob.print();
	// ob.insert(5,1);
	// ob.print();
	// ob.insert(6,8);
	// ob.print();
	// ob.insert(6,1);
	// ob.print();


	auto tb = chrono::steady_clock::now();
	
	for(int i = 1; i <= edges; i++) {
		int s, d;
		cin >> s >> d;
		printf("inserting %d, %d\n", s, d);
		ob.insert(s, d);
		// ob.print();
		if(i%500 == 0) {
		}
	}
	auto ta = chrono::steady_clock::now();
	auto diff = ta - tb;
	// tb = ta;
	cout << chrono::duration <double, milli> (diff).count()*1000 << "\n";

}

