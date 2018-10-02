#include "graphConnectivityStructure.h"
#include "pma_for_csr.h"

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

		void expandNodeList();

	public :

		pcsr();
		void insert(int src, int dest);
		void print();

};

#endif