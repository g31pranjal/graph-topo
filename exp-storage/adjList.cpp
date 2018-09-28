#include "graphConnectivityStructure.h"
#include "adjList.h"
#include <iostream>
#include <string.h>

using namespace std;


adjNode::adjNode() {
	val = -1;
	edgeList = NULL;
	elsize = 0;
} 


void adjList::expandNodeList() {			
	int newSize = (int)lNodeList*multiplier;
	adjNode** newNodeList = (adjNode**)malloc(newSize*sizeof(adjNode*));
	for(int i=0;i<nNodes;i++) 
		newNodeList[i] = nodeList[i];
	free(nodeList);
	nodeList = newNodeList;
	lNodeList = newSize;
	return;
}


void adjList::expandEdgeList(adjNode* node) {
	int oldSize = node->elsize;
	int newSize = int(oldSize*multiplier);
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
	deflength = 2;
	multiplier = 1.5;
	nodeList = (adjNode**)malloc(deflength*sizeof(adjNode*));
	nNodes = 0;
	lNodeList = deflength;
}


adjList::adjList(int maxNodes) {
	deflength = 2;
	multiplier = 1.5;
	nodeList = (adjNode**)malloc((maxNodes+1)*sizeof(adjNode*));
	memset(nodeList, 0, (maxNodes+1)*sizeof(adjNode*));
	nNodes = 0;
	lNodeList = maxNodes+1;
}


void adjList::insert(int src, int dest) {

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

		adjNode* n = new adjNode();
		n->val = src;
		int* el = (int *)malloc(deflength*sizeof(int));
		memset(el, 0, deflength*sizeof(int));
		el[0] = dest;
		n->edgeList = el;
		n->elsize = deflength;

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

		int* el = nodeList[foundAt]->edgeList;
		int eSize = nodeList[foundAt]->elsize;
		if(el[eSize-1] != 0) {
			expandEdgeList(nodeList[foundAt]);
			el = nodeList[foundAt]->edgeList;
			eSize = nodeList[foundAt]->elsize;
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
	return;
}

void adjList::insertInFixedNodelist(int src, int dest) {

	if(nodeList[src] == NULL) {
		adjNode* n = new adjNode();
		n->val = src;
		int* el = (int *)malloc(deflength*sizeof(int));
		memset(el, 0, deflength*sizeof(int));
		n->edgeList = el;
		n->elsize = deflength;
		nodeList[src] = n;
		
		el[0] = dest;
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
