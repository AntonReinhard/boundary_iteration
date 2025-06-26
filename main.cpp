#include <cstddef> // for std::size_t
#include <iostream>

#include "boundary.hpp"
#include "common.hpp"
#include "iterator.hpp"
#include "volume.hpp"

template <typename T, typename NDArrayType> class FlatIterable {
    NDArrayType *array;

  public:
    constexpr FlatIterable(NDArrayType &arr) : array(&arr) {}

    constexpr auto begin() { return FlatIterator<T, NDArrayType>(array, 0); }

    constexpr auto end() {
        return FlatIterator<T, NDArrayType>(array, array->data.size());
    }
};

template <typename T, std::size_t... Dims>
constexpr auto makeMap(Volume<T, Dims...> &array) {
    return FlatIterable<T, Volume<T, Dims...>>(array);
}

constexpr auto make_array() {
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

// static_assert(sum(makeMap(make_array())) == 36);

int main() {
    auto a = make_array();
    for (int x : makeMap(a)) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    return 0;
}
