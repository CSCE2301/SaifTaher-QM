#include <iostream>
#include "input.h"
#include "Implicant.h"
#include "combine.h"
#include "matrix.h"

std::vector<std::vector<Implicant>, std::vector<uint>> reduce_PI_chart(const std::vector<Implicant>& PIs, const std::vector<uint>& minterms);

int main()
{
	std::string filename = "data.txt";
	uint n_vars;
	bool is_minterms;

	std::vector<uint> terms_vector, dc_vector;
	fetch_data(n_vars, is_minterms, terms_vector, dc_vector, filename);
	if (!is_minterms)
		terms_vector = complement_terms(n_vars, terms_vector, dc_vector);

	std::sort(terms_vector.begin(), terms_vector.end());
	std::sort(dc_vector.begin(), dc_vector.end());

	std::map<uint, uint> minterm_2_bit_index;

	for (int i = 0; i < terms_vector.size(); i++)
		minterm_2_bit_index[terms_vector[i]] = i;

	std::set<Implicant*, ImplicantPtrCompare> imp_set;
	for (const auto& x : terms_vector)
		imp_set.insert(new Implicant(x, n_vars));
	for (const auto& x : dc_vector)
		imp_set.insert(new Implicant(x, n_vars));

	std::vector<std::set<Implicant*, ImplicantPtrCompare>> segmented_imps(n_vars + 1);

	for (const auto& x : imp_set)
		segmented_imps[x->n_ones].insert(new Implicant(*x));

	std::set<Implicant> PI_set = PIs(segmented_imps, n_vars);
	std::vector<Implicant> filtered_PIs; filtered_PIs.reserve(PI_set.size());

	for (const auto& x : PI_set)
	{
		if (x.terms.size() > 1)
			filtered_PIs.push_back(x);
		else
		{
			auto minterm_it = std::lower_bound(dc_vector.begin(), dc_vector.end(), *x.terms.begin());
			if (*x.terms.begin() != *minterm_it)
				filtered_PIs.push_back(x);
		}
	}

	std::vector<Implicant>(filtered_PIs).swap(filtered_PIs);

	std::cout << "Prime implicants:\n\n";
	for (const auto& x : filtered_PIs)
	{
		std::cout << "(";
		uint counter = 0;

		for (auto it = x.terms.begin(); it != x.terms.end(); ++it)
		{
			std::cout << *it;
			if (counter != x.terms.size() - 1)
				std::cout << ", ";
			else
				std::cout << ")";
			counter++;
		}

		std::cout << " || " << x.binary_rep << "\n\n";
	}

	std::vector<Implicant> EPIs, nEPIs;
	std::vector<uint> E_minterms, nE_minterms;

	std::pair<std::vector<Implicant>, std::vector<uint>> reduced = reduce_PI_chart(filtered_PIs, terms_vector, minterm_2_bit_index);

	std::cout << "kappa";
}


