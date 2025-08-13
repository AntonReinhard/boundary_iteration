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

constexpr int _SIZE = 5;

template <int SIZE> constexpr auto make_volume() {
    Volume<int, SIZE, SIZE, SIZE> a{};
    int c = 0;
    for (auto &x : makeMap(a)) {
        x = c;
        ++c;
    }
    return a;
}

static_assert(sum(makeMap(make_volume<_SIZE>())) ==
              (ipow(_SIZE, 3) - 1) * ipow(_SIZE, 3) / 2);

constexpr auto get_sum(auto vol) {
    constexpr auto bd = makeBoundaryDirIterator(vol);

    int sum = 0;
    for (auto const &x : bd) {
        auto volume_section = VolumeSection(x, vol);
        for (auto const &el : volume_section) {
            sum += el;
        }
    }
    return sum;
}

int main() {
    constexpr auto a = make_volume<_SIZE>();
    for (const int &x : makeMap(a)) {
        std::cout << x << ' ';
    }
    std::cout << std::endl;

    constexpr auto sum = get_sum(make_volume<7>());

    std::cout << sum << std::endl;

    return 0;
}
