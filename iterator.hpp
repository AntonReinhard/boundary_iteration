#pragma once

#include <cstddef>
#include <iterator>

#include "volume.hpp"

template <typename T, typename TVolume> class FlatIterator;

// base case
template <typename T, std::size_t Dim> class FlatIterator<T, Volume<T, Dim>> {
    Volume<T, Dim> *volume = nullptr;
    std::size_t index = 0;

  public:
    constexpr FlatIterator() = default;

    constexpr FlatIterator(Volume<T, Dim> *vol, std::size_t idx)
        : volume(vol), index(idx) {}

    constexpr T &operator*() const { return volume->data[index]; }

    constexpr FlatIterator &operator++() {
        ++index;
        return *this;
    }

    constexpr bool operator==(const FlatIterator &other) const {
        return volume == other.volume && index == other.index;
    }

    constexpr bool operator!=(const FlatIterator &other) const {
        return !(*this == other);
    }
};

// recursive definition
template <typename T, std::size_t Dim, std::size_t... Rest>
class FlatIterator<T, Volume<T, Dim, Rest...>> {
    using VolumeType = Volume<T, Dim, Rest...>;
    using InnerVolume = Volume<T, Rest...>;
    using InnerIterator = FlatIterator<T, InnerVolume>;

    VolumeType *volume = nullptr;
    std::size_t outer_index = 0;
    InnerIterator inner;

  public:
    constexpr FlatIterator() = default;

    constexpr FlatIterator(VolumeType *vol, std::size_t idx)
        : volume(vol), outer_index(idx) {
        if (outer_index < Dim) {
            inner = InnerIterator(&volume->data[outer_index], 0);
            advanceToValid();
        }
    }

    constexpr T &operator*() const { return *inner; }

    constexpr FlatIterator &operator++() {
        ++inner;
        advanceToValid();
        return *this;
    }

    constexpr bool operator==(const FlatIterator &other) const {
        return volume == other.volume && outer_index == other.outer_index &&
               inner == other.inner;
    }

    constexpr bool operator!=(const FlatIterator &other) const {
        return !(*this == other);
    }

  private:
    constexpr void advanceToValid() {
        if (inner == FlatIterator<T, InnerVolume>(
                         &volume->data[outer_index],
                         volume->data[outer_index].data.size())) {
            ++outer_index;
            if (outer_index != Dim)
                inner = InnerIterator(&volume->data[outer_index], 0);
            else
                inner = InnerIterator();
        }
    }
};
