#pragma once

#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <utility>
#include <algorithm>

typedef unsigned int uint;

void fetch_data(uint& n_vars, bool& is_minterms, std::vector<uint>& terms_vector, std::vector<uint>& dc_vector, std::string filename);
std::vector<uint8_t> build_minterm_table(uint n_vars, bool is_minterms, const std::vector<uint>& terms, const std::vector<uint>& dc);