#pragma once
#include <iostream>
#include <bitset>
#include <vector>
#include <map>
#include <boost/dynamic_bitset.hpp>
#include "Implicant.h"

std::vector<boost::dynamic_bitset<>> transpose(std::vector<boost::dynamic_bitset<>> original);
std::pair<std::vector<Implicant>, std::vector<uint>> reduce_PI_chart(const std::vector<Implicant>& PIs, const std::vector<uint>& minterms, std::map<uint, uint> minterm_2_bit_index);

template <class T>
std::vector<boost::dynamic_bitset<>> reduce_matrix(std::vector<boost::dynamic_bitset<>> original, const std::vector<T>& row_entries, std::vector<T>& reduced_row_entries, bool remove_dominated = true)
{
	std::vector<uint> rows_2_remove; rows_2_remove.reserve(row_entries.size());
	boost::dynamic_bitset<> and_result(original[0].size());

	for (int i = 0; i < row_entries.size() - 1; i++)
	{
		for (int j = i + 1; j < row_entries.size(); j++)
		{
			and_result = original[i] & original[j];

			if (and_result == original[i])
			{
				if (remove_dominated)
					rows_2_remove.push_back(i);
				else
					rows_2_remove.push_back(j);
			}

			else if (and_result == original[j])
			{
				if (remove_dominated)
					rows_2_remove.push_back(j);
				else
					rows_2_remove.push_back(i);
			}
		}
	}

	reduced_row_entries.reserve(row_entries.size() - rows_2_remove.size());
	std::vector<boost::dynamic_bitset<>> reduced_chart; reduced_chart.reserve(row_entries.size() - rows_2_remove.size());
	uint removed_counter = 0;

	for (int i = 0; i < row_entries.size(); i++)
	{
		if (removed_counter < rows_2_remove.size() && rows_2_remove[removed_counter] == i)
		{
			removed_counter++;
			continue;
		}
		reduced_row_entries.push_back(row_entries[i]);
		reduced_chart.push_back(original[i]);
	}

	return reduced_chart;
}