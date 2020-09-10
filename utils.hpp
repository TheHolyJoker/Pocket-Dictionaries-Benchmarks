#ifndef BENCH_UTILS_HPP
#define BENCH_UTILS_HPP

#include <functional>
#include <iostream>
#include <set>
#include <vector>

#include "pd320.hpp"
#include "pd512.hpp"
#include <chrono>

using ns = std::chrono::nanoseconds;
// typedef chrono::nanoseconds ns;

void set_pd(__m512i *pd, size_t quot_range, size_t capacity);


__attribute__((always_inline)) inline uint16_t reduce16(uint16_t hash, uint16_t n) {
    // http://lemire.me/blog/2016/06/27/a-fast-alternative-to-the-modulo-reduction/
    return (uint16_t)(((uint32_t) hash * n) >> 16);
}

#endif//BENCH_UTILS_HPP