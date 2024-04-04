#pragma once
#include <bitset>
#include "Config.h"

struct BigBinary {
    std::bitset<N_BITS> bits;

    void save_to_file(const std::string& path) const {
        try {
            std::ofstream file(path);
            file << *this;
        } catch (std::exception& e) {
            std::cerr << "failed to save to file: " << e.what() << std::endl;
        }
        std::cout << "saved result to " << path << std::endl;
    }

    void report_and_exit(const char* msg, size_t num_steps, int exit_code) const {
        const auto stars = std::string(120, '*');
        std::cout << stars <<  "\n" << msg << '\n' << stars << "\nNumber:\n" << *this;
        if (num_steps > 0) {
            std::cout << "\nnum_steps: " << num_steps;
        }
        std::cout << '\n' << stars << std::endl;
        const std::string filename = "collatz_result_" + std::to_string(rand() % 10000) + ".txt";
        save_to_file("/tmp/" + filename);
        save_to_file("$HOME/" + filename);
        exit(exit_code);
    }

    ssize_t first_non_zero_bit_index() const {
        ssize_t result = N_BITS - 1;
        while (result != 0 && bits.test(result) == 0) {
            --result;
        }
        return result;
    }

    /// Add (2^p) to this number. When p == 0, this is incrementing number (adding 1).
    void add_two_to_the_power_of(size_t p = 0) {
        for (size_t i = p; i < N_BITS; ++i) {
            if (bits[i] == 0) {
                bits[i] = 1;
                break;
            } else {
                bits[i] = 0; // Handle carry if bit is 1
            }
        }
    }

    void add(const BigBinary& b) {
        bool carry = false;

        for (int i = 0; i < N_BITS; ++i) {
            bool sum = bits[i] ^ b.bits[i] ^ carry; // Calculate sum of bits
            carry = (bits[i] & b.bits[i]) | (carry & (bits[i] ^ b.bits[i])); // Determine carry for the next bit
            bits[i] = sum;
        }
    }

    void multiplyByThreeAndAddOne() {
        // the only place that's checked for overflow
        if (bits.test(N_BITS - 1) || bits.test(N_BITS - 2)) {
            report_and_exit("Overflow when tried to apply 3x+1", 0, EXIT_CODE_OVERFLOW);
        }
        const auto original_bits = *this; // copy
        bits <<= 1; // 2 * num. Last bit is now zero
        bits.set(0); // 2 * num + 1. last bit is one
        add(original_bits); // 3 * num
    }

    // does 3x+1 and divides by 2 until it's odd again
    void apply_reduction() {
        multiplyByThreeAndAddOne();
        // divide by 2 until it's odd by removing all trailing zeros
        size_t num_zeros = 0;
        while (!bits.test(num_zeros)) {
            ++num_zeros;
        }
        bits >>= num_zeros;
    }

    bool operator>(const BigBinary& rhs) const {
        for (int i = N_BITS - 1; i >= 0; --i) {
            if (bits[i] != rhs.bits[i]) {
                return bits[i] > rhs.bits[i];
            }
        }
        return false; // Equal
    }

    size_t num_steps_to_converge_below_itself() const {
        if (!bits.test(0) || !bits.test(1)) {
            report_and_exit("You were trying work with a number that doesn't end with 11", 0, EXIT_CODE_INTERNAL_ERROR);
        }
        auto num = *this; // copy to perform operations on it
        num.apply_reduction();
        size_t num_steps = 1;
        while (num > *this) {
            num.apply_reduction();
            ++num_steps;
        }
        if (num.bits == bits) {
            report_and_exit("Found cycle", num_steps, EXIT_CODE_CYCLE_FOUND);
        }
        return num_steps;
    }

    static BigBinary generate_initial_number() {
        std::string s(N_BITS, '0');
        // fill the second half of the string with zeros and ones randomly
        for (size_t i = N_BITS / 2; i < N_BITS; ++i) {
            s[i] = rand() % 2 + '0';
        }
        // make sure we return odd number, as we don't check even numbers
        s[s.size() - 1] = '1';
        // make sure we return number that ends with 11, then we can iterate by 4.
        s[s.size() - 2] = '1';
        return BigBinary{.bits = decltype(bits)(s)};
    }

    template<typename OStream>
    friend OStream& operator<<(OStream& os, const BigBinary& b) {
        const auto first_non_zero = b.first_non_zero_bit_index();
        for (ssize_t i = first_non_zero; i >= 0; --i) {
            os << b.bits[i];
        }
        os << " (~2^" << first_non_zero << ')';
        return os;
    }
};
