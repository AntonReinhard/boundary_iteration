#pragma once

#include "iterator.hpp"
#include "volume.hpp"

template <typename T, typename VolumeType, bool IsConst> class FlatIterable {
    using RawVolume = std::remove_const_t<VolumeType>;
    using VolumePtr =
        std::conditional_t<IsConst, const RawVolume *, RawVolume *>;

    VolumePtr volume;

  public:
    constexpr FlatIterable(const VolumeType &v)
        requires(IsConst)
        : volume(&v) {}

    constexpr FlatIterable(VolumeType &v)
        requires(!IsConst)
        : volume(&v) {}

    constexpr auto begin() const {
        return FlatIterator<T, VolumeType, IsConst>(volume, 0);
    }

    constexpr auto end() const {
        return FlatIterator<T, VolumeType, IsConst>(volume,
                                                    volume->data.size());
    }
};

// Deduction guide for const Volume
template <typename T, std::size_t... Dims>
FlatIterable(const Volume<T, Dims...> &)
    -> FlatIterable<T, Volume<T, Dims...>, true>;

// Deduction guide for non-const Volume
template <typename T, std::size_t... Dims>
FlatIterable(Volume<T, Dims...> &)
    -> FlatIterable<T, Volume<T, Dims...>, false>;

template <typename T, std::size_t... Dims>
constexpr auto makeMap(Volume<T, Dims...> &v) {
    return FlatIterable(v);
}

template <typename T, std::size_t... Dims>
constexpr auto makeMap(const Volume<T, Dims...> &v) {
    return FlatIterable(v);
}
