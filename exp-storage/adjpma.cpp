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

	int smallId = -1;
	int foundAt = -1;
	for(int i=0;i<nNodes;i++) {
		if(nodeList[i]->val < src) 
			smallId = i;
		if(nodeList[i]->val == src) { 
			foundAt = i;
			break;
		}
	}
	if(foundAt == -1) {

		adjNodePma* n = new adjNodePma();
		n->val = src;
		n->edgeList = new pma(deflength, 0.7);
		n->edgeList->insert(dest);

		if(nNodes+1 > lNodeList) 
			expandNodeList();

		int insertAt = smallId+1;

		for(int i = nNodes-1;i >= insertAt;i--) {
			nodeList[i+1] = nodeList[i];
		}
		nodeList[insertAt] = n;
		nNodes++;
	}

	else {

		pma* el = nodeList[foundAt]->edgeList;
		el->insert(dest);

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
