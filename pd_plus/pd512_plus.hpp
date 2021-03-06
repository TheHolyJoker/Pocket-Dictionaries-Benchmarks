/*
 * Taken from this repository.
 * https://github.com/jbapple/crate-dictionary
 * */

#ifndef FILTERS_PD512_PLUS_HPP
#define FILTERS_PD512_PLUS_HPP

#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>

#include <iostream>

#include <immintrin.h>

#define QUOT_SIZE (50)

namespace v_pd512_plus {

    auto bin_print_header_spaced(uint64_t header) -> std::string;

    auto bin_print_header_spaced2(uint64_t header) -> std::string;
    inline void print_headers(const __m512i *pd) {
        // constexpr uint64_t h1_mask = (1ULL << (101ul - 64ul)) - 1ul;
        const uint64_t h0 = _mm_extract_epi64(_mm512_castsi512_si128(*pd), 0);
        const uint64_t h1 = _mm_extract_epi64(_mm512_castsi512_si128(*pd), 1);
        std::cout << "h0: " << bin_print_header_spaced2(h0) << std::endl;
        std::cout << "h1: " << bin_print_header_spaced2(h1) << std::endl;
    }

    inline void print_headers_masked(const __m512i *pd) {
        constexpr uint64_t h1_mask = (1ULL << (101ul - 64ul)) - 1ul;
        const uint64_t h0 = _mm_extract_epi64(_mm512_castsi512_si128(*pd), 0);
        const uint64_t h1 = _mm_extract_epi64(_mm512_castsi512_si128(*pd), 1) & h1_mask;
        std::cout << "h0: " << bin_print_header_spaced2(h0) << std::endl;
        std::cout << "h1: " << bin_print_header_spaced2(h1) << std::endl;
    }
    inline void print_h1(bool mask, const __m512i *pd) {
        constexpr uint64_t h1_mask = (1ULL << (101ul - 64ul)) - 1ul;
        // const uint64_t h0 = _mm_extract_epi64(_mm512_castsi512_si128(*pd), 0);
        uint64_t h1 = _mm_extract_epi64(_mm512_castsi512_si128(*pd), 1);
        if (mask)
            h1 &= h1_mask;
        // std::cout << "h0: " << bin_print_header_spaced2(h0) << std::endl;
        std::cout << "h1: " << bin_print_header_spaced2(h1) << std::endl;
    }
}// namespace v_pd512_plus

namespace pd512_plus {


    auto count_ones_up_to_the_kth_zero(const __m512i *x, size_t k = 51) -> size_t;

    auto count_zeros_up_to_the_kth_one(const __m512i *x, size_t k) -> size_t;

    auto validate_clz(int64_t quot, char rem, const __m512i *pd) -> bool;

    auto validate_clz_helper(int64_t quot, char rem, const __m512i *pd) -> bool;

    // returns the position (starting from 0) of the jth set bit of x.
    inline uint64_t select64(uint64_t x, int64_t j) {
        assert(j < 64);
        const uint64_t y = _pdep_u64(UINT64_C(1) << j, x);
        return _tzcnt_u64(y);
    }

    // returns the position (starting from 0) of the jth set bit of x.
    inline uint64_t select128(unsigned __int128 x, int64_t j) {
        const int64_t pop = _mm_popcnt_u64(x);
        if (j < pop)
            return select64(x, j);
        return 64 + select64(x >> 64, j - pop);
    }

    inline uint64_t select128withPop64(unsigned __int128 x, int64_t j, int64_t pop) {
        if (j < pop) return select64(x, j);
        return 64 + select64(x >> 64, j - pop);
    }

    // returns the number of zeros before the jth (counting from 0) set bit of x
    inline uint64_t nth64(uint64_t x, int64_t j) {
        const uint64_t y = select64(x, j);
        assert(y < 64);
        const uint64_t z = x & ((UINT64_C(1) << y) - 1);
        return y - _mm_popcnt_u64(z);
    }

    // returns the number of zeros before the jth (counting from 0) set bit of x
    inline uint64_t nth128(unsigned __int128 x, int64_t j) {
        const uint64_t y = select128(x, j);
        assert(y < 128);
        const unsigned __int128 z = x & ((((unsigned __int128) 1) << y) - 1);
        return y - _mm_popcnt_u64(z) - _mm_popcnt_u64(z >> 64);
    }

    /*Stackoverflow: https://stackoverflow.com/a/40528716/5381404  */
    inline int lzcnt_u128(unsigned __int128 u) {
        uint64_t hi = u >> 64;
        uint64_t lo = u;
        lo = (hi == 0) ? lo : -1ULL;
        return _lzcnt_u64(hi) + _lzcnt_u64(lo);
    }

    inline int tzcnt_u128(unsigned __int128 u) {
        uint64_t hi = u >> 64;
        uint64_t lo = u;
        lo = (hi == 0) ? lo : -1ULL;
        return _tzcnt_u64(hi) + _tzcnt_u64(lo);
    }
    inline int popcount64(uint64_t x) {
        return _mm_popcnt_u64(x);
    }

    inline int popcount128(unsigned __int128 x) {
        const uint64_t hi = x >> 64;
        const uint64_t lo = x;
        return popcount64(lo) + popcount64(hi);
    }

    inline int popcnt128(__m128i n) {
        const __m128i n_hi = _mm_unpackhi_epi64(n, n);
        return _mm_popcnt_u64(_mm_cvtsi128_si64(n)) + _mm_popcnt_u64(_mm_cvtsi128_si64(n_hi));
    }

    inline uint8_t get_hi_meta_bits(const __m512i *pd) {
        return _mm_extract_epi8(_mm512_castsi512_si128(*pd), 12) & 224;
    }

    inline uint8_t get_last_byte(const __m512i *pd) {
        constexpr int imm1 = 3;
        constexpr int imm2 = 15;
        uint64_t att = _mm_extract_epi8(_mm512_extracti64x2_epi64(*pd, imm1), imm2);
        uint8_t att8 = _mm_extract_epi8(_mm512_extracti64x2_epi64(*pd, imm1), imm2);
        assert(att == att8);
        uint64_t *h_array = ((uint64_t *) pd);
        uint64_t res = h_array[7] >> (64 - 8);
        assert(res == att);
        return _mm_extract_epi8(_mm512_extracti64x2_epi64(*pd, imm1), imm2);
    }

    inline uint8_t get_header_last_byte(const __m512i *pd) {
        return _mm_extract_epi8(_mm512_castsi512_si128(*pd), 12);
    }

    auto get_capacity_naive_with_OF_bit(const __m512i *x) -> size_t;

    inline auto get_capacity_easy_case(const __m512i *pd) -> int {
        constexpr uint64_t h1_mask = ((1ULL << (101 - 64)) - 1);
        const uint64_t h0 = _mm_extract_epi64(_mm512_castsi512_si128(*pd), 0);
        const uint64_t h1 = _mm_extract_epi64(_mm512_castsi512_si128(*pd), 1) & h1_mask;
        const size_t total_pop = _mm_popcnt_u64(h0) + _mm_popcnt_u64(h1);
        assert(total_pop == QUOT_SIZE);
        const int att = (h1 == 0) ? 14ul - _lzcnt_u64(h0) : (128 - QUOT_SIZE - _lzcnt_u64(h1));
        // assert(att == get_capacity_naive_with_OF_bit(pd));
        return (h1 == 0) ? 14ul - _lzcnt_u64(h0) : (128 - QUOT_SIZE - _lzcnt_u64(h1));
    }

    // switch (header_meta_bits & 224) {
    //             case 0:
    //                 return get_capacity_easy_case(pd);
    //                 break;
    //             case (32):
    //                 break;
    //             case (64):
    //                 return get_capacity_easy_case(pd);
    //                 break;
    //             case (128):
    //                 break;
    //             case (160):
    //                 break;
    //             case (192):
    //                 break;
    //             case (224):
    //                 break;
    //             default:
    //                 assert(false);
    //                 break;
    auto get_capacity(const __m512i *pd) -> int;

    auto get_specific_quot_capacity(int64_t quot, const __m512i *pd) -> int;

    inline bool pd_full(const __m512i *pd) {
        return _mm_extract_epi8(_mm512_castsi512_si128(*pd), 12) & 240;
        // return _mm_extract_epi16(_mm512_castsi512_si128(*pd), 6) & 240;
        // 16 + 32 + 64 + 128 == 240
    }

    inline uint8_t get_last_quot_for_full_pd_without_deletions(const __m512i *pd) {
        return count_ones_up_to_the_kth_zero(pd);
    }

    inline uint64_t decode_last_quot(const __m512i *pd) {
        const uint64_t header_last_byte = _mm_extract_epi8(_mm512_castsi512_si128(*pd), 12);
        auto temp = (4 - ((header_last_byte >> 3) & 3));
        // const uint64_t hi_meta_bits = _mm_extract_epi8(_mm512_castsi512_si128(*pd), 12) & (32 + 64);
        // const uint64_t hi_meta_bits = _mm_extract_epi8(_mm512_castsi512_si128(*pd), 12) & 224;
        switch (header_last_byte & (32 + 64)) {
            case 0:
                // std::cout << "d0" << std::endl;
                return (header_last_byte & 16) ? 49 : get_last_byte(pd);
                // break;
            case 32:
                // auto reduce_from_max_quot = 4 - ((header_last_byte >> 3) & 3);
                // std::cout << "d1" << std::endl;
                return (QUOT_SIZE - 1) - (4 - ((header_last_byte >> 3) & 3));
                // break;
            case 64:
                // auto reduce_from_max_quot = ((header_last_byte >> 1) & 15) + 5;
                // std::cout << "d2" << std::endl;
                return (QUOT_SIZE - 1) - ((header_last_byte >> 1) & 15) - 5;
                // break;
            case 96:
                // auto reduce_from_max_quot = ((header_last_byte  & 31) + 18);
                // std::cout << "d3" << std::endl;
                return (QUOT_SIZE - 1) - ((header_last_byte & 31) + 18);
                // break;
            default:
                assert(false);
        }
        assert(false);
        return 4242;

        // if (hi_meta_bits == 0)
        //     return get_last_quot_in_pd_naive_easy_case(pd);
        // else if (hi_meta_bits & 32) {
        //     const uint64_t att = QUOT_SIZE - (header_meta_bits >> 6);
        //     return att;
        // } else if (hi_meta_bits == 64) {
        //     const uint64_t reduce_from_max_quot = ((header_meta_bits >> 1) & 15) + 4;
        //     assert(reduce_from_max_quot >= 4);
        //     assert(reduce_from_max_quot <= 19);
        //     return QUOT_SIZE - (((header_meta_bits >> 1) & 15) + 4);
        // } else if (hi_meta_bits == 128) {
        //     const uint64_t reduce_from_max_quot = (header_meta_bits & 31) + 18;
        //     assert(reduce_from_max_quot >= 18);
        //     assert(reduce_from_max_quot <= 49);
        //     return QUOT_SIZE - ((header_meta_bits & 31) + 18);
        // } else {
        //     assert(false);
        // }
        // assert(false);
        // return 4242;
        // // switch (hi_meta_bits)
        // // {
        // // case 0:
        // //     return get_last_quot_in_pd_naive_easy_case(pd);
        // // case 64:
        // //     return QUOT_SIZE - (((header_meta_bits >> 1) & 15) + 4);
        // // case 128:
        // //     return QUOT_SIZE - ((header_meta_bits & 31) + 18);
        // // default:
        // //     return QUOT_SIZE - (header_meta_bits >> 6);
        // // }
    }

    inline uint64_t decode_last_quot_safe(const __m512i *pd) {
        const uint64_t header_last_byte = _mm_extract_epi8(_mm512_castsi512_si128(*pd), 12);

        //only for testing a single PD
        assert(!(header_last_byte & 128));

        if ((header_last_byte & (32 + 64 + 128)) == 0) {
            auto capacity = count_zeros_up_to_the_kth_one(pd, QUOT_SIZE);
            if (capacity == 0)
                return 0;
            return count_ones_up_to_the_kth_zero(pd, capacity);
        }

        auto last_quot_by_count = count_ones_up_to_the_kth_zero(pd, 51);
        assert(last_quot_by_count <= 50);
        auto last_zero_index = (last_quot_by_count + 51) - 1;
        if (last_zero_index > 100) {
            v_pd512_plus::print_headers(pd);
            v_pd512_plus::print_headers_masked(pd);
        }
        assert(last_zero_index <= 100);

        auto meta_data = (header_last_byte >> 5) & 3;
        if (meta_data == 1) {
            assert(last_zero_index <= 98);
        } else if (meta_data == 2) {
            assert(last_zero_index <= 95);
        } else if (meta_data == 3) {
            assert(last_zero_index <= 94);
        } else {
            v_pd512_plus::print_headers(pd);
            v_pd512_plus::print_headers_masked(pd);
            assert(0);
        }

        return last_quot_by_count;
    }
    // inline uint64_t decode_last_quot_in_pd_att(const __m512i *pd) {
    //     const uint64_t header_meta_bits = _mm_extract_epi8(_mm512_castsi512_si128(*pd), 12);
    //     const uint64_t hi_meta_bits = _mm_extract_epi8(_mm512_castsi512_si128(*pd), 12) & 224;

    //     if ((header_meta_bits & 240) == 0) {
    //         const uint64_t att = get_last_byte(pd);
    //         auto res = get_last_quot_in_pd_naive_easy_case(pd);
    //         assert(att == get_last_quot_in_pd_naive_easy_case(pd));
    //         return get_last_byte(pd);
    //     } else if ((header_meta_bits & 240) == 16) {
    //         const uint64_t h1 = _mm_extract_epi64(_mm512_castsi512_si128(*pd), 1);
    //         const uint64_t temp = _lzcnt_u64(~(h1 << (64 - 36)));
    //         const uint64_t att = QUOT_SIZE - temp;
    //         assert(att == get_last_quot_in_pd_naive_easy_case(pd));
    //         return att;
    //     } else if (hi_meta_bits & 32) {
    //         const uint64_t att = QUOT_SIZE - (header_meta_bits >> 6);
    //         return att;
    //     } else if (hi_meta_bits == 64) {
    //         const uint64_t reduce_from_max_quot = ((header_meta_bits >> 1) & 15) + 4;
    //         assert(reduce_from_max_quot >= 4);
    //         assert(reduce_from_max_quot <= 19);
    //         return QUOT_SIZE - (((header_meta_bits >> 1) & 15) + 4);
    //     } else if (hi_meta_bits == 128) {
    //         const uint64_t reduce_from_max_quot = (header_meta_bits & 31) + 18;
    //         assert(reduce_from_max_quot >= 18);
    //         assert(reduce_from_max_quot <= 49);
    //         return QUOT_SIZE - ((header_meta_bits & 31) + 18);
    //     } else {
    //         assert(false);
    //     }
    //     assert(false);
    //     return 4242;
    // }
    inline uint64_t decode_last_quot_wrapper(const __m512i *pd) {
        // assert(decode_last_quot(pd) == decode_last_quot_safe(pd));
        // return decode_last_quot(pd);
        auto a = decode_last_quot(pd);
        auto b = decode_last_quot_safe(pd);
        // auto c = decode_last_quot_in_pd_att(pd);
        if (a != b) {
            v_pd512_plus::print_headers(pd);
            // v_pd512_plus::print_h1(0,pd);
        }
        assert(a == b);
        // assert(a == c);
        return a;
    }

    inline uint8_t get_last_qr_in_pd(const __m512i *pd) {
        uint64_t quot = 50 - decode_last_quot_wrapper(pd);
        constexpr int imm1 = 3;
        constexpr int imm2 = 15;
        const uint64_t rem = _mm_extract_epi8(_mm512_extracti64x2_epi64(*pd, imm1), imm2);
        return (quot << 8ul) | rem;
    }


    inline bool pd_find_50_v1(int64_t quot, uint8_t rem, const __m512i *pd) {
        assert(0 == (reinterpret_cast<uintptr_t>(pd) % 64));
        assert(quot < 50);
        const __m512i target = _mm512_set1_epi8(rem);
        uint64_t v = _mm512_cmpeq_epu8_mask(target, *pd) >> 13ul;

        if (!v) return false;

        const unsigned __int128 *h = (const unsigned __int128 *) pd;
        constexpr unsigned __int128 kLeftoverMask = (((unsigned __int128) 1) << (50 + 51)) - 1;
        const unsigned __int128 header = (*h) & kLeftoverMask;
        assert(popcount128(header) == 50);
        const int64_t pop = _mm_popcnt_u64(header);
        const uint64_t begin = (quot ? (select128withPop64(header, quot - 1, pop) + 1) : 0) - quot;
        const uint64_t end = select128withPop64(header, quot, pop) - quot;
        if (begin == end) return false;
        assert(begin <= end);
        assert(end <= 51);
        return (v & ((UINT64_C(1) << end) - 1)) >> begin;
    }

    inline bool pd_find_50_v9(int64_t quot, uint8_t rem, const __m512i *pd) {

        assert(0 == (reinterpret_cast<uintptr_t>(pd) % 64));
        assert(quot < 50);
        const __m512i target = _mm512_set1_epi8(rem);
        uint64_t v = _mm512_cmpeq_epu8_mask(target, *pd) >> 13ul;

        if (!v)
            return false;


        if ((_blsr_u64(v) == 0) && (v << quot)) {
            const int64_t h0 = ((uint64_t *) pd)[0];
            // const unsigned __int128 *h = (const unsigned __int128 *) pd;
            // const unsigned __int128 header = (*h);
            const int64_t mask = v << quot;
            const bool att = (!(h0 & mask)) && _mm_popcnt_u64(h0 & (mask - 1)) == quot;
            assert(att == pd_find_50_v1(quot, rem, pd));
            return (!(h0 & mask)) && _mm_popcnt_u64(h0 & (mask - 1)) == quot;
        }


        const unsigned __int128 *h = (const unsigned __int128 *) pd;
        constexpr unsigned __int128 kLeftoverMask = (((unsigned __int128) 1) << (50 + 51)) - 1;
        const unsigned __int128 header = (*h) & kLeftoverMask;

        const int64_t pop = _mm_popcnt_u64(header);
        const uint64_t begin = (quot ? (select128withPop64(header, quot - 1, pop) + 1) : 0) - quot;
        const uint64_t end = select128withPop64(header, quot, pop) - quot;

        if (begin == end) return false;
        assert(begin <= end);
        assert(end <= 51);
        return (v & ((UINT64_C(1) << end) - 1)) >> begin;
    }

    inline bool pd_find_50_v11(int64_t quot, uint8_t rem, const __m512i *pd) {

        const __m512i target = _mm512_set1_epi8(rem);
        const uint64_t v = _mm512_cmpeq_epu8_mask(target, *pd) >> 13ul;

        if (!v) return false;

        const uint64_t h0 = _mm_extract_epi64(_mm512_castsi512_si128(*pd), 0);
        const uint64_t h1 = _mm_extract_epi64(_mm512_castsi512_si128(*pd), 1);

        if ((_blsr_u64(v) == 0)) {
            if (v << quot) {
                const int64_t mask = v << quot;
                const bool att = (!(h0 & mask)) && (_mm_popcnt_u64(h0 & (mask - 1)) == quot);
                assert(att == pd_find_50_v1(quot, rem, pd));
                return (!(h0 & mask)) && (_mm_popcnt_u64(h0 & (mask - 1)) == quot);
            } else {
                const unsigned __int128 *h = (const unsigned __int128 *) pd;
                constexpr unsigned __int128 kLeftoverMask = (((unsigned __int128) 1) << (50 + 51)) - 1;
                const unsigned __int128 header = (*h) & kLeftoverMask;
                const unsigned __int128 mask = ((unsigned __int128) v) << quot;
                const bool att = (!(header & mask)) && (popcount128(header & (mask - 1)) == quot);
                assert(att == pd_find_50_v1(quot, rem, pd));
                return (!(header & mask)) && (popcount128(header & (mask - 1)) == quot);
            }
        }


        const int64_t pop = _mm_popcnt_u64(h0);

        if (quot == 0) {
            return v & (_blsmsk_u64(_mm_extract_epi64(_mm512_castsi512_si128(*pd), 0)) >> 1ul);
        } else if (quot < pop - 1) {
            const uint64_t y = _pdep_u64(UINT64_C(3) << (quot - 1), h0);
            const uint64_t temp = _tzcnt_u64(y) + 1;
            const uint64_t diff = _tzcnt_u64(y >> temp);
            return diff && ((v >> (temp - quot)) & ((UINT64_C(1) << diff) - 1));
        } else if (quot == pop - 1) {
            const uint64_t leading_one_index = _lzcnt_u64(h0);
            const uint64_t next_leading_one_index = _lzcnt_u64(h0 ^ (1ull << (63ul - leading_one_index)));
            const uint64_t diff = next_leading_one_index - leading_one_index - 1;
            const uint64_t temp = (63 - next_leading_one_index) + 1;
            return diff && ((v >> (temp - quot)) & ((UINT64_C(1) << diff) - 1));
        } else if (quot == pop) {
            const uint64_t helper = _lzcnt_u64(h0);
            const uint64_t temp = (63 - helper) + 1;
            const uint64_t diff = helper + _tzcnt_u64(h1);
            return diff && ((v >> (temp - quot)) & ((UINT64_C(1) << diff) - 1));
        } else if (quot == pop + 1) {
            const uint64_t temp = _tzcnt_u64(h1) + 1;
            const uint64_t diff = _tzcnt_u64(h1 >> temp);
            return diff && ((v >> (temp - quot)) & ((UINT64_C(1) << diff) - 1));
        } else {
            const uint64_t y = _pdep_u64(UINT64_C(3) << (quot - pop - 1), h1);
            const uint64_t temp = _tzcnt_u64(y) + 1;
            const uint64_t diff = _tzcnt_u64(y >> temp);
            return diff && ((v >> (temp - quot)) & ((UINT64_C(1) << diff) - 1));
        }
    }

    inline bool pd_find_50_v17(int64_t quot, uint8_t rem, const __m512i *pd) {
        assert(quot < 50);
        const __m512i target = _mm512_set1_epi8(rem);
        uint64_t v = _mm512_cmpeq_epu8_mask(target, *pd) >> 13ul;

        if (!v) return false;

        const uint64_t h0 = _mm_extract_epi64(_mm512_castsi512_si128(*pd), 0);
        const uint64_t h1 = _mm_extract_epi64(_mm512_castsi512_si128(*pd), 1);
        if (_blsr_u64(v) == 0) {
            if (v << quot) {
                const int64_t mask = v << quot;
                const bool att = (!(h0 & mask)) && (_mm_popcnt_u64(h0 & (mask - 1)) == quot);
                assert(att == pd_find_50_v1(quot, rem, pd));
                return (!(h0 & mask)) && (_mm_popcnt_u64(h0 & (mask - 1)) == quot);
            } else {
                const int64_t pop = _mm_popcnt_u64(h0);
                const uint64_t index = (_tzcnt_u64(v) + quot) & 63;
                const int64_t mask = (1ULL << index);
                const bool att = (!(h1 & mask)) && (_mm_popcnt_u64(h1 & (mask - 1)) == (quot - pop));
                assert(att == pd_find_50_v1(quot, rem, pd));

                // const unsigned __int128 *h = (const unsigned __int128 *) pd;
                // constexpr unsigned __int128 kLeftoverMask = (((unsigned __int128) 1) << (50 + 51)) - 1;
                // const unsigned __int128 header = (*h) & kLeftoverMask;
                // const unsigned __int128 mask = ((unsigned __int128) v) << quot;

                // const bool att = (!(header & mask)) && (popcount128(header & (mask - 1)) == quot);
                // assert(att == pd_find_50_v1(quot, rem, pd));
                // return (!(header & mask)) && (popcount128(header & (mask - 1)) == quot);
            }
        }

        const int64_t pop = _mm_popcnt_u64(h0);

        if (quot == 0) {
            // //std::cout << "h0" << std::endl;
            return v & (_blsmsk_u64(h0) >> 1ul);
        } else if (quot < pop) {
            // //std::cout << "h1" << std::endl;
            const uint64_t mask = (~_bzhi_u64(-1, quot - 1));
            const uint64_t h_cleared_quot_set_bits = _pdep_u64(mask, h0);
            return (((_blsmsk_u64(h_cleared_quot_set_bits) ^ _blsmsk_u64(_blsr_u64(h_cleared_quot_set_bits))) & (~h0)) >> quot) & v;
        } else if (quot > pop) {
            // //std::cout << "h2" << std::endl;

            const uint64_t mask = (~_bzhi_u64(-1, quot - pop - 1));
            const uint64_t h_cleared_quot_set_bits = _pdep_u64(mask, h1);
            return (((_blsmsk_u64(h_cleared_quot_set_bits) ^ _blsmsk_u64(_blsr_u64(h_cleared_quot_set_bits))) & (~h1)) >> (quot - pop)) & (v >> (64 - pop));
        } else {
            // //std::cout << "h3" << std::endl;

            const uint64_t helper = _lzcnt_u64(h0);
            const uint64_t temp = (63 - helper) + 1;
            const uint64_t diff = helper + _tzcnt_u64(h1);
            return diff && ((v >> (temp - quot)) & ((UINT64_C(1) << diff) - 1));
        }
    }

    inline bool pd_find_50_v18(int64_t quot, uint8_t rem, const __m512i *pd) {
        assert(0 == (reinterpret_cast<uintptr_t>(pd) % 64));
        assert(quot < 50);
        const __m512i target = _mm512_set1_epi8(rem);
        uint64_t v = _mm512_cmpeq_epu8_mask(target, *pd) >> 13ul;

        if (!v) return false;

        const uint64_t h0 = _mm_extract_epi64(_mm512_castsi512_si128(*pd), 0);
        const uint64_t h1 = _mm_extract_epi64(_mm512_castsi512_si128(*pd), 1);
        if (_blsr_u64(v) == 0) {
            if (v << quot) {
                // const unsigned __int128 *h = (const unsigned __int128 *) pd;
                // const unsigned __int128 header = (*h);
                const int64_t mask = v << quot;
                // const bool att = (!(h0 & mask)) && (_mm_popcnt_u64(h0 & (mask - 1)) == quot);
                // assert(att == pd_find_50_v1(quot, rem, pd));
                return (!(h0 & mask)) && (_mm_popcnt_u64(h0 & (mask - 1)) == quot);
            } else {
                const unsigned __int128 *h = (const unsigned __int128 *) pd;
                constexpr unsigned __int128 kLeftoverMask = (((unsigned __int128) 1) << (50 + 51)) - 1;
                const unsigned __int128 header = (*h) & kLeftoverMask;
                const unsigned __int128 mask = ((unsigned __int128) v) << quot;
                // const bool att = (!(header & mask)) && (popcount128(header & (mask - 1)) == quot);
                // assert(att == pd_find_50_v1(quot, rem, pd));
                return (!(header & mask)) && (popcount128(header & (mask - 1)) == quot);
            }
        }

        const int64_t pop = _mm_popcnt_u64(h0);

        if (quot == 0) {
            // //std::cout << "h0" << std::endl;
            return v & (_blsmsk_u64(h0) >> 1ul);
        } else if (quot < pop) {
            // //std::cout << "h1" << std::endl;
            const uint64_t mask = (~_bzhi_u64(-1, quot - 1));
            const uint64_t h_cleared_quot_set_bits = _pdep_u64(mask, h0);
            return (((_blsmsk_u64(h_cleared_quot_set_bits) ^ _blsmsk_u64(_blsr_u64(h_cleared_quot_set_bits))) & (~h0)) >> quot) & v;
        } else if (quot > pop) {
            // //std::cout << "h2" << std::endl;

            const uint64_t mask = (~_bzhi_u64(-1, quot - pop - 1));
            const uint64_t h_cleared_quot_set_bits = _pdep_u64(mask, h1);
            return (((_blsmsk_u64(h_cleared_quot_set_bits) ^ _blsmsk_u64(_blsr_u64(h_cleared_quot_set_bits))) & (~h1)) >> (quot - pop)) & (v >> (64 - pop));
        } else {
            // //std::cout << "h3" << std::endl;

            const uint64_t helper = _lzcnt_u64(h0);
            const uint64_t temp = (63 - helper) + 1;
            const uint64_t diff = helper + _tzcnt_u64(h1);
            return diff && ((v >> (temp - quot)) & ((UINT64_C(1) << diff) - 1));
        }
    }

    inline bool pd_find_50_old(int64_t quot, uint8_t rem, const __m512i *pd) {
        assert(pd_find_50_v1(quot, rem, pd) == pd_find_50_v9(quot, rem, pd));
        assert(pd_find_50_v1(quot, rem, pd) == pd_find_50_v11(quot, rem, pd));
        assert(pd_find_50_v1(quot, rem, pd) == pd_find_50_v17(quot, rem, pd));
        assert(pd_find_50_v1(quot, rem, pd) == pd_find_50_v18(quot, rem, pd));
        return pd_find_50_v18(quot, rem, pd);
    }


    inline bool did_pd_overflowed(const __m512i *pd) {
        return (_mm_extract_epi8(_mm512_castsi512_si128(*pd), 12) & 128);
        //224 == 128 + 64 + 32;
    }
    inline void set_overflow_bit(__m512i *pd) {
        uint64_t *h_array = ((uint64_t *) pd);
        h_array[1] |= (1ULL) << (103 - 64);
        assert(did_pd_overflowed(pd));
    }
    inline void clear_overflow_bit(__m512i *pd) {
        assert(false);
        uint64_t *h_array = ((uint64_t *) pd);
        // assert(h_array[1] & (1ULL) << (101 - 64));
        assert(did_pd_overflowed(pd));
        h_array[1] ^= (1ULL) << (103 - 64);
        assert(!did_pd_overflowed(pd));

        // h_array[1] &= (((1ULL << 101 - 64) - 1) | ~(((1ULL << 41) - 1)));
    }

    inline void update_max_quot_when_pd_is_not_full(int64_t quot, __m512i *pd) {
        assert(!pd_full(pd));
        const uint64_t old_quot = get_last_byte(pd);
        if (quot <= old_quot)
            return;

        memcpy(&((uint8_t *) pd)[63], &quot, 1);
        assert(get_last_byte(pd) == quot);
    }

    inline void encode_last_quot_only_decrease(int64_t last_quot, __m512i *pd) {
        // v_pd512_plus::print_headers(pd);
        uint64_t decrease_by = QUOT_SIZE - last_quot - 1;
        const uint64_t header_last_byte = _mm_extract_epi8(_mm512_castsi512_si128(*pd), 12);
        uint64_t word = header_last_byte;
        if (decrease_by == 0) {
            assert(0);
        }
        if (decrease_by <= 4) {
            uint64_t bits_to_read = 4 - decrease_by;
            assert(bits_to_read <= 3);
            word &= 7;
            word |= (bits_to_read << 3);
            word |= 32;

        } else if (decrease_by <= 20) {
            uint64_t decrease_by_with_offset = decrease_by - 5;
            assert(decrease_by_with_offset <= 15);
            uint64_t a = header_last_byte & 1;
            uint64_t b = 64;
            uint64_t c = decrease_by_with_offset << 1u;
            uint64_t d = (a & b) || (a & c) || (b & c);
            assert(!d);
            word = (header_last_byte & 1) | 64 | (decrease_by_with_offset << 1u);
        } else {
            uint64_t decrease_by_with_offset = decrease_by - 18;
            // uint64_t a = header_last_byte & 1;
            uint64_t b = 32 + 64;
            uint64_t c = decrease_by_with_offset;
            uint64_t d = !(b & c);
            assert(d);
            word = (32 + 64) | decrease_by_with_offset;
        }
        const uint64_t OF_bit = header_last_byte & 128;
        word |= OF_bit;
        //std::cout << std::string(80, '-') << std::endl;
        // v_pd512_plus::print_h1(0, pd);
        memcpy(&((uint8_t *) pd)[12], &word, 1);
        // v_pd512_plus::print_h1(0, pd);
        //std::cout << std::string(80, '-') << std::endl;
        // v_pd512_plus::print_headers(pd);
        assert(decode_last_quot_wrapper(pd) == last_quot);
    }

    inline void encode_last_quot_when_full_for_the_first_time(int64_t old_last_quot, int64_t new_quot, __m512i *pd) {
        const uint64_t new_last_quot = (new_quot < old_last_quot) ? old_last_quot : new_quot;
        if (new_last_quot == 49) {
            uint64_t hi_meta_bits = get_hi_meta_bits(pd);
            auto temp_value = hi_meta_bits & (32 + 64);
            assert(temp_value == 0);
            return;
        }
        encode_last_quot_only_decrease(new_last_quot, pd);
    }

    inline void decrease_pd_max_quot_after_swap_insertion(int64_t possible_max_quot, int64_t old_quot, __m512i *pd) {
        // set_overflow_bit(pd);
        // assert(pd_full(pd));
        //Todo: FIX THIS! (might work but probably cause many weird problems)
        //Todo: this should be computed before the deletion. depeneding on the next zero place.
        //currently, the PD state is not valid.
        const uint64_t new_quot = get_last_quot_for_full_pd_without_deletions(pd);
        if (new_quot == old_quot)
            return;
        assert(new_quot < old_quot);
        assert(possible_max_quot <= new_quot);
        encode_last_quot_only_decrease(new_quot, pd);
    }


    // inline bool did_pd_overflowed2(const __m512i *pd) {
    //     constexpr __m128i x = __m128i{0, 1ULL << 38ul};
    //     return _mm_extract_epi16(_mm_and_si128(_mm512_castsi512_si128(*pd), x), 6);
    //     // bool res = (_mm_extract_epi16(_mm512_castsi512_si128(*pd), 6) & 64);
    //     // bool att = _mm_extract_epi16(_mm_and_si128(_mm512_castsi512_si128(*pd), x), 6);
    //     // assert(res == att);
    //     // return (_mm_extract_epi16(_mm512_castsi512_si128(*pd), 6) & 64);
    // }
    inline bool pd_find_special_50(int64_t quot, uint8_t rem, const __m512i *pd) {
        return pd_find_50_v18(quot, rem, pd) || did_pd_overflowed(pd);
        // return pd_find_50_v18(quot, rem, pd) || did_pd_overflowed(pd);
        // return pd_find_50_v11(quot, rem, pd) || did_pd_overflowed(pd);
    }

    enum pd_Status {
        No,
        Yes,
        look_in_the_next_level,
        Error
    };

    inline pd_Status pd_find_enums(int64_t quot, uint8_t rem, const __m512i *pd) {
        return pd_find_50_v18(quot, rem, pd) ? Yes : (did_pd_overflowed(pd) ? look_in_the_next_level : No);
        // return pd_find_50_v18(quot, rem, pd) || did_pd_overflowed(pd);
        // return pd_find_50_v18(quot, rem, pd) || did_pd_overflowed(pd);
        // return pd_find_50_v11(quot, rem, pd) || did_pd_overflowed(pd);
    }


    inline pd_Status pd_find_50_case2_helper(int64_t quot, uint8_t rem, const __m512i *pd) {
        int last_byte = get_last_byte(pd);
        int actual_rem = rem;
        int cmp8 = get_last_byte(pd) < rem;
        int cmp_int = last_byte < actual_rem;
        assert(cmp8 == cmp_int);
        if (pd_full(pd)) {
            if (get_last_byte(pd) < rem) {
                //std::cout << "_1" << std::endl;
                return did_pd_overflowed(pd) ? look_in_the_next_level : No;
            } else if (get_last_byte(pd) == rem) {
                //std::cout << "_2" << std::endl;
                return Yes;
            } else {
                //std::cout << "_3" << std::endl;
                return pd_find_50_v18(quot, rem, pd) ? Yes : No;
            }
        }
        //std::cout << "_4" << std::endl;
        // //std::cout << std::endl;
        return pd_find_50_v18(quot, rem, pd) ? Yes : No;
    }

    inline pd_Status pd_find_50(int64_t quot, uint8_t rem, const __m512i *pd) {
        const uint64_t last_quot = decode_last_quot_wrapper(pd);
        int cmp = (quot < last_quot) + (quot == last_quot) * 2;
        switch (cmp) {
            case 1:
                //std::cout << "l1" << std::endl;
                return pd_find_50_v18(quot, rem, pd) ? Yes : No;
            case 0:
                //std::cout << "l0" << std::endl;
                return did_pd_overflowed(pd) ? look_in_the_next_level : No;
            case 2:
                //std::cout << "l2";
                return pd_find_50_case2_helper(quot, rem, pd);
            default:
                break;
        }
        return Error;
    }

    inline void write_header(uint64_t begin, uint64_t end, const unsigned __int128 header, __m512i *pd) {
        const uint64_t old_header_last_byte = get_header_last_byte(pd);
        const uint64_t old_header_meta_bits = get_hi_meta_bits(pd);
        constexpr unsigned kBytes2copy = (50 + 51 + CHAR_BIT - 1) / CHAR_BIT;
        constexpr unsigned __int128 kLeftoverMask = (((unsigned __int128) 1) << (50 + 51)) - 1;

        unsigned __int128 new_header = header & ((((unsigned __int128) 1) << begin) - 1);
        new_header |= ((header >> end) << (end + 1));
        new_header &= kLeftoverMask;

        uint64_t header_last_byte_low_bits = (new_header >> 96);
        assert(header_last_byte_low_bits <= 31);
        uint64_t header_last_byte_hi_bits = get_hi_meta_bits(pd);
        assert((header_last_byte_low_bits & header_last_byte_hi_bits) == 0);
        uint64_t header_last_byte = header_last_byte_low_bits | header_last_byte_hi_bits;
        // assert(popcount128(new_header) == 50);
        // assert(select128(new_header, 50 - 1) - (50 - 1) == fill + 1);
        //redundent
        // new_header |= (did_pd_overflowed(pd)) ? ((unsigned __int128) 1) << 101ul : 0;
        memcpy(pd, &new_header, 12);
        memcpy(&((uint8_t *) pd)[12], &header_last_byte, 1);
        assert(old_header_meta_bits == get_hi_meta_bits(pd));
    }

    inline uint64_t pd_add_50_not_full_after(int64_t quot, uint8_t rem, __m512i *pd) {
        assert(quot < 50);
        assert(!pd_full(pd));
        const unsigned __int128 *h = (const unsigned __int128 *) pd;
        constexpr unsigned __int128 kLeftoverMask = (((unsigned __int128) 1) << (50 + 51)) - 1;
        const unsigned __int128 header = (*h) & kLeftoverMask;
        constexpr unsigned kBytes2copy = (50 + 51 + CHAR_BIT - 1) / CHAR_BIT;
        // assert(popcount128(header) == 50);

        const uint64_t begin = quot ? (select128(header, quot - 1) + 1) : 0;
        const uint64_t end = select128(header, quot);
        assert(begin <= end);
        assert(end <= 50 + 51);
        const __m512i target = _mm512_set1_epi8(rem);

        write_header(begin, end, header, pd);
        // unsigned __int128 new_header = header & ((((unsigned __int128) 1) << begin) - 1);
        // new_header |= ((header >> end) << (end + 1));
        // assert(popcount128(new_header) == 50);
        // // assert(select128(new_header, 50 - 1) - (50 - 1) == fill + 1);
        // //redundent
        // new_header |= (did_pd_overflowed(pd)) ? ((unsigned __int128) 1) << 101ul : 0;
        // memcpy(pd, &new_header, kBytes2copy);

        const uint64_t begin_fingerprint = begin - quot;
        const uint64_t end_fingerprint = end - quot;
        assert(begin_fingerprint <= end_fingerprint);
        assert(end_fingerprint <= 51);

        uint64_t i = begin_fingerprint;
        for (; i < end_fingerprint; ++i) {
            if (rem <= ((const uint8_t *) pd)[kBytes2copy + i]) break;
        }
        assert((i == end_fingerprint) ||
               (rem <= ((const uint8_t *) pd)[kBytes2copy + i]));

        //Todo Fix this:
        //
        //  uint64_t j = (begin == end) ? end_fingerprint : _tzcnt_u64(((_mm512_cmp_epi8_mask(target, *pd, 2) >> (begin_fingerprint + 13)) << (begin_fingerprint)) | (1ull << end_fingerprint));
        //
        // //std::cout << "IJ" << std::endl;
        // assert(i == j);
        // assert((i == end_fingerprint) || (rem <= ((const uint8_t *) pd)[kBytes2copy + i]));

        memmove(&((uint8_t *) pd)[kBytes2copy + i + 1],
                &((const uint8_t *) pd)[kBytes2copy + i],
                sizeof(*pd) - (kBytes2copy + i + 2));// changed 1 to 2.
        ((uint8_t *) pd)[kBytes2copy + i] = rem;

        return 1ul << 15;
    }

    inline uint64_t pd_add_50_full_after(int64_t quot, uint8_t rem, __m512i *pd) {
        //std::cout << "A_fa" << std::endl;
        assert(quot < 50);
        assert(!pd_full(pd));
        const unsigned __int128 *h = (const unsigned __int128 *) pd;
        constexpr unsigned __int128 kLeftoverMask = (((unsigned __int128) 1) << (50 + 51)) - 1;
        const unsigned __int128 header = (*h) & kLeftoverMask;
        constexpr unsigned kBytes2copy = (50 + 51 + CHAR_BIT - 1) / CHAR_BIT;
        // assert(popcount128(header) == 50);

        const uint64_t begin = quot ? (select128(header, quot - 1) + 1) : 0;
        const uint64_t end = select128(header, quot);
        assert(begin <= end);
        assert(end <= 50 + 51);
        const __m512i target = _mm512_set1_epi8(rem);

        write_header(begin, end, header, pd);
        // unsigned __int128 new_header = header & ((((unsigned __int128) 1) << begin) - 1);
        // new_header |= ((header >> end) << (end + 1));
        // new_header &= kLeftoverMask;
        // assert(popcount128(new_header) == 50);
        // // assert(select128(new_header, 50 - 1) - (50 - 1) == fill + 1);
        // //redundent
        // // new_header |= (did_pd_overflowed(pd)) ? ((unsigned __int128) 1) << 103ul : 0;
        // memcpy(pd, &new_header, kBytes2copy);

        const uint64_t begin_fingerprint = begin - quot;
        const uint64_t end_fingerprint = end - quot;
        assert(begin_fingerprint <= end_fingerprint);
        assert(end_fingerprint <= 51);

        uint64_t i = begin_fingerprint;
        for (; i < end_fingerprint; ++i) {
            if (rem <= ((const uint8_t *) pd)[kBytes2copy + i]) break;
        }
        assert((i == end_fingerprint) ||
               (rem <= ((const uint8_t *) pd)[kBytes2copy + i]));

        // uint64_t j = (begin == end) ? end_fingerprint : _tzcnt_u64(((_mm512_cmp_epi8_mask(target, *pd, 2) >> (begin_fingerprint + 13)) << (begin_fingerprint)) | (1ull << end_fingerprint));

        // assert((j == end_fingerprint) || (rem <= ((const uint8_t *) pd)[kBytes2copy + j]));
        // assert(j == i);

        memmove(&((uint8_t *) pd)[kBytes2copy + i + 1],
                &((const uint8_t *) pd)[kBytes2copy + i],
                sizeof(*pd) - (kBytes2copy + i + 1));
        ((uint8_t *) pd)[kBytes2copy + i] = rem;

        return 1ul << 15;
    }

    inline uint64_t pd_add_50_only_rem(uint8_t rem, size_t quot_capacity, __m512i *pd) {
        // assert(quot < 50);
        // assert(!pd_full(pd));
        // const unsigned __int128 *h = (const unsigned __int128 *) pd;
        // constexpr unsigned __int128 kLeftoverMask = (((unsigned __int128) 1) << (50 + 51)) - 1;
        // const unsigned __int128 header = (*h) & kLeftoverMask;
        constexpr unsigned kBytes2copy = (50 + 51 + CHAR_BIT - 1) / CHAR_BIT;
        // // assert(popcount128(header) == 50);

        // const uint64_t begin = quot ? (select128(header, quot - 1) + 1) : 0;
        // const uint64_t end = select128(header, quot);
        // assert(begin <= end);
        // assert(end <= 50 + 51);
        // const __m512i target = _mm512_set1_epi8(rem);

        // unsigned __int128 new_header = header & ((((unsigned __int128) 1) << begin) - 1);
        // new_header |= ((header >> end) << (end + 1));
        // assert(popcount128(new_header) == 50);
        // // assert(select128(new_header, 50 - 1) - (50 - 1) == fill + 1);
        // //redundent
        // new_header |= (did_pd_overflowed(pd)) ? ((unsigned __int128) 1) << 101ul : 0;
        // memcpy(pd, &new_header, kBytes2copy);

        const __m512i target = _mm512_set1_epi8(rem);
        const uint64_t begin_fingerprint = 51 - quot_capacity;
        const uint64_t end_fingerprint = 51;
        assert(begin_fingerprint < end_fingerprint);
        assert(end_fingerprint <= 51);

        uint64_t i = begin_fingerprint;
        for (; i < end_fingerprint; ++i) {
            if (rem <= ((const uint8_t *) pd)[kBytes2copy + i]) break;
        }
        assert((i == end_fingerprint) ||
               (rem <= ((const uint8_t *) pd)[kBytes2copy + i]));

        // uint64_t i =
        //         _tzcnt_u64(((_mm512_cmp_epi8_mask(target, *pd, 2) >> (begin_fingerprint + 13)) << (begin_fingerprint)) | (1ull << end_fingerprint));

        // assert((i == end_fingerprint) || (rem <= ((const uint8_t *) pd)[kBytes2copy + i]));

        memmove(&((uint8_t *) pd)[kBytes2copy + i + 1],
                &((const uint8_t *) pd)[kBytes2copy + i],
                sizeof(*pd) - (kBytes2copy + i + 1));
        ((uint8_t *) pd)[kBytes2copy + i] = rem;

        return 1ul << 15;
    }

    inline auto conditional_remove(int64_t quot, uint8_t rem, __m512i *pd) -> bool {
        /* This function has na error. */
        assert(quot < 50);
        // unsigned __int128 header = 0;
        const __m512i target = _mm512_set1_epi8(rem);
        const uint64_t v = _mm512_cmpeq_epu8_mask(target, *pd) >> 13ul;

        if (!v) return false;

        const unsigned __int128 *h = (const unsigned __int128 *) pd;
        constexpr unsigned __int128 kLeftoverMask = (((unsigned __int128) 1) << (50 + 51)) - 1;
        const unsigned __int128 header = (*h) & kLeftoverMask;
        // assert(popcount128(header) == 50);
        constexpr unsigned kBytes2copy = (50 + 51 + CHAR_BIT - 1) / CHAR_BIT;


        const uint64_t h0 = _mm_extract_epi64(_mm512_castsi512_si128(*pd), 0);
        const uint64_t h1 = _mm_extract_epi64(_mm512_castsi512_si128(*pd), 1);
        const int64_t pop = _mm_popcnt_u64(h0);
        const uint64_t begin = quot ? (select128(header, quot - 1) + 1) : 0;
        const uint64_t end = select128(header, quot);
        assert(begin <= end);

        const uint64_t begin_fingerprint = begin - quot;
        const uint64_t end_fingerprint = end - quot;

        const uint64_t b_mask = ~((1ull << (begin_fingerprint)) - 1);
        const uint64_t end_mask = ((1ull << end_fingerprint) - 1);
        const uint64_t mask = b_mask & end_mask;
        assert((begin < end) ? mask : !mask);
        const uint64_t v_masked = v & mask;
        if (!v_masked) return false;

        const uint64_t i = _tzcnt_u64(v_masked);
        assert(pd_find_50(quot, rem, pd));
        unsigned __int128 new_header = header & ((((unsigned __int128) 1) << begin) - 1);
        new_header |= ((header >> end) << (end - 1));
        // assert(popcount128(new_header) == 50);
        new_header |= (did_pd_overflowed(pd)) ? ((unsigned __int128) 1) << 101ul : 0;


        memcpy(pd, &new_header, kBytes2copy);
        memmove(&((uint8_t *) pd)[kBytes2copy + i],
                &((const uint8_t *) pd)[kBytes2copy + i + 1],
                sizeof(*pd) - (kBytes2copy + i + 1));
        return true;
    }

    inline int my_cmp(int64_t quot1, char rem1, int64_t quot2, char rem2) {
        if (quot1 != quot2)
            return quot1 < quot2;
        return rem1 <= rem2;
    }

    inline uint64_t pd_swap_short(int64_t quot, uint8_t rem, __m512i *pd) {
        const uint64_t new_qr = (quot << 8u) | ((uint64_t) rem);
        const uint64_t last_quot = decode_last_quot_wrapper(pd);
        const uint64_t old_rem = get_last_byte(pd);
        uint64_t old_qr = (last_quot << 8) | old_rem;
        // std::cout << "last_quot: " << last_quot << std::endl;
        // std::cout << "old_rem: " << old_rem << std::endl;
        // std::cout << "old_qr: " << old_qr << std::endl;


        if (old_qr <= new_qr) {
            // std::cout << "swap_1" << std::endl;
            return new_qr;
        }
        //Only remainder update (no quot decreasing).
        if (last_quot == quot) {
            // std::cout << "swap_2" << std::endl;
            size_t quot_capacity = get_specific_quot_capacity(quot, pd);
            // if (!quot_capacity){
            //     std::cout << "quot: " << quot << std::endl;
            //     v_pd512_plus::print_headers(pd);
            // }
            assert(quot_capacity);
            pd_add_50_only_rem(rem, quot_capacity, pd);
            return old_qr;
        }

        // std::cout << "swap_3" << std::endl;
        conditional_remove(last_quot, get_last_byte(pd), pd);
        pd_add_50_full_after(quot, rem, pd);
        // This might be redundent in some cases.
        decrease_pd_max_quot_after_swap_insertion(quot, last_quot, pd);
        return old_qr;
    }

    inline uint64_t pd_conditional_add_50(int64_t quot, uint8_t rem, __m512i *pd) {
        const uint64_t header_last_byte = _mm_extract_epi8(_mm512_castsi512_si128(*pd), 12);
        if ((header_last_byte & 248) == 0) {
            // std::cout << "ca_1" << std::endl;
            uint64_t res = pd_add_50_not_full_after(quot, rem, pd);
            assert(res == (1 << 15));
            update_max_quot_when_pd_is_not_full(quot, pd);
            auto temp = pd_find_50(quot, rem, pd);
            assert(pd_find_50(quot, rem, pd) == Yes);
            return res;
        } else if ((header_last_byte & 248) == 8) {
            // std::cout << "ca_2" << std::endl;
            //std::cout << "before adding" << std::endl;
            // v_pd512_plus::print_h1(0, pd);
            // v_pd512_plus::print_headers_masked(pd);
            // //std::cout << std::endl;

            const uint64_t last_quot = get_last_byte(pd);
            uint64_t res = pd_add_50_full_after(quot, rem, pd);
            int actual_rem = rem;
            uint64_t actual_rem2 = rem;
            const uint64_t new_last_byte = get_last_byte(pd);
            assert(res == (1 << 15));
            encode_last_quot_when_full_for_the_first_time(last_quot, quot, pd);
            //std::cout << "after adding" << std::endl;
            // v_pd512_plus::print_h1(0, pd);
            //std::cout << std::string(84, '-') << std::endl;

            const uint64_t new_last_quot = decode_last_quot_wrapper(pd);
            assert((new_last_quot == quot) || (new_last_quot == last_quot));
            // auto temp = pd_find_50(quot, rem, pd);
            // if (pd_find_50(quot, rem, pd) != Yes) {
            //     pd_find_50(quot, rem, pd);
            //     assert(0);
            // }
            assert(pd_find_50(quot, rem, pd) == Yes);
            return res;
        } else {
            // std::cout << "ca_3" << std::endl;
            assert(pd_full(pd));
            return pd_swap_short(quot, rem, pd);
        }
    }

    void print512(const __m512i *var);

    inline auto pd_popcount_att_helper_start(const __m512i *pd) -> int {
        uint64_t header;
        memcpy(&header, reinterpret_cast<const uint64_t *>(pd), 8);
        return 14ul - _lzcnt_u64(header);
    }

    inline auto get_capacity_without_OF_bit(const __m512i *pd) -> int {
        uint64_t header_end;
        memcpy(&header_end, reinterpret_cast<const uint64_t *>(pd) + 1, 5);
        constexpr uint64_t mask = (1ULL << 37u) - 1u;
        header_end &= mask;
        return (header_end == 0) ? pd_popcount_att_helper_start(pd) : (128 - 51 - _lzcnt_u64(header_end) + 1);
    }

    auto validate_number_of_quotient(const __m512i *pd) -> bool;

    auto get_capacity_old(const __m512i *x) -> size_t;

    auto get_capacity_naive(const __m512i *x) -> size_t;

    auto get_name() -> std::string;

}// namespace pd512_plus


#endif// FILTERS_PD512_PLUS_HPP


// inline uint64_t pd_swap(int64_t quot, uint8_t rem, __m512i *pd) {
//     assert(quot < 50);
//     const size_t capacity = pd_popcount(pd);
//     constexpr uint64_t h1_mask = (1ULL << (101ul - 64ul)) - 1ul;
//     const uint64_t h1 = _mm_extract_epi64(_mm512_castsi512_si128(*pd), 1) & h1_mask;

//     assert(h1);
//     const uint64_t temp = h1 << (64 - 37);
//     assert((temp >> (64 - 37)) == h1);
//     const uint64_t index = _lzcnt_u64(temp);

//     const int64_t old_quot = get_last_quot_in_pd(pd);
//     if (old_quot < quot) {
//std::cout << "pd_swap 0" << std::endl;
//         set_overflow_bit(pd);
//         uint64_t res = (quot << 8u) | ((uint64_t) rem);
//         assert(res);
//         return res;
//     }
//     constexpr int imm1 = 3;
//     constexpr int imm2 = 15;
//     const uint8_t old_rem = _mm_extract_epi8(_mm512_extracti64x2_epi64(*pd, imm1), imm2);


//     if ((old_quot == quot) && (old_rem <= rem)) {
//std::cout << "pd_swap 1" << std::endl;
//         set_overflow_bit(pd);
//         uint64_t res = (quot << 8u) | ((uint64_t) rem);
//         if (!res) {
//             v_pd512::print_headers_masked(pd);
//         }
//         assert(res);
//         return res;
//         // return (quot << 8u) | ((uint8_t)rem);
//     }

//     const uint64_t old_qr = (old_quot << 8) | ((uint64_t) old_rem);
//     const uint64_t new_qr = (quot << 8) | ((uint64_t) rem);
//     if (old_qr <= new_qr) {
//         v_pd512::bin_print(old_qr);
//     }
//     assert(old_qr > new_qr);

//     const __m512i target = _mm512_set1_epi8(old_rem);
//     uint64_t v = _mm512_cmpeq_epu8_mask(target, *pd);
//     assert(v & (1ULL << 63));

//     //todo: change to -> remove last.
//     conditional_remove(old_quot, old_rem, pd);
//     pd_add_50(quot, rem, pd);
//     set_overflow_bit(pd);
//     uint64_t res = (old_quot << 8u) | ((uint64_t) old_rem);
//     assert(res);
//std::cout << "pd_swap 2" << std::endl;
//     return res;
//     // return (old_quot << 8u) | old_rem;
// }


// inline uint8_t get_last_quot_in_pd_super_naive(const __m512i *pd) {
//     return count_ones_up_to_the_kth_zero(pd);
// }

// inline uint64_t get_last_quot_in_pd_naive_easy_case_helper(const __m512i *pd) {
//     assert(get_hi_meta_bits(pd) <= 32);
//     constexpr uint64_t h1_mask = (1ULL << (101ul - 64ul)) - 1ul;
//     const uint64_t h0 = _mm_extract_epi64(_mm512_castsi512_si128(*pd), 0);
//     const uint64_t h1 = _mm_extract_epi64(_mm512_castsi512_si128(*pd), 1) & h1_mask;

//     if (h1 == 0) {
//         uint64_t temp = (~h0) & (h0 >> 1);
//         assert(temp);
//         size_t first_one = _lzcnt_u64(h0);
//         size_t temps_first_one = _lzcnt_u64(temp);
//         assert(temps_first_one > first_one);
//         return temps_first_one - first_one;
//     }

//     uint64_t temp = (~h1) & (h1 >> 1);
//     if (temp == 0) {
//         size_t end = _tzcnt_u64(~h1);
//         size_t begin = _lzcnt_u64(~h0);
//         return end + begin;
//     }
//     assert(_lzcnt_u64(temp) > _lzcnt_u64(h1));
//     return _lzcnt_u64(temp) - _lzcnt_u64(h1);
// }
// inline uint64_t get_last_quot_in_pd_naive_easy_case(const __m512i *pd) {
//     // return get_last_quot_in_pd_naive_easy_case_helper(pd);
//     return QUOT_SIZE - get_last_quot_in_pd_naive_easy_case_helper(pd);
// }


// inline uint8_t get_last_quot_in_pd(const __m512i *pd) {
//     auto a = 50 - get_last_quot_in_pd_naive(pd);
//     auto b = get_last_quot_in_pd_super_naive(pd);
//     if (a != b) {
//         v_pd512::print_headers_masked(pd);
//     }
//     assert(a == b);
//     return 50 - get_last_quot_in_pd_naive(pd);
// }


// inline uint64_t decode_last_quot_in_pd_naive(const __m512i *pd) {
//     const uint64_t header_meta_bits = _mm_extract_epi8(_mm512_castsi512_si128(*pd), 12);
//     const uint64_t hi_meta_bits = _mm_extract_epi8(_mm512_castsi512_si128(*pd), 12) & (32 + 64);
//     if (hi_meta_bits == 0) {
//         uint64_t att = (header_meta_bits & 16) ? 49 : get_last_byte(pd);
//         if (header_meta_bits & 16) {
//             assert(!(header_meta_bits & 8));
//         }

//         // auto capacity = count_zeros_up_to_the_kth_one(pd, att + 1);
//         // assert(capacity == get_capacity(pd));
//std::cout << "h0" << std::endl;
//         // return count_ones_up_to_the_kth_zero(pd, capacity);
//     }
//     if (hi_meta_bits & 32) {
//         const uint64_t reduce_from_max_quot = header_meta_bits >> 6;
//         assert(reduce_from_max_quot < 4);
//std::cout << "h1" << std::endl;
//         return count_ones_up_to_the_kth_zero(pd);
//         // return count_ones_up_to_the_kth_zero(pd, QUOT_SIZE - reduce_from_max_quot);
//     }
//     if (hi_meta_bits == 64) {
//         const uint64_t reduce_from_max_quot = ((header_meta_bits >> 1) & 15) + 4;
//         assert(reduce_from_max_quot >= 4);
//         assert(reduce_from_max_quot <= 19);
//         // assert(QUOT_SIZE - reduce_from_max_quot > 0);
//std::cout << "h2" << std::endl;
//         return count_ones_up_to_the_kth_zero(pd);
//         // return count_ones_up_to_the_kth_zero(pd, QUOT_SIZE - reduce_from_max_quot);
//     }
//     if (hi_meta_bits == 128) {
//         const uint64_t reduce_from_max_quot = (header_meta_bits & 31) + 18;
//         assert(reduce_from_max_quot >= 18);
//         assert(reduce_from_max_quot <= 49);
//std::cout << "h3" << std::endl;
//         return count_ones_up_to_the_kth_zero(pd);
//         // return count_ones_up_to_the_kth_zero(pd, QUOT_SIZE - reduce_from_max_quot);
//     }
//     assert(false);

//     return count_ones_up_to_the_kth_zero(pd);
// }
