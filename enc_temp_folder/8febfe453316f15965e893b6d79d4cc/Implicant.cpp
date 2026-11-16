#include "Implicant.h"

Implicant::Implicant() : binary_rep(""), is_prime(false) {}
Implicant::Implicant(uint term, uint n_vars) : terms(std::set<uint>({term})), is_prime(false)
{
	for (int i = 0; i < n_vars; i++)
	{
		binary_rep = (term & 1) ? "1" + binary_rep : "0" + binary_rep;
		term = term >> 1;
	}
}
Implicant::Implicant(const Implicant& i1, const Implicant& i2, int delta_index) : Implicant()
{
	if (i1.binary_rep.length() != i2.binary_rep.length() || delta_index < 0 || delta_index >= i1.binary_rep.length())
	{
		std::string error_msg = "Invalid implicant combination (" + i1.binary_rep + " and " + i2.binary_rep + ").";
		throw std::invalid_argument(error_msg);
	}

	for (const auto& x : i1.terms)
		terms.insert(x);
	for (const auto& x : i2.terms)
		terms.insert(x);

	binary_rep = i1.binary_rep;
	binary_rep[delta_index] = '-';
}

int Implicant::delta_one(const Implicant& i2)
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