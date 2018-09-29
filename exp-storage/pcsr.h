#include "graphConnectivityStructure.h"

#ifndef PCSR
#define PCSR


class pcsr : public graphCon {

	private :
		int *nodeList;
		int *refList;
		pma_for_csr *edgeList;

		int deflength;
		double multiplier;
		int lNodeList;
		int nNodes;
		int nEdges;

		void expandNodeList();
		void expandEdgeList();


	public :

		csr();
		void insert(int src, int dest);
		void print();

};

#endif