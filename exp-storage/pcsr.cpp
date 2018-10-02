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
		newNodeList[i] = nodeList[i];
		newRefList[i] = refList[i];
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

	printf("insert called... l : %d, nNodes : %d\n", l, nNodes);

	if(l != nNodes && nodeList[l] == src) {
		edgeList->insert(refList[l], dest);
	}
	else {

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

		if(nNodes - 1 == 0) {
			// case of no elements
			printf("case : no elements in nodelist\n");
			edgeList->firstInsert(0, 1, dest, insertAt);
		}
		else if(l == nNodes - 1) {
			// case of storing at the end 
			printf("case : inserting element at end of nodelist\n");
			edgeList->firstInsert(-1, 2, dest, insertAt);
		}
		else {
			// storing in the middle.
			printf("3\n");
			edgeList->firstInsert(refList[insertAt], 3, dest, insertAt);
		}

		
		

		// if(l == nNodes)
		// 	l = nNodes - 1;
		
		// refList[insertAt] = 0;
		// nNodes++;

		// edgeList.firstInsert()

		// int insertAt = l;

		// adjNodePma* n = new adjNodePma();
		// n->val = src;
		// n->edgeList = new pma(deflength, 0.9);
		// n->edgeList->insert(dest);


		
	}
}

void pcsr::print() {

	printf("nodes : %d\n", nNodes);

	for(int i=0;i<nNodes;i++) {
		// int start = refList[i];
		// int end;
		// if(i==nNodes-1)
		// 	end = nEdges;
		// else
		// 	end = refList[i+1];

		cout<<nodeList[i]<<" , "<<refList[i]<<"\n";
		this->edgeList->print();
		// for(int j=start;j<end;j++)
		// 	cout<<edgeList[j]<<" ";
		// cout<<"\n"; 
	}
	cout<<"---\n";
}

