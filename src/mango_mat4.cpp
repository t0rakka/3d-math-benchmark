#include "prepare_test_data.h"

#include <benchmark/benchmark.h>
#include <mango/mango.hpp>

using float4x4 = mango::math::Matrix4x4;

static void mat4_add(benchmark::State& state) {
    const auto testData = prepare_mat4_test_data<float4x4>(state.range(0));

    float4x4 res(0.0f);

    for (auto _ : state) {
        benchmark::ClobberMemory();
        res = testData[0] + testData[1];
        benchmark::ClobberMemory();
    }
    benchmark::DoNotOptimize(res);
}

static void mat4_mult(benchmark::State& state) {
    const auto testData = prepare_mat4_test_data_mango<float4x4>(state.range(0));

    float4x4 res(0.0f);

    for (auto _ : state) {
        benchmark::ClobberMemory();
        res = testData[0] * testData[1];
        benchmark::ClobberMemory();
    }
    benchmark::DoNotOptimize(res);
}

static void mat4_mult_loop(benchmark::State& state) {
    const auto testData = prepare_mat4_test_data_mango<float4x4>(state.range(0));

    float4x4 res(0.0f);

    for (auto _ : state) {
        benchmark::ClobberMemory();
        for (const auto& mat : testData) {
            res = res * mat;
        }
        benchmark::ClobberMemory();
    }
    benchmark::DoNotOptimize(res);
}

static void mat4_mult_loop_accumulate(benchmark::State& state) {
    const auto testData = prepare_mat4_test_data_mango<float4x4>(state.range(0));

    float4x4 res(0.0f);

    for (auto _ : state) {
        benchmark::ClobberMemory();
        res = std::accumulate(
            testData.begin(), testData.end(), float4x4(1.0f), [](const float4x4& lhs, const float4x4& rhs) { return lhs * rhs; });
        benchmark::ClobberMemory();
    }
    benchmark::DoNotOptimize(res);
}

BENCHMARK(mat4_add)->Arg(2);
BENCHMARK(mat4_mult)->Arg(2);
BENCHMARK(mat4_mult_loop)->Arg(2)->Arg(8)->Arg(64)->Arg(1 << 10);
BENCHMARK(mat4_mult_loop_accumulate)->Arg(2)->Arg(8)->Arg(64)->Arg(1 << 10);

// Run the benchmark
BENCHMARK_MAIN();
