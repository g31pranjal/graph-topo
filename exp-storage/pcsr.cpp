#include "pcsr.h"
#include "pma_for_csr.h"
#include <iostream>
#include <string.h>

using namespace std;


void pcsr::expandNodeList() {
	int oldSize = this->lNodeList;
	int newSize = oldSize*multiplier;
	int *newNodeList = (int *)malloc(newSize*sizeof(int));
	memset(newNodeList, 0, newSize*sizeof(int));
	int *newRefList = (int *)malloc(newSize*sizeof(int));
	memset(newRefList, 0, newSize*sizeof(int));
	for(int i = 0; i < this->nNodes; i++) {
		newNodeList[i] = this->nodeList[i];
		newRefList[i] = this->refList[i];
	}
	free(this->nodeList);
	free(this->refList);
	this->nodeList = newNodeList;
	this->refList = newRefList;
	this->lNodeList = newSize;
}


pcsr::pcsr() {
	this->deflength = 2;
	this->multiplier = 1.5;
	this->lNodeList = 0;
	this->nNodes = 0;

	this->nodeList = (int *)malloc(deflength*sizeof(int));
	memset(nodeList, 0, deflength*sizeof(int));
	this->refList = (int *)malloc(deflength*sizeof(int));
	memset(refList, 0, deflength*sizeof(int));
	this->lNodeList = deflength;
	this->edgeList = new pma_for_csr(&refList, 2, 0.9);
}


void pcsr::insert(int src, int dest) {

	printf("pcsr::insert with %d, %d\n", src, dest);
	
	int l = 0;
	int r = nNodes;
	int m;
	while(l != r) {
		m = l + (r - l)/2;
		if(nodeList[m] < src)
			l = m+1;
		else 
			r = m;
	}

	if(l != nNodes && nodeList[l] == src) {
		// this is the general case of insertion
		edgeList->insert(refList[l], dest);
	}
	else {

		// something seems fishy, but lets leave this 
		if(nNodes > lNodeList) { 
			expandNodeList();
		}

		// shift elements of the nodelist to make roam for new element.
		int insertAt = l;
		for(int i = this->nNodes-1;i >= insertAt;i--) {
			nodeList[i+1] = nodeList[i];
		}
		nodeList[insertAt] = src;
		this->nNodes++;

		if(nNodes - 1 == l) {
			printf("case : insert in the end, also covers the case of first insertion\n");
			while(!edgeList->initInsert(1, 0, dest, insertAt) ) {
				printf(".\n");
				// try again after rebalancing is done.
			}
		}
		else {
			printf("case : store somewhere in the middle\n");
			while(!edgeList->initInsert(2, refList[insertAt], dest, insertAt)) {
				int l = 0;
				int r = nNodes;
				int m;
				while(l != r) {
					m = l + (r - l)/2;
					if(nodeList[m] < src)
						l = m+1;
					else 
						r = m;
				}
				insertAt = l;
			}
		}
		
	}
}


void pcsr::print() {
	printf("pcsr::print\n");
	printf("nodes : %d\n", nNodes);
	for(int i=0;i<nNodes;i++)
		cout<<nodeList[i]<<" , "<<refList[i]<<"\n";
	this->edgeList->print();
	cout<<"---\n";
}

