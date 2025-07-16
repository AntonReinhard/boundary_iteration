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
    BoundaryIter(BoundaryDirection<Dim> const &dir,
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
            std::cout << indices[i] << ", ";
        }
        std::cout << std::endl;
    }

private:
    const BoundaryDirection<Dim> dir;
    Volume<ValueT, Dims...> &vol;

    std::array<std::size_t, Dim> indices;
};
