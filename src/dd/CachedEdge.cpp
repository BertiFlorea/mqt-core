#include "dd/CachedEdge.hpp"

#include "dd/Node.hpp"

#include <complex>

namespace dd {

template <typename Node>
CachedEdge<Node>::CachedEdge(Node* n, const Complex& c)
    : p(n), w(static_cast<std::complex<fp>>(c)) {}

template <typename Node>
bool CachedEdge<Node>::operator==(const CachedEdge& other) const {
  return p == other.p && Complex::approxiximatelyEquals(w, other.w);
}

template struct CachedEdge<vNode>;
template struct CachedEdge<mNode>;
template struct CachedEdge<dNode>;

} // namespace dd
