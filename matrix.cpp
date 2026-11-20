#include "matrix.h"

void segregate_essential(const std::vector<Implicant>& PIs, const std::vector<uint8_t>& table, uint n_vars, std::vector<Implicant>& EPIs, Chart& residual)
{
    const uint num_minterms = 1U << n_vars;
    std::vector<uint8_t> count(num_minterms, 0);
    std::vector<int> only(num_minterms, -1);

    for (int i = 0; i < PIs.size(); i++)
        for_each_minterm(PIs[i], [&](uint32_t minterm) {
        if (table[minterm] == 1)
        {
            if (count[minterm] < 2)
                count[minterm]++;
            only[minterm] = i;
        }
    });

    std::vector<bool> is_EPI(PIs.size(), 0);
    for (int m = 0; m < num_minterms; m++)
        if (table[m] == 1 && count[m] == 1)
            is_EPI[only[m]] = 1;

    std::vector<uint8_t> covered_by_EPI(num_minterms, 0);
    for (int i = 0; i < PIs.size(); i++)
        if (is_EPI[i]) 
        {
            EPIs.push_back(PIs[i]);
            for_each_minterm(PIs[i], [&](uint32_t minterm) { covered_by_EPI[minterm] = 1; });
        }

    std::vector<int> col_of(num_minterms, -1);
    for (int m = 0; m < num_minterms; m++)
        if (table[m] == 1 && !covered_by_EPI[m])
        {
            col_of[m] = residual.cols.size();
            residual.cols.push_back(m);
        }

    for (int i = 0; i < PIs.size(); i++) {
        if (is_EPI[i])
            continue;

        boost::dynamic_bitset<> curr_row(residual.cols.size());
        for_each_minterm(PIs[i], [&](uint32_t m) { if (col_of[m] >= 0) curr_row[col_of[m]] = 1; });
        if (curr_row.any()) {
            residual.rows.push_back(PIs[i]);
            residual.bitmap.push_back(std::move(curr_row));
        }
    }
}

static std::vector<boost::dynamic_bitset<>> transpose(std::vector<boost::dynamic_bitset<>> original, uint n_cols)
{
    std::vector<boost::dynamic_bitset<>> transposed(n_cols, boost::dynamic_bitset<>(original.size()));
    for (int i = 0; i < original.size(); i++)
        for (int j = original[i].find_first(); j != boost::dynamic_bitset<>::npos; j = original[i].find_next(j))
            if (original[i][j])
                transposed[j].set(i);
    return transposed;
}

static void compact(Chart& chart, const std::vector<bool>& keep_row, const std::vector<bool>& keep_col)
{
    std::vector<int> new_col_indices(chart.cols.size(), -1);
    std::vector<uint32_t> new_cols;

    for (int i = 0; i < chart.cols.size(); i++)
        if (keep_col[i])
        {
            new_col_indices[i] = new_cols.size();
            new_cols.push_back(chart.cols[i]);
        }

    std::vector<Implicant> new_rows;
    std::vector<boost::dynamic_bitset<>> new_bitmap;

    for (int i = 0; i < chart.rows.size(); i++)
    {
        if (!keep_row[i])
            continue;

        boost::dynamic_bitset<> curr_row(new_cols.size());

        for (int j = chart.bitmap[i].find_first(); j != boost::dynamic_bitset<>::npos; j = chart.bitmap[i].find_next(j))
            if (new_col_indices[j] >= 0)
                curr_row.set(new_col_indices[j]);

        if (curr_row.none())
            continue;

        new_rows.push_back(chart.rows[i]);
        new_bitmap.push_back(std::move(curr_row));
    }

    chart.rows.swap(new_rows);
    chart.cols.swap(new_cols);
    chart.bitmap.swap(new_bitmap);
}

static bool domination_reduction(const std::vector<boost::dynamic_bitset<>>& bitmap, std::vector<bool>& alive, bool kill_dominated)
{
    std::vector<int> count(bitmap.size());

    for (int i = 0; i < bitmap.size(); i++)
        count[i] = bitmap[i].count();

    bool any = false;
    for (int i = 0; i < bitmap.size(); i++)
        for (int j = 0; j < bitmap.size() && alive[i]; j++)
        {
            if (i == j || !alive[j])
                continue;
            if (kill_dominated ? count[i] > count[j] : count[j] > count[i])
                continue;
            if (count[i] == count[j] && i < j)
                continue;

            bool dominated = kill_dominated ? bitmap[i].is_subset_of(bitmap[j]) : bitmap[j].is_subset_of(bitmap[i]);
            if (dominated)
            {
                alive[i] = 0;
                any = true;
            }
        }
    return any;
}

void reduce_chart(Chart& chart, std::vector<Implicant>& forced, bool drop_dominated_rows)
{
    bool changed = true;
    while (changed && !chart.rows.empty() && !chart.cols.empty())
    {
        changed = false;
        std::vector<boost::dynamic_bitset<>> cols_trans = transpose(chart.bitmap, chart.cols.size());
        std::vector<bool> keep_row(chart.rows.size(), 1), keep_col(chart.cols.size(), 1);

        for (int i = 0; i < cols_trans.size(); i++)
            if (cols_trans[i].count() == 1)
            {
                int j = cols_trans[i].find_first();
                if (keep_row[j])
                {
                    keep_row[j] = 0;
                    forced.push_back(chart.rows[j]);
                    for (int k = chart.bitmap[j].find_first(); k != boost::dynamic_bitset<>::npos; k = chart.bitmap[j].find_next(k))
                        keep_col[k] = 0;
                    changed = true;
                }
            }

        if (changed)
        { 
            compact(chart, keep_row, keep_col);
            continue;
        }

        if (domination_reduction(cols_trans, keep_col, false))
        {
            compact(chart, keep_row, keep_col);
            changed = true;
            continue;
        }

        if (drop_dominated_rows)
            if (domination_reduction(chart.bitmap, keep_row, true))
            {
                compact(chart, keep_row, keep_col);
                changed = true;
            }
    }
}

