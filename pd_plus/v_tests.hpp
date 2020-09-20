#ifndef B_FILTERS_V_TESTS_HPP
#define B_FILTERS_V_TESTS_HPP

#include "pd512_plus.hpp"


void pd_init(__m512i *pd);

auto insert_find_single(int64_t quot, uint8_t rem, const __m512i *pd) -> bool;

auto insert_find_all(const __m512i *pd) -> bool;

auto insert_find_all() -> bool;

auto rand_test1() ->bool;

// auto v_last_quot()

#endif//B_FILTERS_V_TESTS_HPP
