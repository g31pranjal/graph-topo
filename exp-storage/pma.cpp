#include <vector> 
#include <assert.h> 
#include <stdio.h> 
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
	this->impl.resize(capacity);
	this->present.resize(capacity);
	this->global_upperbound = gub;
}

void pma::init_vars(int capacity) {
	this->chunk_size = 1 << log2(log2(capacity) * 2);
	assert(this->chunk_size == (1 << log2(this->chunk_size)));
	this->nChunks = capacity / this->chunk_size;
	this->nLevels = log2(this->nChunks);
	this->lgn = log2(capacity);
	// printf("init_vars::capacity: %d, nElems: %d, chunk_size: %d, nChunks: %d\n", capacity, nElems, chunk_size, nChunks);
}

double pma::upper_threshold_at(int level) const {
	assert(level <= this->nLevels);
	return level == 0 ? this->global_upperbound : this->global_upperbound - ((this->global_upperbound - 0.5) * level) / (double)this->nLevels;
}

int pma::left_interval_boundary(int i, int interval_size) {
	assert(interval_size == (1 << log2(interval_size)));
	assert(i < (int)this->impl.size());
	int q = i / interval_size;
	int boundary = q * interval_size;
	// printf("left_interval_boundary(%d, %d) = %d\n", i, interval_size, boundary);
	return boundary;
}

void pma::resize(int capacity) {
	assert(capacity > this->impl.size());
	assert(1 << log2(capacity) == capacity);

	vector<int> tmpi(capacity);
	vector<bool> tmpp(capacity);
	double d = (double)capacity / this->nElems;
	int ctr = 0;
	for (int i = 0; i < (int)this->impl.size(); ++i) {
		if (this->present[i]) {
			int idx = d*(ctr++);
			tmpp[idx] = true;
			tmpi[idx] = this->impl[i];
		}
	}
	this->impl.swap(tmpi);
	this->present.swap(tmpp);
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
		i = this->impl.size();
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
	tmp.clear();
	tmp.reserve(this->chunk_size);
	for (int i = l; i < l + this->chunk_size; ++i) {
		if (this->present[i]) {
			this->present[i] = false;
			tmp.push_back(this->impl[i]);
		}
	}
	vector<int>::iterator iter = std::lower_bound(tmp.begin(), tmp.end(), v);
	tmp.insert(iter, v);

	for (int i = 0; i < tmp.size(); ++i) {
		this->present[l + i] = true;
		this->impl[l + i] = tmp[i];
	}
	++this->nElems;
}

void pma::rebalance_interval(int left, int level) {
	printf("rebalance_interval(%d, %d)\n", left, level);
	int w = (1 << level) * this->chunk_size;
	tmp.clear();
	tmp.reserve(w);
	for (int i = left; i < left + w; ++i) {
		if (this->present[i]) {
			tmp.push_back(this->impl[i]);
			this->present[i] = false;
		}
	}
	double m = (double)(1<<level)*chunk_size / (double)tmp.size();
	assert(m >= 1.0);
	for (int i = 0; i < tmp.size(); ++i) {
		int k = i * m + left;
		assert(k < left + w);
		this->present[k] = true;
		this->impl[k] = tmp[i];
	}
}

void pma::insert(int v) {

	int i = lower_bound(v);
	i = i == this->impl.size() ? i-1 : i;

	assert(i > -1);
	assert(i < this->impl.size());

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
				this->resize(2 * this->impl.size());
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
	for (int i = 0; i < (int)this->impl.size(); ++i) {
		printf("%3d ", this->present[i] ? this->impl[i] : -1);
	}
	printf("\n");
}

