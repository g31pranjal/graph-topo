#include "adjpma.h"
#include "adjList.h"
#include "csr.h"
#include "pcsr.h"
#include <iostream>
#include <array>
#include <string.h>
#include <chrono>

using namespace std;


int main() {

	pcsr ob;
	// csr ob;
	
	// int startNode, endNode, edges;
	// cin >> startNode >> endNode >> edges;
	
	// int a = edges;

	ob.insert(3,4);
	ob.insert(3,5);
	ob.insert(3,8);
	ob.insert(3,2);
	ob.insert(3,11);
	ob.insert(3,1);
	ob.insert(5,1);

	// // ob.insert(3,8);
	// // ob.insert(3,1);
	ob.print();


	// auto tb = chrono::steady_clock::now();
	
	// for(int i = 1; i <= edges; i++) {
	// 	int s, d;
	// 	cin >> s >> d;
	// 	ob.insert(s, d);
	// 	if(i%500 == 0) {
	// 		auto ta = chrono::steady_clock::now();
	// 		auto diff = ta - tb;
	// 		tb = ta;
	// 		cout << chrono::duration <double, milli> (diff).count()*1000 << "\n";
	// 	}
	// }

}
