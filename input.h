#pragma once

#include <fstream>
#include <vector>
#include <string>
#include <exception>
#include <iostream>
#include <sstream>
#include <utility>
#include <algorithm>

typedef unsigned int uint;

void fetch_data(uint& n_vars, bool& is_minterms, std::vector<uint>& terms_vector, std::vector<uint>& dc_vector, std::string filename);
std::vector<uint> complement_terms(uint n_vars, std::vector<uint>& terms, std::vector<uint>& dc);