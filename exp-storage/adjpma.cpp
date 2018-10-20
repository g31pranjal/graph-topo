#include "graphConnectivityStructure.h"
#include "adjpma.h"
#include "pma.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <deque>

using namespace std;

adjNodePma::adjNodePma() {
	this->val = -1;
	this->edgeList = NULL;
} 

void adjPma::expandNodeList() {
	int newSize = (int)this->lNodeList*multiplier;
	adjNodePma** newNodeList = (adjNodePma**)malloc(newSize*sizeof(adjNodePma*));
	for(int i=0;i<nNodes;i++) 
		newNodeList[i] = this->nodeList[i];
	free(this->nodeList);
	this->nodeList = newNodeList;
	this->lNodeList = newSize;
	return;
}


adjPma::adjPma() {
	this->deflength = 2;
	this->multiplier = 1.5;
	this->nodeList = NULL;
	this->lNodeList = 0;
	this->nNodes = -1;
	this->nodeList = (adjNodePma**)malloc(this->deflength*sizeof(adjNodePma*));
	this->nNodes = 0;
	this->lNodeList = deflength;
}

adjPma::adjPma(int maxNodes) {
	this->deflength = 2;
	this->multiplier = 1.5;
	this->nodeList = (adjNodePma**)malloc((maxNodes+1)*sizeof(adjNodePma*));
	this->lNodeList = maxNodes;
	this->nNodes = -1;
	this->nNodes = 0;
}


adjNodePma* adjPma::createAdjNodePma(int src) {
	adjNodePma* n = new adjNodePma();
	n->val = src;
	n->edgeList = new pma(deflength, 1);
	return n;
}


void adjPma::insert(int src, int dest) {

	int l = 0;
	int r = this->nNodes;
	int m;
	while(l != r) {
		m = l + (r - l)/2;
		if(this->nodeList[m]->val < src)
			l = m+1;
		else 
			r = m;
	}

	if(l != this->nNodes && this->nodeList[l]->val == src) {
		pma* el = this->nodeList[l]->edgeList;
		el->insert(dest);
	}
	else {
		int insertAt = l;

		if(nNodes+1 > lNodeList) 
			expandNodeList();

		for(int i = nNodes-1;i >= insertAt;i--) {
			nodeList[i+1] = nodeList[i];
		}
		nodeList[insertAt] = this->createAdjNodePma(src);
		nodeList[insertAt]->edgeList->insert(dest);
		nNodes++;
		
	}
	return;
}

void adjPma::insertInFixedNodelist(int src, int dest) {

	if(this->nodeList[src] != NULL) {
		pma* el = this->nodeList[src]->edgeList;
		el->insert(dest);
	}
	else {
		nodeList[src] = this->createAdjNodePma(src);
		nodeList[src]->edgeList->insert(dest);
		nNodes++;
	}
	return;
}

void adjPma::khops(int k, int iter, int seed) {

	// printf("in khops, nNodes : %d\n", this->nNodes);

	if(seed == -1 ) 
		srand(time(NULL));
	else 
		srand(seed);
	
	deque< vector <int> > opr;
	vector< vector <int> > fnl;
	vector <int> retrv, tmp;
	int broke = 0;
	// chrono::steady_clock::duration t;
	// t = t-t;
	// int rndc = 0;
	
	while(fnl.size() < iter) {
		// printf("fnl size %d \n", fnl.size());	
		
		int r = rand() % this->lNodeList;
		while(this->nodeList[r] == NULL) {
			r = rand() % this->lNodeList;
			// printf("randing\n");
		}

		// rndc++;

		
		// printf("reaching.\n");
		int val = this->nodeList[r]->val;
		tmp.push_back(val);
		opr.push_back(tmp);
		tmp.clear();

		// printf("opr size : %d\n", opr.size());

		while(opr.size() != 0) {
			retrv = opr.front();
			opr.pop_front();
			
			// printf("retrv ");
			// for(int k=0;k<retrv.size();k++)
			// 	printf("%d ", retrv[k]);
			// printf("\n");

			int noe = retrv.size();
			int head = retrv[retrv.size() - 1];

			// printf("noe %d\n", noe);

			int l = 0;
			int r = this->nNodes;
			int m;
			
			while(l != r) {
				m = l + (r - l)/2;
				if(nodeList[m]->val < head)
					l = m+1;
				else 
					r = m;
			}

			// printf("found node at %d\n", l);

			if(l != this->nNodes && this->nodeList[l]->val == head) {
				vector<int> el = this->nodeList[l]->edgeList->linearTraverse();
				// printf("%d\n", el.size() );
				// int elSize = this->nodeList[l]->elsize;
				// printf("elSize : %d\n", elSize);
				// auto t1 = chrono::steady_clock::now();
				for(int i=0; i < el.size(); i++) {
					// retrv.push_back(el[i]);
					// printf("connected to : %d\n", el[i]);
					if(noe < k ) {
						// printf("noe %d k %d\n", noe, k);
						if(opr.size() < 4000) {
							opr.push_back(retrv);
							opr[opr.size()-1].push_back(el[i]);
						}
					}
					else {
						fnl.push_back(retrv);
						fnl[fnl.size()-1].push_back(el[i]);
						// printf("fnl ");
						// for(int k=0;k<fnl[fnl.size()-1].size();k++)
						// 	printf("%d ", fnl[fnl.size()-1][k]);
						// printf("\n");
						// printf("fnl size : %d\n", fnl.size());
						if(fnl.size() > iter)
							break;
					}
					// printf("%d ", el[i]);
				}
				// printf("\n");
				// printf("... opr : %d\n", opr.size());
				// printf("# : %d\n", elSize);
				// rndc++;
				// auto t2 = chrono::steady_clock::now();
				// t = t + t2 - t1;
				// printf("..\n");
				retrv.clear();
				if(fnl.size() > iter)
					break;
			}
			else {
				// fnl.push_back(retrv);
				// printf("broke.\n");
				broke++;
			}
		}
		if(broke > this->nNodes){
			// printf("overflow @ nodes %d.\n", nNodes);
			break;
		}
		// cout << fnl.size() << "\n";
	}

	// cout << ".. rounding " << chrono::duration <double, milli> (t).count() << "\n";
	// printf("rnd cnt %d\n", rndc);
}


void adjPma::print() {
	cout<<lNodeList<<"\n";
	for(int i=0;i<lNodeList && nodeList[i] != NULL && nodeList[i]->val != -1;i++) {
		cout<<nodeList[i]->val<<" -> ";
		nodeList[i]->edgeList->print();
		cout<<"\n";
	}
	cout<<"---\n";
}


// int main() {

// 	adjPma a;

// 	a.insert(1,2);
// 	a.insert(5,2);
// 	a.insert(3,6);
// 	a.print();

// }