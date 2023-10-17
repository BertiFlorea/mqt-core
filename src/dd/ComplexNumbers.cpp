#include "dd/ComplexNumbers.hpp"

#include "dd/RealNumber.hpp"

#include <cassert>
#include <complex>

namespace dd {

void ComplexNumbers::setTolerance(fp tol) noexcept { RealNumber::eps = tol; }

void ComplexNumbers::add(Complex& r, const Complex& a,
                         const Complex& b) noexcept {
  assert(r != Complex::zero);
  assert(r != Complex::one);
  assert(r.r != a.i && "r.r and a.i point to the same entry!");
  assert(r.i != a.r && "r.i and a.r point to the same entry!");
  assert(r.r != b.i && "r.r and b.i point to the same entry!");
  assert(r.i != b.r && "r.i and b.r point to the same entry!");
  r.r->value = RealNumber::val(a.r) + RealNumber::val(b.r);
  r.i->value = RealNumber::val(a.i) + RealNumber::val(b.i);
}

void ComplexNumbers::sub(Complex& r, const Complex& a,
                         const Complex& b) noexcept {
  assert(r != Complex::zero);
  assert(r != Complex::one);
  assert(r.r != a.i && "r.r and a.i point to the same entry!");
  assert(r.i != a.r && "r.i and a.r point to the same entry!");
  assert(r.r != b.i && "r.r and b.i point to the same entry!");
  assert(r.i != b.r && "r.i and b.r point to the same entry!");
  r.r->value = RealNumber::val(a.r) - RealNumber::val(b.r);
  r.i->value = RealNumber::val(a.i) - RealNumber::val(b.i);
}

void ComplexNumbers::mul(Complex& r, const Complex& a,
                         const Complex& b) noexcept {
  assert(r != Complex::zero);
  assert(r != Complex::one);
  assert(r.r != a.i && "r.r and a.i point to the same entry!");
  assert(r.i != a.r && "r.i and a.r point to the same entry!");
  assert(r.r != b.i && "r.r and b.i point to the same entry!");
  assert(r.i != b.r && "r.i and b.r point to the same entry!");
  if (a.approximatelyOne()) {
    r.setVal(b);
  } else if (b.approximatelyOne()) {
    r.setVal(a);
  } else if (a.approximatelyZero() || b.approximatelyZero()) {
    r.r->value = 0.;
    r.i->value = 0.;
  } else {
    const auto ar = RealNumber::val(a.r);
    const auto ai = RealNumber::val(a.i);
    const auto br = RealNumber::val(b.r);
    const auto bi = RealNumber::val(b.i);

    r.r->value = ar * br - ai * bi;
    r.i->value = ar * bi + ai * br;
  }
}

void ComplexNumbers::div(Complex& r, const Complex& a,
                         const Complex& b) noexcept {
  assert(r != Complex::zero);
  assert(r != Complex::one);
  assert(r.r != a.i && "r.r and a.i point to the same entry!");
  assert(r.i != a.r && "r.i and a.r point to the same entry!");
  assert(r.r != b.i && "r.r and b.i point to the same entry!");
  assert(r.i != b.r && "r.i and b.r point to the same entry!");
  if (a.approximatelyEquals(b)) {
    r.r->value = 1.;
    r.i->value = 0.;
  } else if (b.approximatelyOne()) {
    r.setVal(a);
  } else {
    const auto ar = RealNumber::val(a.r);
    const auto ai = RealNumber::val(a.i);
    const auto br = RealNumber::val(b.r);
    const auto bi = RealNumber::val(b.i);

    const auto cmag = br * br + bi * bi;

    r.r->value = (ar * br + ai * bi) / cmag;
    r.i->value = (ai * br - ar * bi) / cmag;
  }
}

fp ComplexNumbers::mag2(const Complex& a) noexcept {
  return std::norm(static_cast<std::complex<fp>>(a));
}

fp ComplexNumbers::mag(const Complex& a) noexcept {
  return std::abs(static_cast<std::complex<fp>>(a));
}

fp ComplexNumbers::arg(const Complex& a) noexcept {
  return std::arg(static_cast<std::complex<fp>>(a));
}

Complex ComplexNumbers::conj(const Complex& a) noexcept {
  return {a.r, RealNumber::flipPointerSign(a.i)};
}

Complex ComplexNumbers::neg(const Complex& a) noexcept {
  return {RealNumber::flipPointerSign(a.r), RealNumber::flipPointerSign(a.i)};
}

Complex ComplexNumbers::lookup(const Complex& c) {
  if (isStaticComplex(c)) {
    return c;
  }

  const auto valr = RealNumber::val(c.r);
  const auto vali = RealNumber::val(c.i);
  return lookup(valr, vali);
}

void ComplexNumbers::incRef(const Complex& c) const noexcept {
  uniqueTable->incRef(c.r);
  uniqueTable->incRef(c.i);
}

void ComplexNumbers::decRef(const Complex& c) const noexcept {
  uniqueTable->decRef(c.r);
  uniqueTable->decRef(c.i);
}

Complex ComplexNumbers::lookup(const std::complex<fp>& c) {
  return lookup(c.real(), c.imag());
}

Complex ComplexNumbers::lookup(const fp r, const fp i) {
  return {uniqueTable->lookup(r), uniqueTable->lookup(i)};
}

std::size_t ComplexNumbers::realCount() const noexcept {
  return uniqueTable->getStats().numEntries;
}

} // namespace dd
