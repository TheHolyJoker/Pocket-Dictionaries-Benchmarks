

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
        assert(res == (1 << 15));
        if (valid_max_quot < q)
            valid_max_quot = q;

        assert(pd512_plus::pd_find_50(q, r, &x) == pd512_plus::Yes);
        auto att_last_quot = pd512_plus::decode_last_quot_wrapper(&x);
        assert(att_last_quot == valid_max_quot);
    }
    return true;
}


auto rand_fill_and_test1(__m512i *pd) -> bool {
    size_t max_capacity = 51;
    uint64_t valid_max_quot = pd512_plus::decode_last_quot_wrapper(pd);
    auto h0 = ((uint64_t *) pd)[0];
    if (!h0)
        pd_init(pd);

    for (size_t i = 0; i < max_capacity; i++) {
        uint64_t q = rand() % QUOT_SIZE;
        uint64_t r = rand() & 255;
        auto res = pd512_plus::pd_conditional_add_50(q, r, pd);
        assert(res == (1 << 15));
        if (valid_max_quot < q)
            valid_max_quot = q;

        assert(pd512_plus::pd_find_50(q, r, pd) == pd512_plus::Yes);
        auto att_last_quot = pd512_plus::decode_last_quot_wrapper(pd);
        assert(att_last_quot == valid_max_quot);
    }
    return true;
}


auto test_swap_smaller(const __m512i *cpd) -> bool {
    __m512i x = *cpd;
    __m512i *pd = &x;
    assert(pd512_plus::pd_full(pd));
    uint64_t last_quot = pd512_plus::decode_last_quot_wrapper(pd);
    uint64_t last_r = pd512_plus::get_last_byte(pd);
    uint64_t old_qr = (last_quot << 8) | ((uint64_t) last_r);
    // std::cout << "last_quot: " << last_quot << std::endl;
    // std::cout << "last_r: " << last_r << std::endl;
    // std::cout << "old_qr: " << old_qr << std::endl;
    if ((last_quot == 0) || (last_r == 0))
        return true;
    uint64_t temp_q = rand() % last_quot;
    uint64_t temp_r = rand() & 255;
    uint64_t new_qr = (temp_q << 8) | ((uint64_t) temp_r);
    // if las
    auto res = pd512_plus::pd_conditional_add_50(temp_q, temp_r, pd);
    assert(new_qr < old_qr);
    assert(res == old_qr);

    x = *cpd;
    pd = &x;

    temp_q = last_quot;
    assert(last_r);
    temp_r = rand() % last_r;
    res = pd512_plus::pd_conditional_add_50(temp_q, temp_r, pd);
    assert(res == old_qr);
    return true;
}

auto test_swap_same(const __m512i *cpd) -> bool {
    __m512i x = *cpd;
    __m512i *pd = &x;
    assert(pd512_plus::pd_full(pd));
    uint64_t last_quot = pd512_plus::decode_last_quot_wrapper(pd);
    uint64_t last_r = pd512_plus::get_last_byte(pd);
    uint64_t old_qr = (last_quot << 8) | ((uint64_t) last_r);

    auto res = pd512_plus::pd_conditional_add_50(last_quot, last_r, pd);
    assert(res == old_qr);
    return true;
}

auto test_swap_bigger(const __m512i *cpd) -> bool {
    __m512i x = *cpd;
    __m512i *pd = &x;
    assert(pd512_plus::pd_full(pd));
    uint64_t last_quot = pd512_plus::decode_last_quot_wrapper(pd);
    uint64_t last_r = pd512_plus::get_last_byte(pd);
    uint64_t old_qr = (last_quot << 8) | ((uint64_t) last_r);

    uint64_t diff = 49 - last_quot;
    uint64_t temp_q = last_quot;
    uint64_t temp_r = rand() & 255;
    uint64_t new_qr;
    if (diff) {
        temp_q += (rand() % diff);
        new_qr = (temp_q << 8) | ((uint64_t) temp_r);
        if (old_qr <= new_qr) {
            auto res = pd512_plus::pd_conditional_add_50(temp_q, temp_r, pd);
            assert(res == new_qr);
            x = *cpd;
            pd = &x;
        }
    }

    diff = 255 - last_r;
    temp_q = last_quot;
    temp_r = last_r;
    if (diff) {
        temp_r += (rand() % diff);
        new_qr = (temp_q << 8) | ((uint64_t) temp_r);
        if (old_qr <= new_qr) {
            auto res = pd512_plus::pd_conditional_add_50(temp_q, temp_r, pd);
            assert(res == new_qr);
        }
    }

    return true;
}


auto test_swap(size_t reps, const __m512i *cpd) -> bool {
    for (size_t i = 0; i < reps; i++) {
        test_swap_bigger(cpd);
        test_swap_smaller(cpd);
        test_swap_same(cpd);
    }
    return true;
}


auto test_swap_wrapper() -> bool {
    for (size_t i = 0; i < 1024; i++) {
        std::cout << "i: " << i << std::endl;
        __m512i x;
        pd_init(&x);
        rand_fill_and_test1(&x);
        test_swap(256, &x);
    }
    return true;
}

auto rand_test_OF() -> bool {
    size_t max_capacity = 51;
    uint64_t valid_max_quot = 0;
    __m512i x;
    pd_init(&x);
    for (size_t i = 0; i < max_capacity; i++) {
        uint64_t q = rand() % QUOT_SIZE;
        uint64_t r = rand() & 255;
        auto res = pd512_plus::pd_conditional_add_50(q, r, &x);
        assert(res == (1 << 15));
        if (valid_max_quot < q)
            valid_max_quot = q;

        assert(pd512_plus::pd_find_50(q, r, &x) == pd512_plus::Yes);
        auto att_last_quot = pd512_plus::decode_last_quot_wrapper(&x);
        assert(att_last_quot == valid_max_quot);
    }
    return true;
}
