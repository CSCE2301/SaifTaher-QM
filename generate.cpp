#include "generate.h"

typedef unsigned int uint;

void gen_data(std::string filename, int n_vars)
{
	std::ofstream out(filename);
	out << n_vars << '\n';

	double minterm_ratio = 0.3, dc_ratio = 0.1;

	uint minterm_max_count = (1 << n_vars), minterm_actual_count = minterm_max_count * minterm_ratio, dc_count = minterm_max_count * dc_ratio;
	std::vector<bool> used(minterm_max_count, 0);

	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(0, minterm_max_count - 1);
	uint current;

	for (int i = 0; i < minterm_actual_count; i++)
	{
		do
		{
			current = dist(rng);
		} while (used[current]);

		out << "m" << current;
		used[current] = true;

		if (i != minterm_actual_count - 1)
			out << ",";
	}

	out << '\n';

	for (int i = 0; i < dc_count; i++)
	{
		do
		{
			current = dist(rng);
		} while (used[current]);

		out << "d" << current;
		used[current] = true;

		if (i != dc_count - 1)
			out << ",";
	}
}