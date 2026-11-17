#include <iostream>
#include "input.h"
#include "Implicant.h"
#include "combine.h"
#include "matrix.h"

std::vector<std::vector<Implicant>, std::vector<uint>> reduce_PI_chart(const std::vector<Implicant>& PIs, const std::vector<uint>& minterms);

std::string binary_rep_2_bool_expr(std::string binary_rep)
{
	std::string expr;
	uint counter = 0;
	for (const char& c : binary_rep)
	{
		switch (c)
		{
		case '1':
			expr += char(65 + counter);
			break;
		case '0':
			expr += char(65 + counter);
			expr += "\'";
			break;
		case '-':
			break;
		default:
			throw std::runtime_error("Invalid binary representation of an implicant.");
		}
		counter++;
	}
	return expr;
}

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
	segregate_essential(filtered_PIs, terms_vector, EPIs, nEPIs, E_minterms, nE_minterms);

	std::cout << "Essential prime implicants:\n";

	for (const auto& x : EPIs)
		std::cout << binary_rep_2_bool_expr(x.binary_rep) << '\n';
	std::cout << '\n';

	std::cout << "Minterms not covered by prime implicants:\n";

	for (const auto& x : nE_minterms)
		std::cout << x << '\n';
	std::cout << '\n';

	std::map<uint, uint> minterm_2_bit_index;

	for (int i = 0; i < nE_minterms.size(); i++)
		minterm_2_bit_index[nE_minterms[i]] = i;

	std::pair<std::vector<Implicant>, std::vector<uint>> reduced = reduce_PI_chart(nEPIs, nE_minterms, minterm_2_bit_index);

	std::cout << "kappa";
}


