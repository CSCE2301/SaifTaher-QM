#include <iostream>
#include "input.h"

int main()
{
	std::string filename = "data.txt";
	uint n_vars;
	bool is_minterms;

	std::vector<uint> terms_vector, dc_vector;
	fetch_data(n_vars, is_minterms, terms_vector, dc_vector, filename);
	if (!is_minterms)
		terms_vector = complement_terms(n_vars, terms_vector, dc_vector);

	std::cout << "kappa";
}
