#include "dd/Complex.hpp"

#include "dd/DDDefinitions.hpp"
#include "dd/RealNumber.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <complex>
#include <iomanip>
#include <istream>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>

namespace dd {
// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables,
// cppcoreguidelines-interfaces-global-init)
Complex Complex::zero{&constants::zero, &constants::zero};
Complex Complex::one{&constants::one, &constants::zero};
// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables,
// cppcoreguidelines-interfaces-global-init)

void Complex::setVal(const Complex& c) const noexcept {
  assert(!RealNumber::isNegativePointer(r));
  assert(!RealNumber::isNegativePointer(i));
  r->value = RealNumber::val(c.r);
  i->value = RealNumber::val(c.i);
}

bool Complex::exactlyZero() const noexcept {
  return RealNumber::exactlyZero(r) && RealNumber::exactlyZero(i);
}

bool Complex::exactlyOne() const noexcept {
  return RealNumber::exactlyOne(r) && RealNumber::exactlyZero(i);
}

bool Complex::approximatelyEquals(const Complex& c) const noexcept {
  return RealNumber::approximatelyEquals(r, c.r) &&
         RealNumber::approximatelyEquals(i, c.i);
}

bool Complex::approximatelyZero() const noexcept {
  return RealNumber::approximatelyZero(r) && RealNumber::approximatelyZero(i);
}

bool Complex::approximatelyOne() const noexcept {
  return RealNumber::approximatelyOne(r) && RealNumber::approximatelyZero(i);
}

bool Complex::operator==(const Complex& other) const noexcept {
  return r == other.r && i == other.i;
}

bool Complex::operator!=(const Complex& other) const noexcept {
  return !operator==(other);
}

std::pair<std::uint64_t, std::uint64_t>
Complex::getLowestFraction(const fp x, const std::uint64_t maxDenominator) {
  assert(x >= 0.);

  std::pair<std::uint64_t, std::uint64_t> lowerBound{0U, 1U};
  std::pair<std::uint64_t, std::uint64_t> upperBound{1U, 0U};

  while ((lowerBound.second <= maxDenominator) &&
         (upperBound.second <= maxDenominator)) {
    auto num = lowerBound.first + upperBound.first;
    auto den = lowerBound.second + upperBound.second;
    auto median = static_cast<fp>(num) / static_cast<fp>(den);
    if (std::abs(x - median) <= RealNumber::eps) {
      if (den <= maxDenominator) {
        return std::pair{num, den};
      }
      if (upperBound.second > lowerBound.second) {
        return upperBound;
      }
      return lowerBound;
    }
    if (x > median) {
      lowerBound = {num, den};
    } else {
      upperBound = {num, den};
    }
  }
  if (lowerBound.second > maxDenominator) {
    return upperBound;
  }
  return lowerBound;
}

std::string Complex::toString(bool formatted, int precision) const {
  return toString(static_cast<std::complex<fp>>(*this), formatted, precision);
}

void Complex::printFormatted(std::ostream& os, const fp num,
                             const bool imaginary) {
  if (RealNumber::approximatelyZero(num)) {
    os << (std::signbit(num) ? "-" : "+") << "0" << (imaginary ? "i" : "");
    return;
  }

  const auto absnum = std::abs(num);
  auto fraction = getLowestFraction(absnum);
  auto approx =
      static_cast<fp>(fraction.first) / static_cast<fp>(fraction.second);

  // suitable fraction a/b found
  if (const auto error = absnum - approx;
      RealNumber::approximatelyZero(error)) {
    const std::string sign = std::signbit(num) ? "-" : (imaginary ? "+" : "");

    if (fraction.first == 1U && fraction.second == 1U) {
      os << sign << (imaginary ? "i" : "1");
    } else if (fraction.second == 1U) {
      os << sign << fraction.first << (imaginary ? "i" : "");
    } else if (fraction.first == 1U) {
      os << sign << (imaginary ? "i" : "1") << "/" << fraction.second;
    } else {
      os << sign << fraction.first << (imaginary ? "i" : "") << "/"
         << fraction.second;
    }

    return;
  }

  const auto abssqrt = absnum / SQRT2_2;
  fraction = getLowestFraction(abssqrt);
  approx = static_cast<fp>(fraction.first) / static_cast<fp>(fraction.second);
  // suitable fraction a/(b * sqrt(2)) found
  if (const auto error = abssqrt - approx;
      RealNumber::approximatelyZero(error)) {
    const std::string sign = std::signbit(num) ? "-" : (imaginary ? "+" : "");

    if (fraction.first == 1U && fraction.second == 1U) {
      os << sign << (imaginary ? "i" : "1") << "/√2";
    } else if (fraction.second == 1U) {
      os << sign << fraction.first << (imaginary ? "i" : "") << "/√2";
    } else if (fraction.first == 1U) {
      os << sign << (imaginary ? "i" : "1") << "/(" << fraction.second << "√2)";
    } else {
      os << sign << fraction.first << (imaginary ? "i" : "") << "/("
         << fraction.second << "√2)";
    }
    return;
  }

  const auto abspi = absnum / PI;
  fraction = getLowestFraction(abspi);
  approx = static_cast<fp>(fraction.first) / static_cast<fp>(fraction.second);
  // suitable fraction a/b π found
  if (const auto error = abspi - approx; RealNumber::approximatelyZero(error)) {
    const std::string sign = std::signbit(num) ? "-" : (imaginary ? "+" : "");
    const std::string imagUnit = imaginary ? "i" : "";

    if (fraction.first == 1U && fraction.second == 1U) {
      os << sign << "π" << imagUnit;
    } else if (fraction.second == 1U) {
      os << sign << fraction.first << "π" << imagUnit;
    } else if (fraction.first == 1U) {
      os << sign << "π" << imagUnit << "/" << fraction.second;
    } else {
      os << sign << fraction.first << "π" << imagUnit << "/" << fraction.second;
    }
    return;
  }

  if (imaginary) { // default
    os << (std::signbit(num) ? "" : "+") << num << "i";
  } else {
    os << num;
  }
}

void Complex::fromString(const std::string& realStr, std::string imagStr,
                         std::complex<fp>& c) {
  c = 0;
  if (!realStr.empty()) {
    c.real(std::stod(realStr));
  }

  imagStr.erase(std::remove(imagStr.begin(), imagStr.end(), ' '),
                imagStr.end());
  imagStr.erase(std::remove(imagStr.begin(), imagStr.end(), 'i'),
                imagStr.end());
  if (imagStr == "+" || imagStr == "-") {
    imagStr = imagStr + "1";
  }
  if (!imagStr.empty()) {
    c.imag(std::stod(imagStr));
  }
}

std::string Complex::toString(const std::complex<fp>& c, const bool formatted,
                              const int precision) {
  if (std::abs(c) < RealNumber::eps) {
    return "0";
  }

  std::ostringstream ss{};
  if (precision >= 0) {
    ss << std::setprecision(precision);
  }

  if (!RealNumber::approximatelyZero(c.real())) {
    if (formatted) {
      printFormatted(ss, c.real());
    } else {
      ss << c.real();
    }
  }
  if (RealNumber::approximatelyZero(c.imag())) {
    return ss.str();
  }

  if (formatted) {
    if (RealNumber::approximatelyEquals(c.real(), c.imag())) {
      ss << "(1+i)";
      return ss.str();
    }
    if (RealNumber::approximatelyEquals(c.real(), -c.imag())) {
      ss << "(1-i)";
      return ss.str();
    }
    printFormatted(ss, c.imag(), true);
    return ss.str();
  }

  if (RealNumber::approximatelyZero(c.real())) {
    ss << c.imag();
  } else {
    if (c.imag() > 0.) {
      ss << "+";
    }
    ss << c.imag();
  }
  ss << "i";
  return ss.str();
}

void Complex::writeBinary(std::ostream& os) const {
  RealNumber::writeBinary(r, os);
  RealNumber::writeBinary(i, os);
}

void Complex::writeBinary(const std::complex<fp>& c, std::ostream& os) {
  RealNumber::writeBinary(c.real(), os);
  RealNumber::writeBinary(c.imag(), os);
}

void Complex::readBinary(std::istream& is, std::complex<fp>& c) {
  fp r{};
  is.read(reinterpret_cast<char*>(&r), sizeof(fp));
  fp i{};
  is.read(reinterpret_cast<char*>(&i), sizeof(fp));
  c = {r, i};
}

Complex::operator std::complex<fp>() const noexcept {
  return {RealNumber::val(r), RealNumber::val(i)};
}

std::ostream& operator<<(std::ostream& os, const Complex& c) {
  return os << c.toString();
}

std::complex<fp> operator/(const Complex& c1, const std::complex<fp>& c2) {
  return static_cast<std::complex<fp>>(c1) / c2;
}
std::complex<fp> operator/(const std::complex<fp>& c1, const Complex& c2) {
  return c1 / static_cast<std::complex<fp>>(c2);
}
std::complex<fp> operator/(const Complex& c1, const Complex& c2) {
  return static_cast<std::complex<fp>>(c1) / static_cast<std::complex<fp>>(c2);
}
} // namespace dd
