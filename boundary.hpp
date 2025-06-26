#pragma once

#include "common.hpp"

enum class BoundaryType { LOWER, MIDDLE, UPPER, OOB };

template <int Dim> struct BoundaryDirection {
    std::array<BoundaryType, Dim> boundaries;

    using difference_type = std::ptrdiff_t;
    using value_type = BoundaryDirection<Dim>;
    using pointer = value_type *;
    using reference = value_type &;

    constexpr reference operator*() const { return boundaries; }
    constexpr pointer operator->() { return &boundaries; }

    constexpr reference operator++() {
        int i = 0;
        while (i < Dim) {
            switch (boundaries[i]) {
            case BoundaryType::LOWER:
                boundaries[i] = BoundaryType::MIDDLE;
                i = Dim + 1;
                break;
            case BoundaryType::MIDDLE:
                boundaries[i] = BoundaryType::UPPER;
                i = Dim + 1;
                break;
            case BoundaryType::UPPER:
                boundaries[i] = BoundaryType::LOWER;
                i++;
                break;
            }
        }
        if (i == Dim) {
            boundaries = fill_array<BoundaryType, Dim>(BoundaryType::OOB);
        }
        return *this;
    }

    constexpr int value() { return 1; }

    constexpr friend bool operator==(const BoundaryDirection<Dim> &bd,
                                     const BoundaryDirection<Dim> &bd2) {
        return bd.boundaries == bd2.boundaries;
    }
    constexpr friend bool operator!=(const BoundaryDirection<Dim> &bd,
                                     const BoundaryDirection<Dim> &bd2) {
        return bd.boundaries != bd2.boundaries;
    }
};

template <int Dim> struct BoundaryDirectionsContainer {
    constexpr BoundaryDirection<Dim> begin() {
        return BoundaryDirection<Dim>{
            fill_array<BoundaryType, Dim>(BoundaryType::LOWER)};
    }
    constexpr BoundaryDirection<Dim> end() {
        return BoundaryDirection<Dim>{
            fill_array<BoundaryType, Dim>(BoundaryType::OOB)};
    }
    constexpr int length() { return ipow(3, Dim); }
};
