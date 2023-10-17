#include "dd/CachedEdge.hpp"

#include "dd/Complex.hpp"
#include "dd/Node.hpp"
#include "dd/RealNumber.hpp"

#include <complex>

namespace dd {

template <typename Node>
CachedEdge<Node>::CachedEdge(Node* n, const Complex& c)
    : p(n), w(static_cast<std::complex<fp>>(c)) {}

template <typename Node>
bool CachedEdge<Node>::operator==(const CachedEdge& other) const {
  return p == other.p && std::abs(w - other.w) < RealNumber::eps;
}

template struct CachedEdge<vNode>;
template struct CachedEdge<mNode>;
template struct CachedEdge<dNode>;

} // namespace dd

namespace std {
template <class Node>
std::size_t hash<dd::CachedEdge<Node>>::operator()(
    const dd::CachedEdge<Node>& e) const noexcept {
  const auto h1 = dd::murmur64(reinterpret_cast<std::size_t>(e.p));
  const auto h2 = dd::murmur64(
      static_cast<std::size_t>(std::round(e.w.real() / dd::RealNumber::eps)));
  const auto h3 = dd::murmur64(
      static_cast<std::size_t>(std::round(e.w.imag() / dd::RealNumber::eps)));
  return dd::combineHash(h1, dd::combineHash(h2, h3));
}

template struct hash<dd::CachedEdge<dd::vNode>>;
template struct hash<dd::CachedEdge<dd::mNode>>;
template struct hash<dd::CachedEdge<dd::dNode>>;
} // namespace std
