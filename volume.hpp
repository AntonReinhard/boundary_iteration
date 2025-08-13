#pragma once

#include <array>
#include <cstddef>

#include "boundary.hpp"

template <typename T, std::size_t... Dims> struct Volume {};

template <typename T, std::size_t Dim> class Volume<T, Dim> {
public:
    static constexpr std::size_t Dimensionality = 1;

    std::array<T, Dim> data;

    constexpr T &operator[](std::size_t i) { return data[i]; }
    constexpr const T &operator[](std::size_t i) const { return data[i]; }

    static constexpr const std::size_t size(std::size_t const dim) {
        if (dim == 0) {
            return Dim;
        }
        throw std::invalid_argument("invalid dimension");
    }

protected:
    template <std::size_t D>
    constexpr T &at_helper(std::array<std::size_t, D> const &i,
                           std::size_t ii) {
        return data[i[ii]];
    }
    template <typename TV, std::size_t... Dims2> friend class Volume;
};

template <typename T, std::size_t Dim, std::size_t... Rest>
class Volume<T, Dim, Rest...> {
public:
    static constexpr std::size_t Dimensionality =
        Volume<T, Rest...>::Dimensionality + 1;

    std::array<Volume<T, Rest...>, Dim> data;

    constexpr Volume<T, Rest...> &operator[](std::size_t i) { return data[i]; }
    constexpr const Volume<T, Rest...> &operator[](std::size_t i) const {
        return data[i];
    }
    constexpr T &operator[](std::array<std::size_t, Dimensionality> const &i) {
        return this->at_helper(i, 0);
    }
    static constexpr const std::size_t size(std::size_t const dim) {
        if (dim == 0) {
            return Dim;
        }
        return Volume<T, Rest...>::size(dim - 1);
    }

protected:
    template <std::size_t D>
    constexpr T &at_helper(std::array<std::size_t, D> const &i,
                           std::size_t ii) {
        return data[i[ii]].at_helper(i, ii + 1);
    }
    template <typename TV, std::size_t... Dims2> friend class Volume;
};

template <typename T, std::size_t... Dims>
consteval auto makeBoundaryDirIterator(Volume<T, Dims...> const &) {
    return BoundaryDirectionsContainer<Volume<T, Dims...>::Dimensionality>{};
}
