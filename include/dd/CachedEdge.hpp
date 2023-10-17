#pragma once

#include "dd/Complex.hpp"
#include "dd/DDDefinitions.hpp"
#include "dd/RealNumber.hpp"

#include <complex>
#include <cstddef>
#include <functional>

namespace dd {

/**
 * @brief A DD node with a cached edge weight
 * @details Some DD operations create intermediate results that are not part of
 * the final result. To avoid storing these intermediate results in the unique
 * table, they are represented via cached numbers.
 * @tparam Node Type of the DD node
 */
template <typename Node> struct CachedEdge {
  Node* p{};
  std::complex<fp> w{};

  CachedEdge() = default;
  CachedEdge(Node* n, const std::complex<fp>& v) : p(n), w(v) {}
  CachedEdge(Node* n, const Complex& c);

  /// Comparing two DD edges with another involves comparing the respective
  /// pointers and checking whether the corresponding weights are "close enough"
  /// according to a given tolerance this notion of equivalence is chosen to
  /// counter floating point inaccuracies
  bool operator==(const CachedEdge& other) const;
  bool operator!=(const CachedEdge& other) const { return !operator==(other); }

  /**
   * @brief Create a terminal edge with the given weight.
   * @param w The weight of the terminal edge.
   * @return A terminal edge with the given weight.
   */
  [[nodiscard]] static constexpr CachedEdge
  terminal(const std::complex<fp>& w) {
    return CachedEdge{Node::getTerminal(), w};
  }

  /**
   * @brief Create a terminal edge with the given weight.
   * @param w The weight of the terminal edge.
   * @return A terminal edge with the given weight.
   */
  [[nodiscard]] static constexpr CachedEdge terminal(const Complex& w) {
    return terminal(static_cast<std::complex<fp>>(w));
  }

  [[nodiscard]] static constexpr CachedEdge zero() { return terminal(0.); }
  [[nodiscard]] static constexpr CachedEdge one() { return terminal(1.); }
};

} // namespace dd

namespace std {
template <class Node> struct hash<dd::CachedEdge<Node>> {
  /**
   * @brief Compute the hash value for the given cached edge.
   * @details The hash value is computed by combining the hash values of the
   * node pointer and the weight. The hash value of the node pointer is computed
   * using the murmur64 hash function on the pointer value. The hash value of
   * the weight is computed by scaling the real and imaginary part by the
   * tolerance of the real numbers, rounding the result to the nearest
   * integer and computing the hash value of the resulting pair of integers.
   * @param e The cached edge to compute the hash value for.
   * @returns The hash value for the given cached edge.
   * @note It is rather hard to define good hash functions for floating point
   * numbers. This hash function is not perfect, but it is fast and should
   * provide a good distribution of hash values. Furthermore, two floating point
   * numbers that are within the tolerance of the complex table will always
   * produce the same hash value.
   */
  constexpr std::size_t
  operator()(dd::CachedEdge<Node> const& e) const noexcept {
    const auto h1 = dd::murmur64(reinterpret_cast<std::size_t>(e.p));
    const auto h2 = dd::murmur64(
        static_cast<std::size_t>(std::round(e.w.real() / dd::RealNumber::eps)));
    const auto h3 = dd::murmur64(
        static_cast<std::size_t>(std::round(e.w.imag() / dd::RealNumber::eps)));
    return dd::combineHash(h1, dd::combineHash(h2, h3));
  }
};
} // namespace std
