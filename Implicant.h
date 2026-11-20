#pragma once

#include <set>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <bitset>

struct Implicant
{
    uint32_t val_mask;
    uint32_t dash_mask;
    bool care;

    bool operator<(const Implicant& i2) const { return dash_mask != i2.dash_mask ? dash_mask < i2.dash_mask : val_mask < i2.val_mask; }
    bool operator==(const Implicant& i2) const { return val_mask == i2.val_mask && dash_mask == i2.dash_mask; }
    bool covers(uint32_t m) const { return (m & ~dash_mask) == val_mask; }
};

template <class F>
void for_each_minterm(const Implicant& imp, F fnxn) {
    uint32_t submask = 0;

    do {
        fnxn(imp.val_mask | submask); submask = (submask - imp.dash_mask) & imp.dash_mask;
    } while (submask);
}