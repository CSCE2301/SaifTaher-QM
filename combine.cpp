#include "combine.h"

std::vector<Implicant> PIs(const std::vector<uint8_t>& table, uint n_vars) {
    const uint32_t max_minterm = (1u << n_vars) - 1;
    std::vector<Implicant> current, next, primes;

    for (uint32_t m = 0; m <= max_minterm; m++)
        if (table[m]) // accepts if 1 or 2, corresponding to minterm or dc
            current.push_back({ m, 0, table[m] == 1 });

    while (!current.empty()) {
        std::vector<bool> combined(current.size(), 0);

        next.clear();
        for (size_t i = 0; i < current.size(); i++) {
            const Implicant& i1 = current[i];

            for (uint32_t f = ~(i1.val_mask | i1.dash_mask) & max_minterm; f; f &= f - 1) {
                uint32_t candidate_mask = f & (~f + 1);
                Implicant probe{ i1.val_mask | candidate_mask, i1.dash_mask, false };

                auto it = std::lower_bound(current.begin() + i + 1, current.end(), probe);
                if (it != current.end() && *it == probe) {
                    combined[i] = combined[it - current.begin()] = 1;
                    if (candidate_mask > i1.dash_mask) // to prevent duplicates
                        next.push_back({ i1.val_mask, i1.dash_mask | candidate_mask, i1.care || it->care });
                }
            }
        }

        for (size_t i = 0; i < current.size(); i++)
            if (!combined[i] && current[i].care) // Don't add implicants which only contain dc terms
                primes.push_back(current[i]);

        std::sort(next.begin(), next.end());
        current.swap(next);
    }
    return primes;
}