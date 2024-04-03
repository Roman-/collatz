#include <iostream>
#include <bitset>
#include <fstream>
#include <limits>
#include "BigBinary.h"

int main(int argc, const char* argv[]) {
    size_t max_tests = std::numeric_limits<size_t>::max();
    if (argc == 2) {
        max_tests = std::stoull(argv[1]);
    }
    const size_t report_interval = 500'000;
    const BigBinary start_number = BigBinary::generate_initial_number();
    std::cout << "Running " << max_tests << " tests on binary numbers with " << N_BITS << " bits, starting with "
        << start_number << std::endl;

    BigBinary num = start_number;
    size_t max_steps_found = 0;
    BigBinary num_with_max_num_steps = {.bits = 0};

    for (size_t test_index = 1; test_index <= max_tests; ++test_index) {
        const size_t num_steps = num.num_steps_to_converge_below_itself();
        if (num_steps > max_steps_found) {
            max_steps_found = num_steps;
            num_with_max_num_steps = num;
        }
        if (test_index % report_interval == 0) {
            std::cout << "Checked " << test_index << " numbers, max steps = " << max_steps_found << " for number " << num_with_max_num_steps << std::endl;
        }
        num.add_two();
    }
    std::cout << "Checked " << max_tests << " numbers in range\n" << start_number << " -\n" << num << "\n\n";
    std::cout << "Max steps = " << max_steps_found << " for number\n" << num_with_max_num_steps << std::endl;
    return 0;
}
