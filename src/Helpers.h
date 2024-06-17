#pragma once
#include <string>
#include <array>

std::string format_round_number(uint64_t number) {
    std::array rounds{1'000'000'000ULL, 1'000'000ULL, 1'000ULL};
    std::array suffixes{"B", "M", "k"};
    for (size_t i = 0; i < rounds.size(); ++i) {
        if (number % rounds[i] == 0) {
            return std::to_string(number / rounds[i]) + suffixes[i];
        }
    }
    return std::to_string(number);
}