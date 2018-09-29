#include "pcsr.h"
#include <iostream>
#include <string.h>

using namespace std;

void csr::expandNodeList() {
	int oldSize = lNodeList;
	int newSize = oldSize*multiplier;
	int *newNodeList = (int *)malloc(newSize*sizeof(int));
	memset(newNodeList, 0, newSize*sizeof(int));
	int *newRefList = (int *)malloc(newSize*sizeof(int));
	memset(newRefList, 0, newSize*sizeof(int));
	for(int i=0;i<nNodes;i++) {
		newNodeList[i] = nodeList[i];
		newRefList[i] = refList[i];
	}
	free(nodeList);
	free(refList);
	nodeList = newNodeList;
	refList = newRefList;
	lNodeList = newSize;
}


csr::csr() {
	deflength = 2;
	multiplier = 1.5;
	lNodeList = 0;
	lEdgeList = 0;
	nNodes = 0;
	nEdges = 0;

	nodeList = (int *)malloc(deflength*sizeof(int));
	memset(nodeList, 0, deflength*sizeof(int));
	refList = (int *)malloc(deflength*sizeof(int));
	memset(refList, 0, deflength*sizeof(int));
	lNodeList = deflength;
}


void csr::insert(int src, int dest) {

	if(lEdgeList == nEdges) 
		expandEdgeList();
	
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

		int start = refList[l];
		int end;
		if(l == nNodes-1)
			end = nEdges;
		else
			end = refList[l+1];
		
		for(int i = l+1;i<nNodes;i++)
			refList[i]++;

		int smallId = start - 1;
		for(int i=start;i<end;i++) {
			if(edgeList[i] < dest) 
				smallId = i;
		}
		int insertAt = smallId + 1;

		for(int i=nEdges;i>insertAt;i--)
			edgeList[i] = edgeList[i-1];
		edgeList[insertAt] = dest;
		nEdges++;

	}
	else {
		
		if(lNodeList == nNodes) 
			expandNodeList();
		int insertAt = l;

		for(int i=nNodes;i>insertAt;i--) {
			nodeList[i] = nodeList[i-1];
			refList[i] = refList[i-1] + 1;
		}
		nodeList[insertAt] = src;
		if(insertAt==nNodes)
			refList[insertAt] = nEdges;
		nNodes++;

		int eInsertAt = refList[insertAt];
		for(int i=nEdges;i>eInsertAt;i--)
			edgeList[i] = edgeList[i-1];
		edgeList[eInsertAt] = dest;
		nEdges++;
		
	}
}

void csr::print() {
	for(int i=0;i<nNodes;i++) {
		int start = refList[i];
		int end;
		if(i==nNodes-1)
			end = nEdges;
		else
			end = refList[i+1];

		cout<<nodeList[i]<<","<<refList[i]<<" -> ";
		for(int j=start;j<end;j++)
			cout<<edgeList[j]<<" ";
		cout<<"\n"; 
	}
	cout<<"---\n";
}