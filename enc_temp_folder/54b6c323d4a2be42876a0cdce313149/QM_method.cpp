#include <iostream>
#include "input.h"
#include "Implicant.h"

int main()
{
	std::string filename = "data.txt";
	uint n_vars;
	bool is_minterms;

	std::vector<uint> terms_vector, dc_vector;
	fetch_data(n_vars, is_minterms, terms_vector, dc_vector, filename);
	if (!is_minterms)
		terms_vector = complement_terms(n_vars, terms_vector, dc_vector);

	std::vector<Implicant> v;
	for (const auto& x : terms_vector)
		v.push_back(Implicant(x, n_vars));
	for (const auto& x : dc_vector)
		v.push_back(Implicant(x, n_vars));

	int delta_index = v[0].delta_one(v[1]);
	Implicant test1(v[0], v[1], delta_index);

	delta_index = v[5].delta_one(v[3]);
	Implicant test2(v[5], v[3], delta_index);

	delta_index = test1.delta_one(test2);
	Implicant test3(test1, test2, delta_index);

	std::cout << "kappa";
}
