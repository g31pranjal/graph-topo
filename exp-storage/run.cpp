#include "adjpma.h"
#include "adjList.h"
#include "csr.h"
#include <iostream>
#include <array>
#include <string.h>
#include <chrono>
#include <vector>

using namespace std;

int main() {


	
	int startNode, endNode, edges;
	cin >> startNode >> endNode >> edges;
	
	// adjList ob(endNode);
	csr ob1;
	adjList ob2;
	adjPma ob3;

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


		ob1.insert(s, d);
		ob2.insert(s, d);
		ob3.insert(s, d);
		
		if(i%500 == 0) {
			// auto ta = chrono::steady_clock::now();
			// auto diff = ta - tb;
			// tb = ta;
			vector< double > a;

			a.clear();

			for(int hop=6; hop <= 7; hop ++) {
				for(int run = 0; run<5; run++) {
					auto t1 = chrono::steady_clock::now();
					ob1.khops(hop, 100, (run+1)*2 + hop );
					auto t2 = chrono::steady_clock::now();
					auto diff = t2 - t1;
					a.push_back( chrono::duration <double, milli> (diff).count() ) ;
				}				
			}

			for(int hop=6; hop <= 7; hop ++) {
				for(int run = 0; run<5; run++) {
					auto t1 = chrono::steady_clock::now();
					ob2.khops(hop, 100, (run+1)*2 + hop );
					auto t2 = chrono::steady_clock::now();
					auto diff = t2 - t1;
					a.push_back( chrono::duration <double, milli> (diff).count() ) ;	
				}				
			}

			for(int hop=6; hop <= 7; hop ++) {
				for(int run = 0; run<5; run++) {
					auto t1 = chrono::steady_clock::now();
					ob3.khops(hop, 100, (run+1)*2 + hop );
					auto t2 = chrono::steady_clock::now();
					auto diff = t2 - t1;
					a.push_back( chrono::duration <double, milli> (diff).count() ) ;
				}				
			}


			for(int s=0;s<a.size();s++)
				cout << a[s] << ", ";
			cout << "\n";
							
		}

		// if(i == 3000) {
		// 	printf("3k insertions done.\n");
		// 	break;
		// }
	}
	// ob.print();
	// printf("starting khops.\n");

}

