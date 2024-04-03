#pragma once

static constexpr size_t N_BITS = 160;

// report progress interval when searching. Bigger values give less often progress reports
static constexpr size_t PROGRESS_REPORT_INTERVAL = 500'000;

static constexpr int EXIT_CODE_CYCLE_FOUND = 42;

static constexpr int EXIT_CODE_OVERFLOW = 43;
