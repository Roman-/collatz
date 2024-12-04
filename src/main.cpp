#include <iostream>
#include <bitset>
#include <fstream>
#include <limits>
#include "Config.h"
#include "BigBinary.h"
#include "Helpers.h"

int main(int argc, const char* argv[]) {
    std::srand(time(nullptr));

    size_t max_tests = std::numeric_limits<size_t>::max();
    if (argc == 2) {
        max_tests = std::stoull(argv[1]);
    }
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
            if (num_steps > 500) {
                std::cout << "New max steps = " << max_steps_found << " for number " << num_with_max_num_steps << std::endl;
            }
        }
        if (test_index % PROGRESS_REPORT_INTERVAL == 0) {
            std::cout << "Checked " << format_round_number(test_index) << " numbers, max steps = " << max_steps_found << " for number " << num_with_max_num_steps << ".\nNow on number " << num << std::endl;
        }
        num.add_two_to_the_power_of(2); // add 4 to get number that ends with '11' (use the `n mod 4 = 1` optimization)
    }
    std::cout << "Checked " << max_tests << " numbers in range\n" << start_number << " -\n" << num << "\n\n";
    std::cout << "Max steps = " << max_steps_found << " for number\n" << num_with_max_num_steps << '\n';
    return EXIT_CODE_SEARCH_FINISHED;
}
