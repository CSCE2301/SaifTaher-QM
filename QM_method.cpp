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

	std::vector<Implicant> imp_v;
	for (const auto& x : terms_vector)
		imp_v.push_back(Implicant(x, n_vars));
	for (const auto& x : dc_vector)
		imp_v.push_back(Implicant(x, n_vars));

	std::vector<std::vector<Implicant>> segmented_imps(n_vars + 1);

	for (const auto& x : imp_v)
		segmented_imps[x.n_ones].push_back(x);

	std::cout << "kappa";
}
