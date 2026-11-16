#include "Implicant.h"

Implicant::Implicant() : binary_rep(""), is_prime(true), n_ones(0) {}
Implicant::Implicant(uint term, uint n_vars) : terms(std::set<uint>({term})), is_prime(true)
{
	for (int i = 0; i < n_vars; i++)
	{
		binary_rep = (term & 1) ? "1" + binary_rep : "0" + binary_rep;
		term = term >> 1;
	}

	std::string::difference_type ones_count_str = std::count(binary_rep.begin(), binary_rep.end(), '1');
	n_ones = static_cast<int>(ones_count_str);
}
Implicant::Implicant(Implicant& i1, Implicant& i2, int delta_index) : Implicant()
{
	if (i1.binary_rep.length() != i2.binary_rep.length() || delta_index < 0 || delta_index >= i1.binary_rep.length())
	{
		std::string error_msg = "Invalid implicant combination (" + i1.binary_rep + " and " + i2.binary_rep + ").";
		throw std::invalid_argument(error_msg);
	}

	i1.is_prime = i2.is_prime = false;

	for (const auto& x : i1.terms)
		terms.insert(x);
	for (const auto& x : i2.terms)
		terms.insert(x);

	binary_rep = i1.binary_rep;
	binary_rep[delta_index] = '-';

	n_ones = std::min(i1.n_ones, i2.n_ones);
}

Implicant::Implicant(const Implicant& i2)
{
	binary_rep = i2.binary_rep;
	n_ones = i2.n_ones;
	terms = i2.terms;
	is_prime = i2.is_prime;
}

int Implicant::delta_one(const Implicant& i2) const
{
	if (binary_rep.length() != i2.binary_rep.length())
		return -1;

	int delta_index = -1;

	for (int i = 0; i < binary_rep.length(); i++)
	{
		if (binary_rep[i] != i2.binary_rep[i])
		{
			if (delta_index == -1)
				delta_index = i;
			else
			{
				delta_index = -1;
				break;
			}
		}
	}

	return delta_index;
}

bool Implicant::operator<(const Implicant& i2) const
{
	return binary_rep < i2.binary_rep;
}