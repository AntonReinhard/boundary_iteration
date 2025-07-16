#include <cstddef> // for std::size_t
#include <iostream>
#include <type_traits>

#include "boundary.hpp"
#include "common.hpp"
#include "iterable.hpp"
#include "iterator.hpp"
#include "volume.hpp"
#include "volume_iter.hpp"
#include "volume_section.hpp"

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

    constexpr auto bd = makeBoundaryDirIterator(a);

    auto b = make_volume();
    for (auto const &x : bd) {
        std::cout << x << ": ";

        auto volume_section = VolumeSection(x, b);

        for (auto const &el : volume_section) {
            std::cout << el << ", ";
        }

        std::cout << std::endl;
    }

    return 0;
}
