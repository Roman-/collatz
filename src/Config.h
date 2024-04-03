#pragma once

static constexpr size_t N_BITS = 170;

// report progress interval when searching. Bigger values give less often progress reports
static constexpr size_t PROGRESS_REPORT_INTERVAL = 1'000'000;

static constexpr int EXIT_CODE_SEARCH_FINISHED = 0;

static constexpr int EXIT_CODE_CYCLE_FOUND = 42;

// This may actually mean we've onto something
static constexpr int EXIT_CODE_OVERFLOW = 43;
