#pragma once

#include <array>
#include <iostream>
#include <optional>
#include <ranges>
#include <tuple>
#include <vector>

#include "volume_iter.hpp"

template <typename ValueT, int Dim, std::size_t... Dims> class VolumeSection {
public:
    VolumeSection(BoundaryDirection<Dim> const &dir,
                  Volume<ValueT, Dims...> &vol)
        : dir(dir), vol(vol) {}

    BoundaryIter<ValueT, Dim, Dims...> begin() {
        return BoundaryIter<ValueT, Dim, Dims...>(dir, vol);
    }

    BoundaryIter<ValueT, Dim, Dims...> end() {
        auto bi = BoundaryIter<ValueT, Dim, Dims...>(dir, vol);
        bi.to_end();
        return bi;
    }

    constexpr auto
    operator<=>(VolumeSection<ValueT, Dim, Dims...> const &) const = default;

private:
    const BoundaryDirection<Dim> dir;
    Volume<ValueT, Dims...> &vol;
};
