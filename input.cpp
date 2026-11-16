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

std::vector<uint> complement_terms(uint n_vars, std::vector<uint>& terms, std::vector<uint>& dc)
{
	std::sort(terms.begin(), terms.end());
	uint curr_term = 0, curr_dc = 0, cap = 1 << n_vars;
	std::vector<uint> complemented; complemented.reserve(cap);

	for (uint i = 0; i < cap; i++)
	{
		if (curr_term >= terms.size() || i != terms[curr_term])
		{
			if (curr_dc >= dc.size() || i != dc[curr_dc])
				complemented.push_back(i);
			else
				curr_dc++;
		}
		else
			curr_term++;
	}

	std::vector<uint>(complemented).swap(complemented);
	return complemented;
}