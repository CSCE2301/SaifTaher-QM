#include "combine.h"

std::vector<std::set<Implicant*, ImplicantPtrCompare>> combine(std::vector<std::set<Implicant*, ImplicantPtrCompare>> prev_iteration, bool& changed, uint n_ones_max)
{
	std::vector<std::set<Implicant*, ImplicantPtrCompare>> next_iteration(n_ones_max);
	int delta_ones;
	Implicant* combination;

	for (int i = 0; i < prev_iteration.size() - 1; i++)
	{
		if (prev_iteration[i].size() == 0 || prev_iteration[i + 1].size() == 0)
			continue;
		for (const auto &x: prev_iteration[i])
		{
			for (const auto& y : prev_iteration[i + 1])
			{
				delta_ones = x->delta_one(*y);
				if (delta_ones != -1)
				{
					changed = true;
					combination = new Implicant(*x, *y, delta_ones);
					next_iteration[combination->n_ones].insert(combination);
				}
			}
		}
	}

	return next_iteration;
}

std::set<Implicant> PIs(std::vector<std::set<Implicant*, ImplicantPtrCompare>> first_iteration, uint n_vars)
{
	std::vector<std::vector<std::set<Implicant*, ImplicantPtrCompare>>> all_iterations;
	std::vector<std::set<Implicant*, ImplicantPtrCompare>> next_iteration;
	all_iterations.push_back(first_iteration);

	bool changed;
	int counter = 1;
	do
	{
		changed = false;
		next_iteration = combine(all_iterations[all_iterations.size() - 1], changed, n_vars + 1 - counter);
		counter++;
		if (changed)
			all_iterations.push_back(next_iteration);
	} while (changed);

	std::set<Implicant> PI_set;

	for (int i = 0; i < all_iterations.size(); i++)
	{
		for (int j = 0; j < all_iterations[i].size(); j++)
		{
			for (const auto& x : all_iterations[i][j])
				if (x->is_prime)
					PI_set.insert(*x);
		}
	}

	return PI_set;
}