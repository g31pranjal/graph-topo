#include "graphConnectivityStructure.h"
#include "adjList.h"
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <deque>
#include <tuple>
#include <ctime>
#include <chrono>

using namespace std;


adjNode::adjNode() {
	this->val = -1;
	this->edgeList = NULL;
	this->elsize = 0;
} 


void adjList::expandNodeList() {			
	int newSize = (int)this->lNodeList*this->multiplier;
	adjNode** newNodeList = (adjNode**)malloc(newSize*sizeof(adjNode*));
	for(int i=0;i<this->nNodes;i++) 
		newNodeList[i] = this->nodeList[i];
	free(this->nodeList);
	this->nodeList = newNodeList;
	this->lNodeList = newSize;
	return;
}


void adjList::expandEdgeList(adjNode* node) {
	int oldSize = node->elsize;
	int newSize = int(oldSize*this->multiplier);
	int* el = (int *)malloc(newSize*sizeof(int));
	memset(el, 0, newSize*sizeof(int));
	for(int i=0;i<oldSize;i++)
		el[i] = node->edgeList[i];
	free(node->edgeList);
	node->edgeList = el;
	node->elsize = newSize;
	return;
}


adjList::adjList() {
	this->deflength = 2;
	this->multiplier = 1.5;
	this->nodeList = (adjNode**)malloc(deflength*sizeof(adjNode*));
	this->nNodes = 0;
	this->lNodeList = deflength;
}


adjList::adjList(int maxNodes) {
	this->deflength = 2;
	this->multiplier = 1.5;
	this->nodeList = (adjNode**)malloc((maxNodes+1)*sizeof(adjNode*));
	memset(nodeList, 0, (maxNodes+1)*sizeof(adjNode*));
	this->nNodes = 0;
	this->lNodeList = maxNodes+1;
}


adjNode* adjList::createAdjNode(int src) {
	adjNode* n = new adjNode();
	n->val = src;
	int* el = (int *)malloc(this->deflength*sizeof(int));
	memset(el, 0, this->deflength*sizeof(int));
	n->edgeList = el;
	n->elsize = this->deflength;
	return n;
}


void adjList::insert(int src, int dest) {

	int l = 0;
	int r = this->nNodes;
	int m;
	while(l != r) {
		m = l + (r - l)/2;
		if(nodeList[m]->val < src)
			l = m+1;
		else 
			r = m;
	}


	if(l != this->nNodes && this->nodeList[l]->val == src) {

		int* el = this->nodeList[l]->edgeList;
		int eSize = this->nodeList[l]->elsize;
		if(el[eSize-1] != 0) {
			expandEdgeList(this->nodeList[l]);
			el = this->nodeList[l]->edgeList;
			eSize = this->nodeList[l]->elsize;
		}

		int smallId = -1;
		for(int i=0;i<eSize;i++) {
			if(el[i] == 0)
				break;
			if(el[i] < dest)
				smallId = i;
		}

		int insertAt = smallId+1;

		int popped = dest;
		int i = 0;
		for(i=insertAt;el[i] != 0;i++) {
			int tmp = el[i];
			el[i] = popped;
			popped = tmp;
		}
		el[i] = popped;
	}

	else {

		if(this->nNodes+1 > lNodeList) 
			this->expandNodeList();

		int insertAt = l;

		for(int i = this->nNodes-1;i >= insertAt;i--) {
			this->nodeList[i+1] = this->nodeList[i];
		}
		this->nodeList[insertAt] = this->createAdjNode(src);
		this->nodeList[insertAt]->edgeList[0] = dest;
		this->nNodes++;
	}
	return;
}


void adjList::insertInFixedNodelist(int src, int dest) {

	if(this->nodeList[src] == NULL) {
		this->nodeList[src] = this->createAdjNode(src);		
		this->nodeList[src]->edgeList[0] = dest;
		this->nNodes++;
	}
	else {
		int* el = nodeList[src]->edgeList;
		int eSize = nodeList[src]->elsize;
		if(el[eSize-1] != 0) {
			expandEdgeList(nodeList[src]);
			el = nodeList[src]->edgeList;
			eSize = nodeList[src]->elsize;
		}

		int smallId = -1;
		for(int i=0;i<eSize;i++) {
			if(el[i] == 0)
				break;
			if(el[i] < dest)
				smallId = i;
		}

		int insertAt = smallId+1;

		int popped = dest;
		int i = 0;
		for(i=insertAt;el[i] != 0;i++) {
			int tmp = el[i];
			el[i] = popped;
			popped = tmp;
		}
		el[i] = popped;
	}
}


void adjList::khops(int k, int iter, int seed) {

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
				int *el = this->nodeList[l]->edgeList;
				int elSize = this->nodeList[l]->elsize;
				// printf("elSize : %d\n", elSize);
				// auto t1 = chrono::steady_clock::now();
				for(int i=0; i < elSize && el[i] != 0; i++) {
					// retrv.push_back(el[i]);
					// printf("connected to : %d\n", el[i]);
					if(noe < k) {
						if( opr.size() < 4000) {
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
				}
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



void adjList::print() {
	cout<<lNodeList<<"\n";
	for(int i=0; i < lNodeList; i++) {
		if(nodeList[i] != NULL && nodeList[i]->val != -1) {
			cout<<nodeList[i]->val<<" -> ";
			for(int j=0;j<nodeList[i]->elsize;j++) {
				cout<<nodeList[i]->edgeList[j]<<" ";
			}
			cout<<"\n";
		}
	}
	cout<<"---\n";
}
