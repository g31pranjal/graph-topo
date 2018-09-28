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
		csr(int maaxNodes);
		void insert(int src, int dest);
		void insertInFixedNodelist(int src, int dest);
		void print();

};

#endif