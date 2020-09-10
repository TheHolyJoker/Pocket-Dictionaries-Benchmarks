#include "utils.hpp"

void set_pd(__m512i *pd, size_t quot_range, size_t capacity) {
    __m512i temp_pd = __m512i{(INT64_C(1) << 50) - 1, 0, 0, 0, 0, 0, 0, 0};
    for (size_t i = 0; i < capacity; i++) {
        uint64_t quot = rand() % quot_range;
        uint8_t rem = rand() & 255;
        bool res = pd512::pd_add_50(quot, rem, &temp_pd);
        assert(res);
        assert(pd512::pd_find_50(quot, rem, &temp_pd));
    }

    memcpy(pd, &temp_pd, 64);
}
