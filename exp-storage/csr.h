#include "graphConnectivityStructure.h"

#ifndef CSR
#define CSR


class csr : public graphCon {

	private :
		int *nodeList;
		int *refList;
		int *edgeList;

		int deflength;
		double multiplier;
		int lNodeList;
		int lEdgeList;
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