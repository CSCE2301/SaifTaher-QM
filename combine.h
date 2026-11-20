#pragma once

#include <vector>
#include "Implicant.h"

typedef unsigned int uint;

std::vector<Implicant> PIs(const std::vector<uint8_t>& table, uint n_vars);