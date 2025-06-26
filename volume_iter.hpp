// file for an iterator, iterating over a volume in an ordered way and providing
// the boundary direction
#pragma once

#include <iostream>
#include <optional>
#include <ranges>
#include <tuple>
#include <vector>

template <typename AIterable, typename BGenerator> class NestedPairIterator {
  public:
    using AIterator = decltype(std::begin(std::declval<AIterable &>()));
    using AValue = typename std::iterator_traits<AIterator>::value_type;
    using BIterable =
        decltype(std::declval<BGenerator>()(std::declval<AValue>()));
    using BIterator = decltype(std::begin(std::declval<BIterable>()));
    using BValue = typename std::iterator_traits<BIterator>::value_type;
    using ValueType = std::tuple<AValue, BValue>;

    class iterator {
      public:
        iterator(AIterator a_it, AIterator a_end, BGenerator &b_gen)
            : a_it(a_it), a_end(a_end), b_gen(b_gen) {
            advance_b();
        }

        ValueType operator*() const { return {*a_it, *b_it}; }

        iterator &operator++() {
            ++b_it;
            if (b_it == b_end) {
                ++a_it;
                advance_b();
            }
            return *this;
        }

        bool operator!=(const iterator &other) const {
            return a_it != other.a_it;
        }

      private:
        void advance_b() {
            while (a_it != a_end) {
                auto b_range = b_gen(*a_it);
                b_it = std::begin(b_range);
                b_end = std::end(b_range);
                if (b_it != b_end) break;
                ++a_it;
            }
        }

        AIterator a_it, a_end;
        BGenerator &b_gen;
        BIterator b_it, b_end;
    };

    NestedPairIterator(AIterable &a, BGenerator b_gen) : a(a), b_gen(b_gen) {}

    iterator begin() { return iterator(std::begin(a), std::end(a), b_gen); }

    iterator end() { return iterator(std::end(a), std::end(a), b_gen); }

  private:
    AIterable &a;
    BGenerator b_gen;
};

