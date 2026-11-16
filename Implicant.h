#pragma once

#include <set>
#include <string>
#include <algorithm>
#include <stdexcept>

typedef unsigned int uint;

class Implicant
{
public:
	std::set<uint> terms;
	std::string binary_rep;
	bool is_prime;
	int n_ones;

	Implicant();
	Implicant(uint term, uint n_vars);
	Implicant(Implicant& i1, Implicant& i2, int delta_index);
	Implicant(const Implicant& i2);

	int delta_one(const Implicant& i2) const;
	bool operator<(const Implicant& i2) const;
};

// For checking for duplicates in std::set
struct ImplicantPtrCompare {
	bool operator()(const Implicant* a, const Implicant* b) const {
		return a->binary_rep < b->binary_rep;
	}
};