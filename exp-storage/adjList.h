#include "graphConnectivityStructure.h"

#ifndef ADJNODE
#define ADJNODE

class adjNode {
	public :
		int val;
		int elsize;
		int* edgeList;

		adjNode();
};

#endif


#ifndef ADJLIST
#define ADJLIST

class adjList : public graphCon {


	private : 
		int deflength;
		double multiplier;
		adjNode** nodeList;
		int lNodeList;
		int nNodes;

		void expandNodeList();
		void expandEdgeList(adjNode* node);

	public :

		adjList();
		void insert(int src, int dest);
		void print();

};

#endif