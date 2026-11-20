#pragma once
#include <iostream>
#include <bitset>
#include <vector>
#include <map>
#include <boost/dynamic_bitset.hpp>
#include "Implicant.h"

typedef unsigned int uint;

struct Chart
{
    std::vector<Implicant> rows;
    std::vector<uint32_t> cols;
    std::vector<boost::dynamic_bitset<>> bitmap;
};

void segregate_essential(const std::vector<Implicant>& PIs, const std::vector<uint8_t>& table, uint n_vars, std::vector<Implicant>& EPIs, Chart& residual);
static std::vector<boost::dynamic_bitset<>> transpose(std::vector<boost::dynamic_bitset<>> original, uint n_cols);
static void compact(Chart& chart, const std::vector<bool>& keep_row, const std::vector<bool>& keep_col);
static bool domination_reduction(const std::vector<boost::dynamic_bitset<>>& bitmap, std::vector<bool>& alive, bool kill_dominated);
void reduce_chart(Chart& chart, std::vector<Implicant>& forced, bool drop_dominated_rows);