#include <cstddef> // for std::size_t
#include <iostream>
#include <type_traits>

#include "boundary.hpp"
#include "common.hpp"
#include "iterable.hpp"
#include "iterator.hpp"
#include "volume.hpp"

constexpr int SIZE = 5;

constexpr auto make_volume() {
    Volume<int, SIZE, SIZE, SIZE> a{};
    for (auto &x : makeMap(a)) {
        x = 1;
    }
    return a;
}

static_assert(sum(makeMap(make_volume())) == ipow(SIZE, 3));

int main() {
    constexpr auto a = make_volume();
    for (const int &x : makeMap(a)) {
        std::cout << x << ' ';
    }
    std::cout << std::endl;

    constexpr auto bd = makeBoundaryIterator(a);

    for (auto const &x : bd) {
        std::cout << x << ' ';
    }
    std::cout << std::endl;

    return 0;
}
