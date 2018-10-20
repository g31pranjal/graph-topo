#include <vector> 
#include <assert.h> 
#include <stdio.h> 

using namespace std;

#ifndef PMA
#define PMA

struct pma {
	private :
		int* impl;
		int* present;
		int capacity;
		int nElems;
		int chunk_size;
		int nChunks;
		int nLevels;
		int lgn;
		int*  tmp;
		double global_upperbound;
		void init_vars(int capacity);
		double upper_threshold_at(int level) const;
		int left_interval_boundary(int i, int interval_size);
		void resize(int capacity);
		void get_interval_stats(int left, int level, bool &in_limit	, int &sz);
		int val_in_chunk(int l, int v);
		int lower_bound(int v);
		void insert_in_window(int l, int v);
		void rebalance_interval(int left, int level);
		int log2(int n);

	public :
		pma(int capacity, double gub);
		void insert(int v);
		void print();
		vector<int> linearTraverse();
};


#endif
