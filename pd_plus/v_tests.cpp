

#include "v_tests.hpp"

void pd_init(__m512i *pd) {
    *pd = __m512i{(INT64_C(1) << 50) - 1, 0, 0, 0, 0, 0, 0, 0};
}

auto insert_find_single(int64_t quot, uint8_t rem, const __m512i *pd) -> bool {
    assert(!pd512_plus::pd_full(pd));
    __m512i temp_pd = *pd;

    auto before_last_quot = pd512_plus::decode_last_quot_wrapper(pd);
    pd512_plus::pd_conditional_add_50(quot, rem, &temp_pd);
    auto lookup_res = pd512_plus::pd_find_50(quot, rem, &temp_pd);
    assert(lookup_res == pd512_plus::Yes);
    if (quot > before_last_quot) {
        auto new_last_quot = pd512_plus::decode_last_quot_wrapper(&temp_pd);
        assert(new_last_quot == quot);
    }
    return true;
}


auto insert_find_all(const __m512i *pd) -> bool {
    for (size_t q = 0; q < 50; q++) {
        for (size_t r = 0; r < 256; r++) {
            insert_find_single(q, r, pd);
        }
    }
    return true;
}


auto insert_find_all() -> bool {
    __m512i x;
    pd_init(&x);
    return insert_find_all(&x);
}

auto rand_test1() -> bool {
    size_t max_capacity = 51;
    uint64_t valid_max_quot = 0;
    __m512i x;
    pd_init(&x);
    for (size_t i = 0; i < max_capacity; i++) {
        uint64_t q = rand() % QUOT_SIZE;
        uint64_t r = rand() & 255;
        auto res = pd512_plus::pd_conditional_add_50(q, r, &x);
        assert(res == (1<<15));
        if (valid_max_quot < q)
            valid_max_quot = q;

        assert(pd512_plus::pd_find_50(q, r, &x) == pd512_plus::Yes);
        auto att_last_quot = pd512_plus::decode_last_quot_wrapper(&x);
        assert(att_last_quot == valid_max_quot);
    }
    return true;
}
