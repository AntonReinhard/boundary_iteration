#pragma once

#include <cstddef>
#include <utility>

// https://codereview.stackexchange.com/questions/250555/integer-power-c-function-with-base-and-exponent-parameters
constexpr int ipow(int base, int exponent) {
    if (exponent == 0) return 1;

    int result = ipow(base, exponent / 2);
    result *= result;

    if (exponent & 1) result *= base;

    return result;
}

template <typename T, std::size_t N, std::size_t... Is>
constexpr std::array<T, N> fill_array(const T &value,
                                      std::index_sequence<Is...>) {
    return {((void)Is, value)...}; // Expand `value` N times
}

template <typename T, std::size_t N>
constexpr std::array<T, N> fill_array(const T &value) {
    return fill_array<T, N>(value, std::make_index_sequence<N>{});
}

template <typename Iterable> constexpr int sum(Iterable iter) {
    int s = 0;
    auto begin = iter.begin();
    while (begin != iter.end()) {
        s += *begin;
        ++begin;
    }

    return s;
}
