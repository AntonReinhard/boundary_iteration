#include <cstddef> // for std::size_t
#include <iostream>
#include <sstream>
#include <type_traits>

#include "boundary.hpp"
#include "common.hpp"
#include "iterable.hpp"
#include "iterator.hpp"
#include "volume.hpp"
#include "volume_iter.hpp"
#include "volume_section.hpp"

template <int SIZE> constexpr auto make_volume() {
    Volume<int, SIZE, SIZE, SIZE> a{};
    int c = 0;
    for (auto &x : makeMap(a)) {
        x = c;
        ++c;
    }
    return a;
}

constexpr int TESTDIM = 3;
constexpr int TESTSIZE = 5;
static_assert(sum(makeMap(make_volume<TESTSIZE>())) ==
              (ipow(TESTSIZE, TESTDIM) - 1) * ipow(TESTSIZE, TESTDIM) / 2);

constexpr auto get_sum(auto vol) {
    constexpr auto bd = makeBoundaryDirIterator(vol);

    int sum = 0;

    // this for loop might get unrolled because bd is constexpr
    for (auto const &x : bd) {
        auto volume_section = VolumeSection(x, vol);
        for (auto const &el : volume_section) {
            sum += el;
        }
    }
    return sum;
}

int main() {
    // it's possible to make a completely constexpr volume, filled with values:
    constexpr auto vol1 = make_volume<5>();

    // we can iterate through this with makeMap
    for (const int &x : makeMap(vol1)) {
        std::cout << x << ' ';
    }
    std::cout << std::endl;

    // get_sum can evaluate at constexpr time
    constexpr auto sum = get_sum(make_volume<7>());
    std::cout << sum << std::endl;

    // even a mutable volume can make a constexpr BoundaryDirIterator
    // because it's only dependent on its type information
    auto vol2 = make_volume<2>();
    vol2[1][1][1] = 5;
    constexpr auto bd = makeBoundaryDirIterator(vol2);

    // get_sum can of course not evaluate at constexpr time anymore for vol2
    // but the outer "direction" loop could be unrolled by the compiler
    std::cout << get_sum(vol2) << std::endl;

    return 0;
}
