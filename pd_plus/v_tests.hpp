#ifndef B_FILTERS_V_TESTS_HPP
#define B_FILTERS_V_TESTS_HPP

#include "pd512_plus.hpp"


void pd_init(__m512i *pd);

auto insert_find_single(int64_t quot, uint8_t rem, const __m512i *pd) -> bool;

auto insert_find_all(const __m512i *pd) -> bool;

auto insert_find_all() -> bool;

auto rand_test1() -> bool;

auto rand_fill_and_test1(__m512i *pd) -> bool;

// auto rand_test_OF() -> bool;

auto test_swap_smaller(const __m512i *cpd) -> bool;

auto test_swap_same(const __m512i *cpd) -> bool;

auto test_swap_bigger(const __m512i *cpd) -> bool;

auto test_swap(size_t reps, const __m512i *cpd) -> bool;

auto test_swap_wrapper() -> bool;

#endif//B_FILTERS_V_TESTS_HPP
