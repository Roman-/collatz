#pragma once

// The amount of bits reserved for storing the number must exceed num_bits for the maximum expected value at least twice.
// On 2023-11-03, the convergence of all numbers below 1.5 × 2^70 is verified by David Barina (xbarin02).
static constexpr size_t N_BITS = 410;

// report progress interval when searching. Bigger values give less often progress reports
static constexpr size_t PROGRESS_REPORT_INTERVAL = 1'000'000;

static constexpr int EXIT_CODE_SEARCH_FINISHED = 0;

static constexpr int EXIT_CODE_CYCLE_FOUND = 42;

// This may actually mean we've onto something
static constexpr int EXIT_CODE_OVERFLOW = 43;

static constexpr int EXIT_CODE_INTERNAL_ERROR = 255;