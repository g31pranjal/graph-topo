#include "csr.h"
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


void csr::expandEdgeList() {
	int oldSize = lEdgeList;
	int newSize = (int)oldSize*multiplier;
	if(newSize == 0)
		newSize = deflength;
	// cout<<"edge expanding to "<<oldSize*multiplier<<" from "<<oldSize<<"\n";
	int *newEdgeList = (int *)malloc(newSize*sizeof(int));
	memset(newEdgeList, 0, newSize*sizeof(int));
	for(int i=0;i<nEdges;i++) {
		newEdgeList[i] = edgeList[i];
	}
	if(lEdgeList != 0)
		free(edgeList);
	edgeList = newEdgeList;	
	lEdgeList = newSize;
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

csr::csr(int maxNodes) {
	deflength = 2;
	multiplier = 1.5;
	lEdgeList = 0;
	nNodes = 0;
	nEdges = 0;

	nodeList = (int *)malloc((maxNodes+1)*sizeof(int));
	memset(nodeList, 0, (maxNodes+1)*sizeof(int));
	refList = (int *)malloc((maxNodes+1)*sizeof(int));
	memset(refList, 0, (maxNodes+1)*sizeof(int));
	lNodeList = (maxNodes+1);
}


void csr::insert(int src, int dest) {


	if(lEdgeList == nEdges) 
		expandEdgeList();
	

	int foundAt = -1;
	int smallId = -1;
	for(int i=0;i<nNodes;i++) {
		if(nodeList[i] < src)
			smallId = i;
		if(nodeList[i] == src) {
			foundAt = i;
			break;
		}
	}

	if(foundAt == -1) {
		if(lNodeList == nNodes) 
			expandNodeList();
		int insertAt = smallId + 1;

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
	else {
		
		int start = refList[foundAt];
		int end;
		if(foundAt == nNodes-1)
			end = nEdges;
		else
			end = refList[foundAt+1];
		
		for(int i = foundAt+1;i<nNodes;i++)
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
}

void csr::insertInFixedNodelist(int src, int dest) {

	if(lEdgeList == nEdges) 
		expandEdgeList();

	if(nodeList[src] == 0) {
		// no entry !!
		nodeList[src] = src;
		nNodes++;
		int i = src+1;
		int nextEleAt = -1;
		for(;i<lNodeList;i++)
			if(nodeList[i] != 0) {
				if(nextEleAt == -1)
					nextEleAt = i;
				refList[i]++;
			}
		refList[src] = nextEleAt == -1 ? nEdges : refList[nextEleAt] - 1;
		
		for(int i = nEdges; i>refList[src];i--)
			edgeList[i] = edgeList[i-1];
		edgeList[refList[src]] = dest;
		nEdges++;
	}
	else {
		int start = refList[src];
		int i = src+1;
		int nextEleAt = -1;
		for(;i<lNodeList;i++)
			if(nodeList[i] != 0) {
				if(nextEleAt == -1)
					nextEleAt = i;
				refList[i]++;
			}
		int end = nextEleAt == -1 ? nEdges : refList[nextEleAt] - 1;

		// cout << start << ", " << end << "\n";

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

}


void csr::print() {
	for(int i = 0; i < lNodeList; i++) {
		if(nodeList[i] != 0) {
			int start = refList[i];
			int nextEleAt = -1;
			for(int j=i+1;j<lNodeList;j++) 
				if(nodeList[j] != 0) {
					nextEleAt = j;
					break;
				}
			int end = nextEleAt == -1 ? nEdges : refList[nextEleAt];

			cout<<nodeList[i]<<","<<refList[i]<<" -> ";
			for(int j=start;j<end;j++)
				cout<<edgeList[j]<<" ";
			cout<<"\n"; 
		}
	}
	for(int i=0;i<lEdgeList;i++) 
		cout << edgeList[i] << " ";
	cout << "\n";
	cout<<"---\n";
}
