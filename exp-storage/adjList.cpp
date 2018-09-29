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
	nodeList = NULL;
	lNodeList = 0;
	nNodes = -1;
	nodeList = (adjNode**)malloc(deflength*sizeof(adjNode*));
	nNodes = 0;
	lNodeList = deflength;

}

void adjList::insert(int src, int dest) {

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

		int* el = nodeList[l]->edgeList;
		int eSize = nodeList[l]->elsize;
		if(el[eSize-1] != 0) {
			expandEdgeList(nodeList[l]);
			el = nodeList[l]->edgeList;
			eSize = nodeList[l]->elsize;
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

		adjNode* n = new adjNode();
		n->val = src;
		int* el = (int *)malloc(deflength*sizeof(int));
		memset(el, 0, deflength*sizeof(int));
		el[0] = dest;
		n->edgeList = el;
		n->elsize = deflength;

		if(nNodes+1 > lNodeList) 
			expandNodeList();

		int insertAt = l;

		for(int i = nNodes-1;i >= insertAt;i--) {
			nodeList[i+1] = nodeList[i];
		}
		nodeList[insertAt] = n;
		nNodes++;
		
	}
	return;
}


void adjList::print() {
	cout<<lNodeList<<"\n";
	for(int i=0;i<lNodeList && nodeList[i] != NULL && nodeList[i]->val != -1;i++) {
		cout<<nodeList[i]->val<<" -> ";
		for(int j=0;j<nodeList[i]->elsize;j++) {
			cout<<nodeList[i]->edgeList[j]<<" ";
		}
		cout<<"\n";
	}
	cout<<"---\n";
}
