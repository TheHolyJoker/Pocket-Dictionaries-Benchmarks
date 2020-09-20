#include "v_tests.hpp"


// bool print_pass() {
//     std::cout << "Passed tests!" << std::endl;
//     return true;
// }

int main(int argc, char **argv) {
    std::cout << "Hello1" << std::endl;
    rand_test1();
    for (size_t i = 0; i < 1024; i++) {
        std::cout << "i: " << i << std::endl;
        rand_test1();
    }

    std::cout << "end." << std::endl;
    return 0;
}
