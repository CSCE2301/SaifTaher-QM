#pragma once

#include <vector>
#include <set>
#include "Implicant.h"

typedef unsigned int uint;

std::vector<std::set<Implicant*, ImplicantPtrCompare>> combine(std::vector<std::set<Implicant*, ImplicantPtrCompare>> prev_iteration, bool& changed, uint n_ones_max);
std::set<Implicant> PIs(std::vector<std::set<Implicant*, ImplicantPtrCompare>> first_iteration, uint n_vars);