#include "csr.h"
#include <iostream>
#include <string.h>
#include <deque>
#include <vector>
#include <tuple>

using namespace std;


void csr::expandNodeList() {
	int oldSize = this->lNodeList;
	int newSize = oldSize*this->multiplier;
	int *newNodeList = (int *)malloc(newSize*sizeof(int));
	memset(newNodeList, 0, newSize*sizeof(int));
	int *newRefList = (int *)malloc(newSize*sizeof(int));
	memset(newRefList, 0, newSize*sizeof(int));
	for(int i=0;i<this->nNodes;i++) {
		newNodeList[i] = this->nodeList[i];
		newRefList[i] = this->refList[i];
	}
	free(this->nodeList);
	free(this->refList);
	this->nodeList = newNodeList;
	this->refList = newRefList;
	this->lNodeList = newSize;
}


void csr::expandEdgeList() {
	int oldSize = this->lEdgeList;
	int newSize = (int)oldSize*this->multiplier;
	if(newSize == 0)
		newSize = this->deflength;
	int *newEdgeList = (int *)malloc(newSize*sizeof(int));
	memset(newEdgeList, 0, newSize*sizeof(int));
	for(int i=0;i<this->nEdges;i++) {
		newEdgeList[i] = this->edgeList[i];
	}
	if(this->lEdgeList != 0)
		free(this->edgeList);
	this->edgeList = newEdgeList;	
	this->lEdgeList = newSize;
}


csr::csr() {
	this->deflength = 2;
	this->multiplier = 1.5;
	this->lNodeList = 0;
	this->lEdgeList = 0;
	this->nNodes = 0;
	this->nEdges = 0;

	this->nodeList = (int *)malloc(this->deflength*sizeof(int));
	memset(this->nodeList, 0, this->deflength*sizeof(int));
	this->refList = (int *)malloc(this->deflength*sizeof(int));
	memset(this->refList, 0, this->deflength*sizeof(int));
	this->lNodeList = this->deflength;
}

csr::csr(int maxNodes) {
	this->deflength = 2;
	this->multiplier = 1.5;
	this->lEdgeList = 0;
	this->nNodes = 0;
	this->nEdges = 0;

	this->nodeList = (int *)malloc((maxNodes+1)*sizeof(int));
	memset(this->nodeList, 0, (maxNodes+1)*sizeof(int));
	this->refList = (int *)malloc((maxNodes+1)*sizeof(int));
	memset(this->refList, 0, (maxNodes+1)*sizeof(int));
	this->lNodeList = (maxNodes+1);
}


void csr::k2hops(int iter) {

	// printf("in khops, nNodes : %d\n", this->	nNodes);

	deque< tuple <int, int, int> > opr; 
	vector< tuple <int, int, int> > fnl; 
	tuple <int, int, int> retrv;

	while(fnl.size() < iter) {
		int r = rand() % this->lNodeList;
		while(this->nodeList[r] == 0) 
			r = rand() % this->lNodeList;

		int val = this->nodeList[r];
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
				if(nodeList[m] < head)
					l = m+1;
				else 
					r = m;
			}

			// printf("found node at %d\n", l);

			if(l != this->nNodes && this->nodeList[l] == head) {
				int rl = this->refList[l];
				int rh = rl;
				if(l+1 < this->nNodes)
					rh = this->refList[l+1];
				else 
					rh = this->nEdges;
				
				for(int i=rl; i < rh; i++) {
					if(noe == 1) {
						get<2>(retrv) = this->edgeList[i];
						fnl.push_back(make_tuple(get<0>(retrv), get<1>(retrv), get<2>(retrv)));
						if(fnl.size() >= iter)
							break;
						 	// printf("completed %d %d %d\n", get<0>(retrv), get<1>(retrv), get<2>(retrv));
					}
					else if(noe == 0) {
						get<1>(retrv) = this->edgeList[i];
						opr.push_back(make_tuple(get<0>(retrv), get<1>(retrv), -1));
					}
				}
			}
		}
		// cout << fnl.size() << "\n";
	}
}


void csr::k3hops(int iter) {

	// printf("in khops, nNodes : %d\n", this->	nNodes);

	deque< tuple <int, int, int, int> > opr; 
	vector< tuple <int, int, int, int> > fnl; 
	tuple <int, int, int, int> retrv;

	while(fnl.size() < iter) {
		int r = rand() % this->lNodeList;
		while(this->nodeList[r] == 0) 
			r = rand() % this->lNodeList;

		int val = this->nodeList[r];
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
				if(nodeList[m] < head)
					l = m+1;
				else 
					r = m;
			}

			// printf("found node at %d\n", l);

			if(l != this->nNodes && this->nodeList[l] == head) {
				int rl = this->refList[l];
				int rh = rl;
				if(l+1 < this->nNodes)
					rh = this->refList[l+1];
				else 
					rh = this->nEdges;
				
				for(int i=rl; i < rh; i++) {
					if(noe == 2) {
						get<3>(retrv) = this->edgeList[i];
						fnl.push_back(make_tuple(get<0>(retrv), get<1>(retrv), get<2>(retrv), get<3>(retrv)));
						if(fnl.size() >= iter)
							break;
						// printf("completed %d %d %d %d\n", get<0>(retrv), get<1>(retrv), get<2>(retrv), get<3>(retrv));
					}
					else if(noe == 0) {
						get<1>(retrv) = this->edgeList[i];
						opr.push_back(make_tuple(get<0>(retrv), get<1>(retrv), -1, -1));
						// printf("level 1\n");
					}
					else if(noe == 1) {
						get<2>(retrv) = this->edgeList[i];
						opr.push_back(make_tuple(get<0>(retrv), get<1>(retrv), get<2>(retrv), -1));
						// printf("level 2\n");
					}
				}
			}
		}
		// cout << fnl.size() << "\n";
	}
}


void csr::insert(int src, int dest) {

	if(this->lEdgeList == this->nEdges) 
		this->expandEdgeList();
	
	int l = 0;
	int r = this->nNodes;
	int m;
	while(l != r) {
		m = l + (r - l)/2;
		if(this->nodeList[m] < src)
			l = m+1;
		else 
			r = m;
	}

	if(l != this->nNodes && this->nodeList[l] == src) {

		int start = this->refList[l];
		int end;
		if(l == this->nNodes-1)
			end = this->nEdges;
		else
			end = this->refList[l+1];
		
		for(int i = l+1;i<this->nNodes;i++)
			this->refList[i]++;

		int smallId = start - 1;
		for(int i=start;i<end;i++) {
			if(this->edgeList[i] < dest) 
				smallId = i;
		}
		int insertAt = smallId + 1;

		for(int i=this->nEdges;i>insertAt;i--)
			this->edgeList[i] = this->edgeList[i-1];
		this->edgeList[insertAt] = dest;
		this->nEdges++;

	}
	else {
		
		if(this->lNodeList == this->nNodes) 
			this->expandNodeList();
		int insertAt = l;

		for(int i=this->nNodes;i>insertAt;i--) {
			this->nodeList[i] = this->nodeList[i-1];
			this->refList[i] = this->refList[i-1] + 1;
		}
		this->nodeList[insertAt] = src;
		if(insertAt==this->nNodes)
			this->refList[insertAt] = this->nEdges;
		this->nNodes++;

		int eInsertAt = this->refList[insertAt];
		for(int i=this->nEdges;i>eInsertAt;i--)
			this->edgeList[i] = this->edgeList[i-1];
		this->edgeList[eInsertAt] = dest;
		nEdges++;
		
	}
}

void csr::insertInFixedNodelist(int src, int dest) {

	if(this->lEdgeList == this->nEdges) 
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
