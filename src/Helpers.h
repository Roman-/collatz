#pragma once
#include <string>
#include <array>
#include <cstdint>

inline std::string format_round_number(std::uint64_t number) {
    const std::array<std::uint64_t, 5> rounds{
        1'000'000'000'000'000ULL, 1'000'000'000'000ULL, 1'000'000'000ULL, 1'000'000ULL, 1'000ULL
    };
    const std::array<const char*, 5> suffixes{"Q", "T", "B", "M", "k"};
    for (size_t i = 0; i < rounds.size(); ++i) {
        if (number % rounds[i] == 0) {
            return std::to_string(number / rounds[i]) + suffixes[i];
        }
    }
    return std::to_string(number);
}