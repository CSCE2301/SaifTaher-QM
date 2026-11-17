#include "matrix.h"

std::vector<boost::dynamic_bitset<>> transpose(std::vector<boost::dynamic_bitset<>> original)
{
	uint n_rows = original.size(), n_cols = original[0].size();
	std::vector<boost::dynamic_bitset<>> transposed(n_cols, boost::dynamic_bitset<>(n_rows));

	for (int i = 0; i < n_rows; i++)
		for (int j = 0; j < n_cols; j++)
			transposed[j][i] = original[i][j];

	return transposed;
}

std::pair<std::vector<Implicant>, std::vector<uint>> reduce_PI_chart(const std::vector<Implicant>& PIs, const std::vector<uint>& minterms, std::map<uint, uint> minterm_2_bit_index)
{
	std::vector<boost::dynamic_bitset<>> chart(PIs.size(), boost::dynamic_bitset<>(minterms.size()));

	for (int i = 0; i < PIs.size(); i++)
	{
		for (const auto& x : PIs[i].terms)
		{
			if (minterm_2_bit_index.find(x) != minterm_2_bit_index.end())
			{
				chart[i][minterm_2_bit_index[x]] = 1;
			}
		}
	}

	std::vector<Implicant> reduced_PIs;
	std::vector<boost::dynamic_bitset<>> reduced_chart = reduce_matrix(chart, PIs, reduced_PIs);

	std::vector<uint> reduced_minterms;
	std::vector<boost::dynamic_bitset<>> transposed = transpose(reduced_chart);
	reduce_matrix(transposed, minterms, reduced_minterms, false);

	return std::pair<std::vector<Implicant>, std::vector<uint>>(reduced_PIs, reduced_minterms);
}

void segregate_essential(const std::vector<Implicant>& PIs, const std::vector<uint> minterms, std::vector<Implicant>& EPIs, std::vector<Implicant>& nEPIs, std::vector<uint>& E_minterms, std::vector<uint>& nE_minterms)
{
	E_minterms.reserve(minterms.size()); nE_minterms.reserve(minterms.size());
	EPIs.reserve(PIs.size()); nEPIs.reserve(PIs.size());
	std::map<uint, std::vector<const Implicant*>> coverage;
	std::map<const Implicant*, bool> added;

	std::map<uint, bool> is_minterm;
	for (const auto& x : minterms)
		is_minterm[x] = true;

	for (int i = 0; i < PIs.size(); i++)
		for (const auto& y : PIs[i].terms)
			if (is_minterm[y])
				coverage[y].push_back(&PIs[i]);

	for (auto it = coverage.begin(); it != coverage.end(); ++it)
	{
		if (it->second.size() == 1)
		{
			if (!added[it->second[0]])
			{
				EPIs.push_back(*it->second[0]);
				added[it->second[0]] = true;
			}
			E_minterms.push_back(it->first);
		}
	}

	std::map<uint, bool> covered_by_EPIs;
	std::map<uint, bool> added_to_nE_terms;

	for (const auto& x : EPIs)
		for (const auto& y : x.terms)
			if (is_minterm[y])
				covered_by_EPIs[y] = true;

	for (auto it = coverage.begin(); it != coverage.end(); ++it)
	{
		if (it->second.size() != 1)
		{
			for (int i = 0; i < it->second.size(); i++)
			{
				if (!added[it->second[i]])
				{
					nEPIs.push_back(*it->second[i]);
					added[it->second[i]] = true;
				}
			}
		}
	}

	for (const auto& x : minterms)
		if (!covered_by_EPIs[x] && !added_to_nE_terms[x])
		{
			nE_minterms.push_back(x);
			added_to_nE_terms[x] = true;
		}

	std::vector<Implicant>(EPIs).swap(EPIs); std::vector<Implicant>(nEPIs).swap(nEPIs);
	std::vector<uint>(E_minterms).swap(E_minterms); std::vector<uint>(nE_minterms).swap(nE_minterms);
}