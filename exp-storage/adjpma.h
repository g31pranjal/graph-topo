#include "graphConnectivityStructure.h"
#include "pma.h"

#ifndef ADJNODEPMA
#define ADJNODEPMA

class adjNodePma {
	public :
		int val;
		pma* edgeList;

		adjNodePma();
};

#endif


#ifndef ADJPMA
#define ADJPMA

class adjPma : public graphCon {


	private : 
		int deflength;
		double multiplier;
		adjNodePma** nodeList;
		int lNodeList;
		int nNodes;

		void expandNodeList();
		// void expandEdgeList(adjNode* node);

	public :

		adjPma();
		void insert(int src, int dest);
		void print();

};

#endif