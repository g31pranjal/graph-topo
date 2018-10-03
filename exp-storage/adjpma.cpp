#include "graphConnectivityStructure.h"
#include "adjpma.h"
#include "pma.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>

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