// file for an iterator, iterating over a volume in an ordered way and providing
// the boundary direction
#pragma once

#include <array>
#include <iostream>
#include <optional>
#include <ranges>
#include <tuple>
#include <vector>

template <typename ValueT, int Dim, std::size_t... Dims> class BoundaryIter {
public:
    constexpr BoundaryIter(BoundaryDirection<Dim> const &dir,
                           Volume<ValueT, Dims...> &vol)
        : dir(dir), vol(vol), indices() {
        for (std::size_t i = 0; i < Dim; ++i) {
            switch (dir.data[i]) {
            case BoundaryType::LOWER:
                indices[i] = 0;
                break;
            case BoundaryType::UPPER:
                indices[i] = Volume<ValueT, Dims...>::size(i) - 1;
                break;
            case BoundaryType::MIDDLE:
                indices[i] = 1;
                break;
            default:
                throw std::invalid_argument("invalid direction");
            }
        }
    }

    constexpr BoundaryIter<ValueT, Dim, Dims...> &operator++() {
        int i = 0;
        while (i < Dim) {
            switch (dir.data[i]) {
            case BoundaryType::LOWER:
                [[fallthrough]];
            case BoundaryType::UPPER:
                // don't iterate these indices
                ++i;
                continue;
            case BoundaryType::MIDDLE:
                ++(indices[i]);
                // skip last index, that's reserved for "upper"
                if (indices[i] >= Volume<ValueT, Dims...>::size(i) - 1) {
                    indices[i] = 1;
                    ++i;
                } else {
                    i = Dim + 1; // break and exit
                }
                break;
            }
        }
        if (i == Dim) {
            indices = {fill_array<std::size_t, Dim>(std::size_t(-1))};
        }
        return *this;
    }

    constexpr ValueT &operator*() { return vol[indices]; }

    constexpr bool
    operator==(BoundaryIter<ValueT, Dim, Dims...> const &other) const {
        return indices == other.indices;
    }

    constexpr void to_end() {
        for (auto i = 0; i < Dim; ++i) {
            indices[i] = -1;
        }
    }

private:
    const BoundaryDirection<Dim> dir;
    Volume<ValueT, Dims...> &vol;

    std::array<std::size_t, Dim> indices;
};
