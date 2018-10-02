#include "adjpma.h"
#include "adjList.h"
#include "csr.h"
#include <iostream>
#include <array>
#include <string.h>
#include <chrono>

using namespace std;


int main() {


	// adjList ob;
	csr ob;
	
	int startNode, endNode, edges;
	cin >> startNode >> endNode >> edges;
	
	csr ob(endNode);

	// ob.insertInFixedNodelist(3, 4);
	// ob.insertInFixedNodelist(1, 5);
	// ob.insertInFixedNodelist(2, 8);
	// ob.insertInFixedNodelist(7, 2);
	// ob.insertInFixedNodelist(3, 7);
	// ob.print();
	// ob.insertInFixedNodelist(3, 4);
	// ob.insertInFixedNodelist(3, 9);
	// ob.insertInFixedNodelist(3, 1);
	// ob.print();

	
	int a = edges;

	// ob.insert(3,4);
	// ob.insert(3,8);
	// ob.insert(3,1);
	// ob.print();


	auto tb = chrono::steady_clock::now();
	
	for(int i = 1; i <= edges; i++) {
		int s, d;
		cin >> s >> d;

		ob.insertInFixedNodelist(s, d);
		// ob.insert(s, d);
		if(i%500 == 0) {

			auto ta = chrono::steady_clock::now();
			auto diff = ta - tb;
			tb = ta;
			cout << chrono::duration <double, milli> (diff).count()*1000 << "\n";
		}
	}

}
