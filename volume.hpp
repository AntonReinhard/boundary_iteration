#pragma once

#include <array>
#include <cstddef>

template <typename T, std::size_t... Dims> struct Volume {};

template <typename T, std::size_t Dim> class Volume<T, Dim> {
  public:
    static constexpr std::size_t Dimensionality = 1;

    std::array<T, Dim> data;

    constexpr T &operator[](std::size_t i) { return data[i]; }
    constexpr const T &operator[](std::size_t i) const { return data[i]; }
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
};
