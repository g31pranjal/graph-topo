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
		adjNodePma* createAdjNodePma(int src);

	public :

		adjPma();
		adjPma(int maxNodes);
		void insert(int src, int dest);
		void insertInFixedNodelist(int src, int dest);
		void khops(int k, int iter, int seed);
		void print();

};

#endif