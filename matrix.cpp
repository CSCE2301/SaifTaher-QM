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