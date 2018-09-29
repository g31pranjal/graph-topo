#include "graphConnectivityStructure.h"
#include "adjpma.h"
#include "pma.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>

using namespace std;

adjNodePma::adjNodePma() {
	val = -1;
	edgeList = NULL;
} 

void adjPma::expandNodeList() {
	int newSize = (int)lNodeList*multiplier;
	adjNodePma** newNodeList = (adjNodePma**)malloc(newSize*sizeof(adjNodePma*));
	for(int i=0;i<nNodes;i++) 
		newNodeList[i] = nodeList[i];
	free(nodeList);
	nodeList = newNodeList;
	lNodeList = newSize;
	return;
}


adjPma::adjPma() {
	deflength = 2;
	multiplier = 1.5;
	nodeList = NULL;
	lNodeList = 0;
	nNodes = -1;
	nodeList = (adjNodePma**)malloc(deflength*sizeof(adjNodePma*));
	nNodes = 0;
	lNodeList = deflength;

}


void adjPma::insert(int src, int dest) {

	int l = 0;
	int r = nNodes;
	int m;
	while(l != r) {
		m = l + (r - l)/2;
		if(nodeList[m]->val < src)
			l = m+1;
		else 
			r = m;
	}

	if(l != nNodes && nodeList[l]->val == src) {
		
		pma* el = nodeList[l]->edgeList;
		el->insert(dest);

	}

	else {
		int insertAt = l;

		adjNodePma* n = new adjNodePma();
		n->val = src;
		n->edgeList = new pma(deflength, 0.9);
		n->edgeList->insert(dest);

		if(nNodes+1 > lNodeList) 
			expandNodeList();

		for(int i = nNodes-1;i >= insertAt;i--) {
			nodeList[i+1] = nodeList[i];
		}
		nodeList[insertAt] = n;
		nNodes++;
		
	}
	return;
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