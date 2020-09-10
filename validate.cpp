#include "validate.hpp"



void valid_find() {
    const uint64_t quot_range = 50;
    __m512i x;
    bool y;
    set_pd(&x, quot_range, 51);

    for (uint64_t i = 0; i < 1'000'000; ++i) {
        y ^= pd512::pd_find_50(reduce16(rand(), quot_range), rand() & 255, &x);
    }
}



int main(int argc, char **argv)
{
    std::cout << "Validate2!" << std::endl;
    valid_find();
    return 0;
}
