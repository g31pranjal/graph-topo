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

	auto tb = chrono::steady_clock::now();
	vector< double > a;
	double val;
	
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

			a.clear();

			for(int hop=2; hop <= 7; hop ++) {
				val = 0.0;
				for(int run = 0; run<5; run++) {
					auto t1 = chrono::steady_clock::now();
					ob1.khops(hop, 100, (run+1)*2 + hop );
					auto t2 = chrono::steady_clock::now();
					auto diff = t2 - t1;
					val += chrono::duration <double, milli> (diff).count() / 5 ;
				}
				a.push_back(val) ;	
			}

			for(int hop=2; hop <= 7; hop ++) {
				val = 0.0;
				for(int run = 0; run<5; run++) {
					auto t1 = chrono::steady_clock::now();
					ob2.khops(hop, 100, (run+1)*2 + hop );
					auto t2 = chrono::steady_clock::now();
					auto diff = t2 - t1;
					val += chrono::duration <double, milli> (diff).count() / 5 ;
				}				
				a.push_back(val) ;	
			}

			for(int hop=2; hop <= 7; hop ++) {
				val = 0.0;
				for(int run = 0; run<5; run++) {
					auto t1 = chrono::steady_clock::now();
					ob3.khops(hop, 100, (run+1)*2 + hop );
					auto t2 = chrono::steady_clock::now();
					auto diff = t2 - t1;
					val += chrono::duration <double, milli> (diff).count() / 5 ;
				}				
				a.push_back(val) ;	
			}


			for(int s=0;s<a.size();s++)
				cout << a[s] << ", ";
			cout << "\n";
							
		}
	}

}

