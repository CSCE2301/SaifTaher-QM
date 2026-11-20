#include "input.h"
#include "Implicant.h"
#include "combine.h"
#include "matrix.h"
#include "generate.h"

static bool is_full_cover(const Chart& final_chart, boost::dynamic_bitset<> included)
{
	boost::dynamic_bitset<> covered(final_chart.cols.size());
	for (int i = included.find_first(); i != boost::dynamic_bitset<>::npos; i = included.find_next(i))
		covered |= final_chart.bitmap[i];

	return covered.count() == final_chart.cols.size();
}

static std::pair<std::string, std::string> binary_rep_2_bool_expr(const Implicant& imp, uint n_vars)
{
	std::string bin_expr, bool_expr;
	uint counter = 0;

	for (int i = 0; i < n_vars; i++)
	{
		if (imp.dash_mask >> i & 1)
		{
			bin_expr = "-" + bin_expr;
		}
		else
		{
			switch (imp.val_mask >> i & 1)
			{
			case 1:
				bin_expr = "1" + bin_expr;
				bool_expr = char(65 + (n_vars - 1) - i) + bool_expr;
				break;
			case 0:
				bin_expr = "0" + bin_expr;
				bool_expr = "'" + bool_expr;
				bool_expr = char(65 + (n_vars - 1) - i) + bool_expr;
				break;
			}
		}	
	}

	return { bin_expr, bool_expr };
}

void solve_final(const Chart& final_chart, std::vector<boost::dynamic_bitset<>>& all_solutions, boost::dynamic_bitset<>& included, uint& best, uint current_index, bool unique)
{
	if (included.count() > best)
		return;
	if (included.count() == best && unique)
		return;

	if (is_full_cover(final_chart, included))
	{
		if (included.count() < best)
		{
			best = included.count();
			all_solutions.clear();
		}
		else if (unique)
			return;

		all_solutions.push_back(included);
		return;
	}
	
	if (current_index < final_chart.rows.size())
	{
		included[current_index] = 1;
		solve_final(final_chart, all_solutions, included, best, current_index + 1, unique);

		included[current_index] = 0;
		solve_final(final_chart, all_solutions, included, best, current_index + 1, unique);
	}
}

int main()
{
	std::string filename = "data.txt";
	uint n_vars;

	//int n_vars_gen = 7;
	//n_vars = n_vars_gen;
	//gen_data(filename, n_vars_gen);
	
	bool is_minterms;

	std::vector<uint> terms_vector, dc_vector;
	fetch_data(n_vars, is_minterms, terms_vector, dc_vector, filename);
	
	std::vector<uint8_t> minterm_table = build_minterm_table(n_vars, is_minterms, terms_vector, dc_vector);

	std::vector<Implicant> PI_vec = PIs(minterm_table, n_vars);

	std::pair<std::string, std::string> exp_strs;
	std::cout << "Prime implicants:\n\n";
	for (const auto& x : PI_vec)
	{
		exp_strs = binary_rep_2_bool_expr(x, n_vars);
		std::cout << exp_strs.first << " || " << exp_strs.second << '\n';
	}

	std::cout << '\n';

	std::vector<Implicant> EPIs;
	Chart residual;
	segregate_essential(PI_vec, minterm_table, n_vars, EPIs, residual);

	std::cout << "Essential prime implicants:\n";

	std::vector<std::string> essential_bool_exprs;
	std::string current;

	for (const auto& x : EPIs)
	{
		current = binary_rep_2_bool_expr(x, n_vars).second;
		essential_bool_exprs.push_back(current);
		std::cout << current << '\n';
	}
		
	std::cout << '\n';

	std::cout << "Minterms not covered by essential prime implicants:\n";

	for (const auto& x : residual.cols)
		std::cout << x << '\n';
	std::cout << '\n';

	std::vector<Implicant> forced;
	bool unique_solution = true;

	reduce_chart(residual, forced, unique_solution);
	for (const auto& x : forced)
	{
		current = binary_rep_2_bool_expr(x, n_vars).second;
		essential_bool_exprs.push_back(current);
	}

	uint best = std::min(residual.rows.size(), residual.cols.size());
	std::vector<boost::dynamic_bitset<>> all_solutions;
	boost::dynamic_bitset<> included(residual.rows.size());
	solve_final(residual, all_solutions, included, best, 0, unique_solution);

	std::string essential_prefix, essential_prefix_nand;
	for (const auto& s : essential_bool_exprs)
	{
		essential_prefix += s + " + ";
		essential_prefix_nand += "(" + s + ")'" + " x ";
	}

	essential_prefix = essential_prefix.length() == 0 ? essential_prefix : essential_prefix.substr(0, essential_prefix.size() - 3);
	essential_prefix_nand = essential_prefix_nand.length() == 0 ? essential_prefix_nand : essential_prefix_nand.substr(0, essential_prefix_nand.size() - 3);

	std::vector<std::string> non_essential_bool_exprs;
	for (const auto& x : residual.rows)
	{
		current = binary_rep_2_bool_expr(x, n_vars).second;
		non_essential_bool_exprs.push_back(current);
	}

	std::cout << "Minimized boolean expression(s):\n\n";
	std::string current_nand;

	if (all_solutions.size() == 0)
	{
		std::cout << "Solution: " << essential_prefix;
		std::cout << "\nNAND version: " << essential_prefix_nand << "\n\n";
	}
	else
	{
		for (int i = 0; i < all_solutions.size(); i++)
		{
			std::cout << "Solution " << i + 1 << ": " << essential_prefix;
			current_nand = essential_prefix_nand;

			for (int j = 0; j < all_solutions[i].size(); j++)
			{
				if (all_solutions[i][j] == 1)
				{
					if (j != 0 || (j == 0 && essential_prefix.length() != 0))
					{
						current_nand += " x ";
						std::cout << " + ";
					}

					std::cout << non_essential_bool_exprs[j];
					current_nand += "(" + non_essential_bool_exprs[j] + ")'";
				}
			}

			std::cout << "\nNAND version: " << current_nand << "\n\n";
			current_nand = "";
		}
	}
}