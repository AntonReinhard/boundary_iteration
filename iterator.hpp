#pragma once

#include <cstddef>
#include <iterator>

#include "volume.hpp"

template <typename T, typename TVolume, bool IsConst> class FlatIterator;

// base case
template <typename T, std::size_t Dim, bool IsConst>
class FlatIterator<T, Volume<T, Dim>, IsConst> {
    using VolumePtr =
        std::conditional_t<IsConst, const Volume<T, Dim> *, Volume<T, Dim> *>;

    VolumePtr volume = nullptr;
    std::size_t index = 0;

public:
    constexpr FlatIterator() = default;

    constexpr FlatIterator(Volume<T, Dim> const *vol, std::size_t idx)
        requires(IsConst)
        : volume(vol), index(idx) {}

    constexpr FlatIterator(Volume<T, Dim> *vol, std::size_t idx)
        requires(!IsConst)
        : volume(vol), index(idx) {}

    constexpr T const &operator*() const
        requires(IsConst)
    {
        return volume->data[index];
    }
    constexpr T &operator*()
        requires(!IsConst)
    {
        return volume->data[index];
    }

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
template <typename T, std::size_t Dim, std::size_t... Rest, bool IsConst>
class FlatIterator<T, Volume<T, Dim, Rest...>, IsConst> {
    using VolumeType = Volume<T, Dim, Rest...>;
    using RawVolume = std::remove_const_t<VolumeType>;
    using VolumePtr =
        std::conditional_t<IsConst, const RawVolume *, RawVolume *>;
    using InnerVolume = Volume<T, Rest...>;
    using InnerIterator = FlatIterator<T, InnerVolume, IsConst>;
    using Reference = std::conditional_t<IsConst, const T &, T &>;

    VolumePtr volume = nullptr;
    std::size_t outer_index = 0;
    InnerIterator inner;

public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using reference = Reference;
    using const_reference = reference const;

    constexpr FlatIterator() = default;

    constexpr FlatIterator(VolumePtr const vol, std::size_t idx)
        requires(IsConst)
        : volume(vol), outer_index(idx) {
        if (outer_index < Dim) {
            inner = InnerIterator(&volume->data[outer_index], 0);
            advanceToValid();
        }
    }

    constexpr FlatIterator(VolumePtr vol, std::size_t idx)
        requires(!IsConst)
        : volume(vol), outer_index(idx) {
        if (outer_index < Dim) {
            inner = InnerIterator(&volume->data[outer_index], 0);
            advanceToValid();
        }
    }

    constexpr const_reference operator*() const
        requires(IsConst)
    {
        return *inner;
    }

    constexpr reference operator*()
        requires(!IsConst)
    {
        return *inner;
    }

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
        if (inner == FlatIterator<T, InnerVolume, IsConst>(
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
