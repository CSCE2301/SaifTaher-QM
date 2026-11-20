#include "input.h"

void fetch_data(uint& n_vars, bool& is_minterms, std::vector<uint>& terms_vector, std::vector<uint>& dc_vector, std::string filename)
{
	std::ifstream input_file;

	try
	{
		input_file.open(filename);
	}
	catch (const std::exception& e)
	{
		std::cout << "This file does not exist or cannot be opened.\n";
		return;
	}

	try
	{
		input_file >> n_vars;
		terms_vector.reserve(1 << n_vars);
		dc_vector.reserve(1 << n_vars);

		std::string terms, curr_term;

		input_file.ignore(1000, '\n');
		std::getline(input_file, terms);
		is_minterms = terms[0] == 'm';

		std::stringstream terms_sstream(terms);
		while (std::getline(terms_sstream, curr_term, ','))
		{
			terms_vector.push_back(std::stoi(curr_term.substr(1, curr_term.length() - 1)));
		}
		std::vector<uint>(terms_vector).swap(terms_vector);

		std::getline(input_file, terms);
		terms_sstream = std::stringstream(terms);
		while (std::getline(terms_sstream, curr_term, ','))
		{
			dc_vector.push_back(std::stoi(curr_term.substr(1, curr_term.length() - 1)));
		}
		std::vector<uint>(dc_vector).swap(dc_vector);
	}
	catch (const std::exception& e)
	{
		std::cout << "This file does not match the expected format. The program supports up to 20 variables only.\n";
	}
}

std::vector<uint8_t> build_minterm_table(uint n_vars, bool is_minterms, const std::vector<uint>& terms, const std::vector<uint>& dc)
{
	std::vector<uint8_t> table(1u << n_vars, is_minterms ? 0 : 1);

	for (uint t : terms)
		table[t] = is_minterms ? 1 : 0;
	for (uint d : dc)
		table[d] = 2;

	return table;
}