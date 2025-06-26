#include <cstddef> // for std::size_t
#include <iostream>
#include <type_traits>

#include "boundary.hpp"
#include "common.hpp"
#include "iterable.hpp"
#include "iterator.hpp"
#include "volume.hpp"

constexpr auto make_volume() {
    Volume<int, 2, 2, 2> a{};
    a[0][0][0] = 1;
    a[0][0][1] = 2;
    a[0][1][0] = 3;
    a[0][1][1] = 4;
    a[1][0][0] = 5;
    a[1][0][1] = 6;
    a[1][1][0] = 7;
    a[1][1][1] = 8;
    return a;
}

static_assert(sum(makeMap(make_volume())) == 36);

int main() {
    constexpr auto a = make_volume();
    for (const int &x : makeMap(a)) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    return 0;
}
