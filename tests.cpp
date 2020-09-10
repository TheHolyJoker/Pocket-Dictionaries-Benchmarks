#include "tests.hpp"


// auto benchmark_find_v1() -> ulong{
auto benchmark_find_v1(const __m512i *pd) -> ulong {
    const uint64_t quot_range = 50;
    // __m512i pd;
    // set_pd(pd, quot_range, 51);
    static volatile bool dummy;
    bool x = 0;
    // volatile uint64_t result = 0;
    auto t0 = std::chrono::high_resolution_clock::now();
    for (uint64_t i = 0; i < 1'000'000'000; ++i) {
        // Prevent overlapping execution by serializing on result
        x ^= pd512::pd_find_50_v1(reduce16(i, quot_range), i, pd);
    }
    dummy = x;
    auto t1 = std::chrono::high_resolution_clock::now();
    // std::cout << dummy << "\t";
    return std::chrono::duration_cast<ns>(t1 - t0).count();
}

auto benchmark_find_v3(const __m512i *pd) -> ulong {
    const uint64_t quot_range = 50;
    // __m512i pd;
    // set_pd(pd, quot_range, 51);
    static volatile bool dummy;
    bool x = 0;
    // volatile uint64_t result = 0;
    auto t0 = std::chrono::high_resolution_clock::now();
    for (uint64_t i = 0; i < 1'000'000'000; ++i) {
        // Prevent overlapping execution by serializing on result
        x ^= pd512::pd_find_50_v3(reduce16(i, quot_range), i, pd);
    }
    dummy = x;
    auto t1 = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<ns>(t1 - t0).count();
}

auto benchmark_find_v6(const __m512i *pd) -> ulong {
    const uint64_t quot_range = 50;
    // __m512i pd;
    // set_pd(pd, quot_range, 51);
    static volatile bool dummy;
    bool x = 0;
    // volatile uint64_t result = 0;
    auto t0 = std::chrono::high_resolution_clock::now();
    for (uint64_t i = 0; i < 1'000'000'000; ++i) {
        // Prevent overlapping execution by serializing on result
        x ^= pd512::pd_find_50_v6(reduce16(i, quot_range), i, pd);
    }
    dummy = x;
    auto t1 = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<ns>(t1 - t0).count();
}

auto benchmark_find_v7(const __m512i *pd) -> ulong {
    const uint64_t quot_range = 50;
    // __m512i pd;
    // set_pd(pd, quot_range, 51);
    static volatile bool dummy;
    bool x = 0;
    // volatile uint64_t result = 0;
    auto t0 = std::chrono::high_resolution_clock::now();
    for (uint64_t i = 0; i < 1'000'000'000; ++i) {
        // Prevent overlapping execution by serializing on result
        x ^= pd512::pd_find_50_v7(reduce16(i, quot_range), i, pd);
    }
    dummy = x;
    auto t1 = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<ns>(t1 - t0).count();
}

auto benchmark_find_v8(const __m512i *pd) -> ulong {
    const uint64_t quot_range = 50;
    // __m512i pd;
    // set_pd(pd, quot_range, 51);
    static volatile bool dummy;
    bool x = 0;
    // volatile uint64_t result = 0;
    auto t0 = std::chrono::high_resolution_clock::now();
    for (uint64_t i = 0; i < 1'000'000'000; ++i) {
        // Prevent overlapping execution by serializing on result
        x ^= pd512::pd_find_50_v8(reduce16(i, quot_range), i, pd);
    }
    dummy = x;
    auto t1 = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<ns>(t1 - t0).count();
}

auto benchmark_find_v9(const __m512i *pd) -> ulong {
    const uint64_t quot_range = 50;
    // __m512i pd;
    // set_pd(pd, quot_range, 51);
    static volatile bool dummy;
    bool x = 0;
    // volatile uint64_t result = 0;
    auto t0 = std::chrono::high_resolution_clock::now();
    for (uint64_t i = 0; i < 1'000'000'000; ++i) {
        // Prevent overlapping execution by serializing on result
        x ^= pd512::pd_find_50_v9(reduce16(i, quot_range), i, pd);
    }
    dummy = x;
    auto t1 = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<ns>(t1 - t0).count();
}

auto benchmark_find_v10(const __m512i *pd) -> ulong {
    const uint64_t quot_range = 50;
    // __m512i pd;
    // set_pd(pd, quot_range, 51);
    static volatile bool dummy;
    bool x = 0;
    // volatile uint64_t result = 0;
    auto t0 = std::chrono::high_resolution_clock::now();
    for (uint64_t i = 0; i < 1'000'000'000; ++i) {
        // Prevent overlapping execution by serializing on result
        x ^= pd512::pd_find_50_v10(i % quot_range, i, pd);
    }
    dummy = x;
    auto t1 = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<ns>(t1 - t0).count();
}

auto benchmark_find_v11(const __m512i *pd) -> ulong {
    const uint64_t quot_range = 50;
    // __m512i pd;
    // set_pd(pd, quot_range, 51);
    static volatile bool dummy;
    bool x = 0;
    // volatile uint64_t result = 0;
    auto t0 = std::chrono::high_resolution_clock::now();
    for (uint64_t i = 0; i < 1'000'000'000; ++i) {
        // Prevent overlapping execution by serializing on result
        x ^= pd512::pd_find_50_v11(reduce16(i, quot_range), i, pd);
    }
    dummy = x;
    auto t1 = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<ns>(t1 - t0).count();
}

auto benchmark_find_v12(const __m512i *pd) -> ulong {
    const uint64_t quot_range = 50;
    // __m512i pd;
    // set_pd(pd, quot_range, 51);
    static volatile bool dummy;
    bool x = 0;
    // volatile uint64_t result = 0;
    auto t0 = std::chrono::high_resolution_clock::now();
    for (uint64_t i = 0; i < 1'000'000'000; ++i) {
        // Prevent overlapping execution by serializing on result
        x ^= pd512::pd_find_50_v12(reduce16(i, quot_range), i, pd);
    }
    dummy = x;
    auto t1 = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<ns>(t1 - t0).count();
}

auto benchmark_find_v13(const __m512i *pd) -> ulong {
    const uint64_t quot_range = 50;
    // __m512i pd;
    // set_pd(pd, quot_range, 51);
    static volatile bool dummy;
    bool x = 0;
    // volatile uint64_t result = 0;
    auto t0 = std::chrono::high_resolution_clock::now();
    for (uint64_t i = 0; i < 1'000'000'000; ++i) {
        // Prevent overlapping execution by serializing on result
        x ^= pd512::pd_find_50_v13(i % quot_range, i, pd);
    }
    dummy = x;
    auto t1 = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<ns>(t1 - t0).count();
}

auto benchmark_find_v14(const __m512i *pd) -> ulong {
    const uint64_t quot_range = 50;
    // __m512i pd;
    // set_pd(pd, quot_range, 51);
    static volatile bool dummy;
    bool x = 0;
    // volatile uint64_t result = 0;
    auto t0 = std::chrono::high_resolution_clock::now();
    for (uint64_t i = 0; i < 1'000'000'000; ++i) {
        // Prevent overlapping execution by serializing on result
        x ^= pd512::pd_find_50_v14(reduce16(i, quot_range), i, pd);
    }
    dummy = x;
    auto t1 = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<ns>(t1 - t0).count();
}

auto benchmark_find_v15(const __m512i *pd) -> ulong {
    const uint64_t quot_range = 50;
    // __m512i pd;
    // set_pd(pd, quot_range, 51);
    static volatile bool dummy;
    bool x = 0;
    // volatile uint64_t result = 0;
    auto t0 = std::chrono::high_resolution_clock::now();
    for (uint64_t i = 0; i < 1'000'000'000; ++i) {
        // Prevent overlapping execution by serializing on result
        x ^= pd512::pd_find_50_v15(reduce16(i, quot_range), i, pd);
    }
    dummy = x;
    auto t1 = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<ns>(t1 - t0).count();
}

auto benchmark_find_v16(const __m512i *pd) -> ulong {
    const uint64_t quot_range = 50;
    // __m512i pd;
    // set_pd(pd, quot_range, 51);
    static volatile bool dummy;
    bool x = 0;
    // volatile uint64_t result = 0;
    auto t0 = std::chrono::high_resolution_clock::now();
    for (uint64_t i = 0; i < 1'000'000'000; ++i) {
        // Prevent overlapping execution by serializing on result
        x ^= pd512::pd_find_50_v16(reduce16(i, quot_range), i, pd);
    }
    dummy = x;
    auto t1 = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<ns>(t1 - t0).count();
}

auto benchmark_find_v17(const __m512i *pd) -> ulong {
    const uint64_t quot_range = 50;
    const uint64_t pop = _mm_popcnt_u64(((uint64_t *) pd)[0]);
    // __m512i pd;
    // set_pd(pd, quot_range, 51);
    static volatile bool dummy;
    bool x = 0;

    // volatile uint64_t result = 0;
    auto t0 = std::chrono::high_resolution_clock::now();
    for (uint64_t i = 0; i < 1'000'000'000; ++i) {
        // Prevent overlapping execution by serializing on result
        x ^= pd512::pd_find_50_v17(reduce16(i, quot_range), i, pd, pop);
    }
    dummy = x;
    auto t1 = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<ns>(t1 - t0).count();
}

auto benchmark_find_v18(const __m512i *pd) -> ulong {
    const uint64_t quot_range = 50;
    // __m512i pd;
    // set_pd(pd, quot_range, 51);
    static volatile bool dummy;
    bool x = 0;
    // volatile uint64_t result = 0;
    auto t0 = std::chrono::high_resolution_clock::now();
    for (uint64_t i = 0; i < 1'000'000'000; ++i) {
        // Prevent overlapping execution by serializing on result
        x ^= pd512::pd_find_50_v18(reduce16(i, quot_range), i, pd);
    }
    dummy = x;
    auto t1 = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<ns>(t1 - t0).count();
}

// void benchmark_find() {
//     const uint64_t quot_range = 50;
//     __m512i x;
//     set_pd(&x, quot_range, 51);
//     uint64_t result = 0;
//     for (uint64_t i = 0; i < 1'000'000'000; ++i) {
//         // Prevent overlapping execution by serializing on result
//         result += pd512::pd_find_50(i % quot_range, (i >> 6) + result, &x);
//     }
//     std::cout << result << std::endl;
// }

/* Taken from https://github.com/jbapple/crate-dictionary/blob/master/bench-pd.cpp */
// auto benchmark_find_single(std::function<bool(int64_t, uint8_t, const __m512i *)> gen_find, const __m512i *pd) -> ulong {
// auto benchmark_find_single(pd_find_func_type gen_find, const __m512i *pd) -> ulong {
// auto benchmark_find_single(FunctionFunc gen_find, const __m512i *pd) -> ulong {
//     // __m512i x;
//     // set_pd(&x, quot_range, 51);
//     const uint64_t quot_range = 50;
//     volatile uint64_t result = 0;
//     auto t0 = std::chrono::high_resolution_clock::now();
//     for (uint64_t i = 0; i < 1'000'000'000; ++i) {
//         // Prevent overlapping execution by serializing on result
//         result += gen_find(i % quot_range, (i >> 6) + result, pd);
//     }
//     auto t1 = std::chrono::high_resolution_clock::now();
//     return std::chrono::duration_cast<ns>(t1 - t0).count();
// }


auto benchmark_find_all() -> void {
    const uint64_t quot_range = 50;
    __m512i x;
    set_pd(&x, quot_range, 51);
    while (true) {
        std::cout << "v1:\t" << benchmark_find_v1(&x) << std::endl;
        // std::cout << "v2:\t" << benchmark_find_v2(&x) << std::endl;
        std::cout << "v3:\t" << benchmark_find_v3(&x) << std::endl;
        // std::cout << "v4:\t" << benchmark_find_v4(&x) << std::endl;
        // std::cout << "v5:\t" << benchmark_find_v5(&x) << std::endl;
        std::cout << "v6:\t" << benchmark_find_v6(&x) << std::endl;
        // std::cout << "v7:\t" << benchmark_find_v7(&x) << std::endl;
        std::cout << "v8:\t" << benchmark_find_v8(&x) << std::endl;
        std::cout << "v9:\t" << benchmark_find_v9(&x) << std::endl;
        std::cout << "v10:\t" << benchmark_find_v10(&x) << std::endl;
        std::cout << "v11:\t" << benchmark_find_v11(&x) << std::endl;
        std::cout << "v12:\t" << benchmark_find_v12(&x) << std::endl;
        std::cout << "v13:\t" << benchmark_find_v13(&x) << std::endl;
        std::cout << "v14:\t" << benchmark_find_v14(&x) << std::endl;
        std::cout << "v15:\t" << benchmark_find_v15(&x) << std::endl;
        std::cout << "v16:\t" << benchmark_find_v16(&x) << std::endl;
        std::cout << "v17:\t" << benchmark_find_v17(&x) << std::endl;
        std::cout << std::string(80, '*') << std::endl;
    }
    /* while (true) {
        // std::cout << "v1:\t" << benchmark_find_single(pd512::pd_find_50_v1, &x) << std::endl;
        // std::cout << "v13:\t" << benchmark_find_single(pd512::pd_find_50_v13, &x) << std::endl;
        // std::cout << "v12:\t" << benchmark_find_single(pd512::pd_find_50_v12, &x) << std::endl;
        std::cout << "v10:\t" << benchmark_find_single(pd512::pd_find_50_v10, &x) << std::endl;
        // std::cout << "v9:\t" << benchmark_find_single(pd512::pd_find_50_v9, &x) << std::endl;
        // std::cout << "v8:\t" << benchmark_find_single(pd512::pd_find_50_v8, &x) << std::endl;
        // std::cout << "v6:\t" << benchmark_find_single(pd512::pd_find_50_v6, &x) << std::endl;
        std::cout << "v3:\t" << benchmark_find_single(pd512::pd_find_50_v3, &x) << std::endl;
        // std::cout << "v10:\t" << benchmark_find_single(pd512::pd_find_50_v10, &x) << std::endl;
        // std::cout << "v11:\t" << benchmark_find_single(pd512::pd_find_50_v11, &x) << std::endl;
        std::cout << std::string(80, '*') << std::endl;
    } */
}

auto benchmark_find_fast_functions() -> void {
    const uint64_t quot_range = 50;
    __m512i x;
    set_pd(&x, quot_range, 51);
    while (true) {
        // std::cout << "v1:\t" << benchmark_find_v1(&x) << std::endl;
        // std::cout << "v2:\t" << benchmark_find_v2(&x) << std::endl;
        // std::cout << "v3:\t" << benchmark_find_v3(&x) << std::endl;
        // std::cout << "v4:\t" << benchmark_find_v4(&x) << std::endl;
        // std::cout << "v5:\t" << benchmark_find_v5(&x) << std::endl;
        // std::cout << "v6:\t" << benchmark_find_v6(&x) << std::endl;
        // std::cout << "v7:\t" << benchmark_find_v7(&x) << std::endl;
        std::cout << "v8:\t" << benchmark_find_v8(&x) << std::endl;
        std::cout << "v9:\t" << benchmark_find_v9(&x) << std::endl;
        // std::cout << "v10:\t" << benchmark_find_v10(&x) << std::endl;
        std::cout << "v11:\t" << benchmark_find_v11(&x) << std::endl;
        // std::cout << "v12:\t" << benchmark_find_v12(&x) << std::endl;
        // std::cout << "v13:\t" << benchmark_find_v13(&x) << std::endl;
        std::cout << "v14:\t" << benchmark_find_v14(&x) << std::endl;
        std::cout << "v15:\t" << benchmark_find_v15(&x) << std::endl;
        std::cout << "v16:\t" << benchmark_find_v16(&x) << std::endl;
        std::cout << "v17:\t" << benchmark_find_v17(&x) << std::endl;
        std::cout << "v18:\t" << benchmark_find_v18(&x) << std::endl;

        std::cout << std::string(80, '*') << std::endl;
    }
}

auto wrapper() -> ulong {
    const uint64_t quot_range = 50;
    __m512i x;
    set_pd(&x, quot_range, 51);
    volatile uint64_t result = 0;
    auto t0 = std::chrono::high_resolution_clock::now();
    for (uint64_t i = 0; i < 1'000'000'000; ++i) {
        // Prevent overlapping execution by serializing on result
        result += pd512::pd_find_50(i % quot_range, (i >> 6) + result, &x);
    }
    auto t1 = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<ns>(t1 - t0).count();
    // std::cout << result << std::endl;
}
/* using namespace std;
static void test_select()
{
    assert(pd512::select64(0, 0) == 64);
    assert(select64(1, 0) == 0);
    assert(select64(1 << 10, 0) == 10);
    assert(select64(UINT64_C(1) << 63, 0) == 63);
    assert(select64((UINT64_C(1) << 63) | 1, 0) == 0);
    assert(select64((UINT64_C(1) << 63) | 1, 1) == 63);
}

static void test_pd_find_64()
{
    __m512i x = {~INT64_C(0), 0, 0, 0, 0, 0, 0, 0};
    assert(false == pd_find_64(0, 0, &x));
    assert(false == pd_find_64(1, 0, &x));
    assert(false == pd_find_64(0, 1, &x));
    x[0] = ~INT64_C(1);
    x[1] = 1;
    assert(true == pd_find_64(0, 0, &x));
    assert(false == pd_find_64(1, 0, &x));
    assert(false == pd_find_64(0, 1, &x));
    x[0] = ~INT64_C(2);
    assert(false == pd_find_64(0, 0, &x));
    assert(true == pd_find_64(1, 0, &x));
    assert(false == pd_find_64(0, 1, &x));
    x[0] = ~INT64_C(1);
    x[1] = 1 | (INT64_C(1) << 56);
    assert(false == pd_find_64(0, 0, &x));
    assert(false == pd_find_64(1, 0, &x));
    assert(true == pd_find_64(0, 1, &x));
    x[1] = static_cast<int64_t>(1 | (UINT64_C(0xAB) << 56));
    assert(true == pd_find_64(0, static_cast<char>(0xAB), &x));
}

int main()
{
    __m512i x = {INT64_C(0x5555'5555'5555'5555), ((INT64_C(1) << 32) - 1),
                 0x0123'4567'89ab'cdef, 0x0123'4567'89ab'cdef,
                 0x0123'4567'89ab'cdef, 0x0123'4567'89ab'cdef,
                 0x0123'4567'89ab'cdef, 0x0123'4567'89ab'cdef};

    uint64_t result = 0;
    for (uint64_t i = 0; i < 1'000'000'000; ++i)
    {
        // Prevent overlapping execution by serializing on result
        result += pd512::pd_find_64(i & 63, (i >> 6) + result, &x);
    }
    cout << result << endl;
}
 */
