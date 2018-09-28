#include <vector> 
#include <assert.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "pma.h"

using namespace std;


int log2(int n) {
	int lg2 = 0;
	while (n > 1) {
		n /= 2;
		++lg2;
	}
	return lg2;
}


pma::pma(int capacity = 2, double gub = 0.7) : nElems(0) {
	
	assert(capacity > 1);
	assert(1 << log2(capacity) == capacity);

	this->init_vars(capacity);
	this->impl = (int *)malloc(capacity*sizeof(int));
	this->present = (int *)malloc(capacity*sizeof(int));
	memset(this->present, 0, capacity*sizeof(int));
	this->global_upperbound = gub;
	this->tmp = NULL;
}

void pma::init_vars(int capacity) {
	this->chunk_size = 1 << log2(log2(capacity) * 2);
	assert(this->chunk_size == (1 << log2(this->chunk_size)));
	this->nChunks = capacity / this->chunk_size;
	this->nLevels = log2(this->nChunks);
	this->lgn = log2(capacity);
	this->capacity = capacity;
	// printf("init_vars::capacity: %d, nElems: %d, chunk_size: %d, nChunks: %d\n", capacity, nElems, chunk_size, nChunks);
}

double pma::upper_threshold_at(int level) const {
	assert(level <= this->nLevels);
	return level == 0 ? this->global_upperbound : this->global_upperbound - ((this->global_upperbound - 0.5) * level) / (double)this->nLevels;
}

int pma::left_interval_boundary(int i, int interval_size) {
	assert(interval_size == (1 << log2(interval_size)));
	assert(i < (int)this->capacity);
	int q = i / interval_size;
	int boundary = q * interval_size;
	// printf("left_interval_boundary(%d, %d) = %d\n", i, interval_size, boundary);
	return boundary;
}

void pma::resize(int capacity) {
	assert(capacity > this->capacity);
	assert(1 << log2(capacity) == capacity);

	int *tmpi = (int *)malloc(capacity*sizeof(int));
	int *tmpp = (int *)malloc(capacity*sizeof(int));
	memset(tmpp, 0, capacity*sizeof(int));

	double d = (double)capacity / this->nElems;
	int ctr = 0;
	for (int i = 0; i < (int)this->capacity; ++i) {
		if (this->present[i]) {
			int idx = d*(ctr++);
			tmpp[idx] = true;
			tmpi[idx] = this->impl[i];
		}
	}

	free(this->impl);
	free(this->present);
	this->impl = tmpi;
	this->present = tmpp;
	this->init_vars(capacity);
}

void pma::get_interval_stats(int left, int level, bool &in_limit	, int &sz) {
	double t = upper_threshold_at(level);
	int w = (1 << level) * this->chunk_size;
	sz = 0;
	for (int i = left; i < left + w; ++i) 
		sz += this->present[i] ? 1 : 0;
	double q = (double)(sz+1) / double(w);
	in_limit = q <= t;
}

int pma::val_in_chunk(int l, int v) {
	int i;
	for (i = l; i < l + chunk_size; ++i) 
		if (this->present[i]) 
			if (this->impl[i] >= v) 
				return i;
	return i;
}

int pma::lower_bound(int v) {
	int i;
	if (this->nElems == 0) {
		i = this->capacity;
	} 
	else {
		int l = 0, r = this->nChunks;
		int m;
		while (l != r) {
			m = l + (r-l)/2;
			int left = left_interval_boundary(m * chunk_size, chunk_size);
			int pos = val_in_chunk(left, v);
			if (pos == left + chunk_size) 
				l = m + 1;
			else 
				r = m;
		}
		i = l * chunk_size;
	}
	return i;
}

void pma::insert_in_window(int l, int v) {
	this->tmp = (int *)malloc(this->chunk_size*sizeof(int));
	int ntmp = 0;
	bool inserted = false;
	for (int i = l; i < l + this->chunk_size; ++i) {
		if (this->present[i]) {
			if(!inserted && this->impl[i] >= v) {
				this->tmp[ntmp++] = v;
				i--;
				inserted = true;
				continue;
			}
			this->present[i] = 0;
			this->tmp[ntmp++] = this->impl[i];
		}

	}
	if(!inserted)
		this->tmp[ntmp++] = v;

	printf("chunck :: ");
	for(int i=0;i<ntmp;i++)
		printf("%d ", tmp[i]);
	printf("\n");

	for (int i = 0; i < ntmp; ++i) {
		this->present[l + i] = 1;
		this->impl[l + i] = this->tmp[i];
	}
	free(this->tmp);
	++this->nElems;
}

void pma::rebalance_interval(int left, int level) {
	// printf("rebalance_interval(%d, %d)\n", left, level);
	int w = (1 << level) * this->chunk_size;
	this->tmp = (int *)malloc(sizeof(int)*w);
	int ntmp = 0;
	for (int i = left; i < left + w; ++i) {
		if (this->present[i]) {
			this->tmp[ntmp++] = this->impl[i];
			this->present[i] = 0;
		}
	}
	double m = (double)(1<<level)*chunk_size / (double)ntmp;
	assert(m >= 1.0);
	for (int i = 0; i < ntmp; ++i) {
		int k = i * m + left;
		assert(k < left + w);
		this->present[k] = 1;
		this->impl[k] = tmp[i];
	}
	free(this->tmp);
}

void pma::insert(int v) {

	int i = lower_bound(v);
	i = i == this->capacity ? i-1 : i;

	assert(i > -1);
	assert(i < this->capacity);

	int w = chunk_size;
	int level = 0;
	int l = this->left_interval_boundary(i, w);

	int sz;
	bool in_limit;
	get_interval_stats(l, level, in_limit, sz);

	if(in_limit)
		this->insert_in_window(l, v);
	else {
		while (!in_limit) {
			w *= 2;
			level += 1;
			if (level > this->nLevels) {
				this->resize(2 * this->capacity);
				this->insert(v);
				return;
			}

			l = this->left_interval_boundary(i, w);
			get_interval_stats(l, level, in_limit, sz);
			// printf("level: %d, this->nLevels: %d, in_limit: %d, sz: %d\n", level, this->nLevels, in_limit, sz);
		}
		this->rebalance_interval(l, level);
		this->insert(v);
	}
} 

void pma::print() {
	for (int i = 0; i < (int)this->capacity; ++i) {
		printf("%3d ", this->present[i] == 1 ? this->impl[i] : -1);
	}
	printf("\n");
}

int main() {

	pma obj;

	obj.insert(80);
	obj.print();
    obj.insert(50);
	obj.print();
    obj.insert(70);
	obj.print();
    obj.insert(90);
	obj.print();
    obj.insert(65);
	obj.print();
	obj.print();
    obj.insert(85);
	obj.print();
    obj.insert(10);
	obj.print();
    obj.insert(21);
	obj.print();
    obj.insert(22);
	obj.print();
    obj.insert(20);
	obj.print();
    obj.insert(24);
	obj.print();
    obj.insert(15);
	obj.print();
    obj.insert(17);
	obj.print();
    obj.insert(23);
	obj.print();
    

}

