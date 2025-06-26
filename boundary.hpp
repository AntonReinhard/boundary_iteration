#pragma once

#include "common.hpp"

enum class BoundaryType { LOWER, MIDDLE, UPPER, OOB };

template <int Dim> struct BoundaryDirection {
    std::array<BoundaryType, Dim> data;

    constexpr auto operator<=>(BoundaryDirection<Dim> const &) const = default;
};

template <int Dim> struct BoundaryDirectionIter {
    BoundaryDirection<Dim> boundaries;

    using difference_type = std::ptrdiff_t;
    using value_type = BoundaryDirection<Dim>;
    using reference = value_type &;
    using const_reference = value_type const &;
    using pointer = value_type *;
    using const_pointer = value_type const *;

    constexpr const_reference &operator*() const { return boundaries; }
    constexpr reference &operator*() { return boundaries; }

    constexpr BoundaryDirectionIter<Dim> &operator++() {
        int i = 0;
        while (i < Dim) {
            switch (boundaries.data[i]) {
            case BoundaryType::LOWER:
                boundaries.data[i] = BoundaryType::MIDDLE;
                i = Dim + 1;
                break;
            case BoundaryType::MIDDLE:
                boundaries.data[i] = BoundaryType::UPPER;
                i = Dim + 1;
                break;
            case BoundaryType::UPPER:
                boundaries.data[i] = BoundaryType::LOWER;
                i++;
                break;
            }
        }
        if (i == Dim) {
            boundaries = {fill_array<BoundaryType, Dim>(BoundaryType::OOB)};
        }
        return *this;
    }

    constexpr auto
    operator<=>(BoundaryDirectionIter<Dim> const &) const = default;
};

template <int Dim>
std::ostream &operator<<(std::ostream &os, BoundaryDirection<Dim> const &bd) {
    for (auto &b : bd.data) {
        switch (b) {
        case BoundaryType::LOWER:
            os << 'v';
            break;
        case BoundaryType::MIDDLE:
            os << '-';
            break;
        case BoundaryType::UPPER:
            os << '^';
            break;
        case BoundaryType::OOB:
            os << 'x';
            break;
        }
    }
    return os;
}

template <int Dim> struct BoundaryDirectionsContainer {
    constexpr BoundaryDirectionIter<Dim> begin() const {
        return BoundaryDirectionIter<Dim>{
            fill_array<BoundaryType, Dim>(BoundaryType::LOWER)};
    }

    constexpr BoundaryDirectionIter<Dim> end() const {
        return BoundaryDirectionIter<Dim>{
            fill_array<BoundaryType, Dim>(BoundaryType::OOB)};
    }

    constexpr int length() { return ipow(3, Dim); }
};
