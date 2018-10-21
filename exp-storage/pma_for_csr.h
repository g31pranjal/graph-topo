#include <vector> 
#include <assert.h> 
#include <stdio.h> 

using namespace std;

#ifndef PMA_CSR
#define PMA_CSR

struct pma_for_csr {
	private :

		int **wrapper;

		typedef struct comp {
			int val;
			int typ; // type : 0 for normal, 1 for backref
			comp(int a, int b) : val(a), typ(b) {}
		} ent;

		ent** impl;
		int* present;
		int capacity;
		int nElems;
		int chunk_size;
		int nChunks;
		int nLevels;
		int lgn;
		ent**  tmp;
		double global_upperbound;
		int endMarkerLoc;

		void init_vars(int capacity);
		double upper_threshold_at(int level) const;
		int left_interval_boundary(int i, int interval_size);
		void resize(int capacity);
		void get_interval_stats(int left, int level, bool &in_limit	, int &sz);
		int val_in_chunk(int l, int v);
		void insert_in_window(int rl, int rh, int l, int v);
		int lower_bound(int v);
		// will be as it is.
		void rebalance_interval(int left, int level);
		int log2(int n);
		int insert_s(ent * s, int loc);
		int insertMarker(int at, int nref);

	public :
		pma_for_csr(int ** reflistadr, int capacity, double gub);
		void insert(int l, int v);
		bool initInsert(int cas, int insertLoc, int v, int nref);
		void print();
		void updateBackref(int ref, int newVal);

};


#endif
