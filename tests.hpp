#ifndef BENCH_TESTS_HPP
#define BENCH_TESTS_HPP

#include "utils.hpp"
// using namespace std
using ns = std::chrono::nanoseconds;


void set_pd(__m512i *pd, size_t quot_range, size_t capacity);

auto benchmark_find_v1(const __m512i *pd) -> ulong;
auto benchmark_find_v3(const __m512i *pd) -> ulong;
auto benchmark_find_v6(const __m512i *pd) -> ulong;
auto benchmark_find_v7(const __m512i *pd) -> ulong;
auto benchmark_find_v8(const __m512i *pd) -> ulong;
auto benchmark_find_v9(const __m512i *pd) -> ulong;
auto benchmark_find_v10(const __m512i *pd) -> ulong;
auto benchmark_find_v11(const __m512i *pd) -> ulong;
auto benchmark_find_v12(const __m512i *pd) -> ulong;
auto benchmark_find_v13(const __m512i *pd) -> ulong;
auto benchmark_find_v14(const __m512i *pd) -> ulong;
auto benchmark_find_v15(const __m512i *pd) -> ulong;
auto benchmark_find_v16(const __m512i *pd) -> ulong;
auto benchmark_find_v17(const __m512i *pd) -> ulong;
// auto benchmark_find_v1(const __m512i *pd) -> ulong;
// auto benchmark_find_v1(const __m512i *pd) -> ulong;
// auto benchmark_find_v1() -> ulong;
// auto benchmark_find_single(pd_find_func_type gen_find, const __m512i *pd) -> ulong;
// auto benchmark_find_single(FunctionFunc gen_find, const __m512i *pd) -> ulong;

auto benchmark_find_all() -> void;
auto benchmark_find_fast_functions() -> void;

// void

auto wrapper() -> ulong;


#endif// BENCH_TESTS_HPP
