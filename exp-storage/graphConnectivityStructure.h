#ifndef GRAPHCON
#define GRAPHCON

class graphCon {
	public :
		virtual void insert(int src, int dest) = 0; 
		virtual void print() = 0;  
		// virtual void khops(int k, int iter, int seed) = 0;

};


#endif