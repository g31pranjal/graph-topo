#include "graphConnectivityStructure.h"
#include "adjList.h"
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <deque>
#include <tuple>

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


void adjList::k2hops(int iter) {

	// printf("in khops, nNodes : %d\n", this->	nNodes);

	deque< tuple <int, int, int> > opr; 
	vector< tuple <int, int, int> > fnl; 
	tuple <int, int, int> retrv;

	while(fnl.size() < iter) {
		int r = rand() % this->lNodeList;
		while(this->nodeList[r] == NULL) 
			r = rand() % this->lNodeList;

		int val = this->nodeList[r]->val;
		opr.push_back(make_tuple(val, -1, -1));

		// printf("opr size : %d\n", opr.size());

		while(opr.size() != 0) {
			retrv = opr.front();
			opr.pop_front();


			int noe = -1;
			int head;
			if(get<0>(retrv) != -1) {
				noe = 0;
				head = get<0>(retrv);
				// printf("1 ele in retrv\n");
			}
			if(get<1>(retrv) != -1) {
				noe = 1;
				head = get<1>(retrv);
			}

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
				for(int i=0; i < elSize && el[i] != 0; i++) {
					if(noe == 1) {
						get<2>(retrv) = el[i];
						fnl.push_back(make_tuple(get<0>(retrv), get<1>(retrv), get<2>(retrv)));
						if(fnl.size() >= iter)
							break;
						 	// printf("completed %d %d %d\n", get<0>(retrv), get<1>(retrv), get<2>(retrv));
					}
					else if(noe == 0) {
						get<1>(retrv) = el[i];
						opr.push_back(make_tuple(get<0>(retrv), get<1>(retrv), -1));
					}
				}
			}
		}
		// cout << fnl.size() << "\n";
	}
}


void adjList::k3hops(int iter) {

	// printf("in khops, nNodes : %d\n", this->	nNodes);

	deque< tuple <int, int, int, int> > opr; 
	vector< tuple <int, int, int, int> > fnl; 
	tuple <int, int, int, int> retrv;

	while(fnl.size() < iter) {
		int r = rand() % this->lNodeList;
		while(this->nodeList[r] == NULL) 
			r = rand() % this->lNodeList;

		int val = this->nodeList[r]->val;
		opr.push_back(make_tuple(val, -1, -1, -1));

		// printf("opr size : %d\n", opr.size());

		while(opr.size() != 0) {
			retrv = opr.front();
			opr.pop_front();

			// printf("retrv %d %d %d %d\n", get<0>(retrv), get<1>(retrv), get<2>(retrv), get<3>(retrv));

			int noe = -1;
			int head;
			if(get<0>(retrv) != -1) {
				noe = 0;
				head = get<0>(retrv);
				// printf("1 ele in retrv\n");
			}
			if(get<1>(retrv) != -1) {
				noe = 1;
				head = get<1>(retrv);
			}
			if(get<2>(retrv) != -1) {
				noe = 2;
				head = get<2>(retrv);
			}

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
				for(int i=0; i < elSize && el[i] != 0; i++) {
					if(noe == 2) {
						get<3>(retrv) = el[i];
						fnl.push_back(make_tuple(get<0>(retrv), get<1>(retrv), get<2>(retrv), get<3>(retrv)));
						if(fnl.size() >= iter)
							break;
						// printf("completed %d %d %d %d\n", get<0>(retrv), get<1>(retrv), get<2>(retrv), get<3>(retrv));
					}
					else if(noe == 0) {
						get<1>(retrv) = el[i];
						opr.push_back(make_tuple(get<0>(retrv), get<1>(retrv), -1, -1));
						// printf("level 1\n");
					}
					else if(noe == 1) {
						get<2>(retrv) = el[i];
						opr.push_back(make_tuple(get<0>(retrv), get<1>(retrv), get<2>(retrv), -1));
						// printf("level 2\n");
					}
				}
			}
		}
		// cout << fnl.size() << "\n";
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
