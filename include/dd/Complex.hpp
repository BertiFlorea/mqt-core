#pragma once

#include "dd/DDDefinitions.hpp"
#include "dd/RealNumber.hpp"

#include <complex>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iostream>
#include <string>
#include <utility>

namespace dd {

struct RealNumber;

/// A complex number represented by two pointers to compute table entries.
struct Complex {
  /// Compute table entry for the real part.
  RealNumber* r;
  /// Compute table entry for the imaginary part.
  RealNumber* i;

  /**
   * @brief The static constant for the complex number zero.
   * @return A complex number with real and imaginary part equal to zero.
   */
  static constexpr Complex zero() noexcept {
    return {&constants::zero, &constants::zero};
  }

  /**
   * @brief The static constant for the complex number one.
   * @return A complex number with real part equal to one and imaginary part
   * equal to zero.
   */
  static constexpr Complex one() noexcept {
    return {&constants::one, &constants::zero};
  }

  /**
   * @brief Set the value based on the given complex number.
   * @param c The value to set.
   */
  void setVal(const Complex& c) const noexcept;

  /**
   * @brief Check whether the complex number is exactly equal to zero.
   * @returns True if the complex number is exactly equal to zero, false
   * otherwise.
   * @see RealNumber::exactlyZero
   */
  [[nodiscard]] constexpr bool exactlyZero() const noexcept {
    return RealNumber::exactlyZero(r) && RealNumber::exactlyZero(i);
  }

  /**
   * @brief Check whether the complex number is exactly equal to one.
   * @returns True if the complex number is exactly equal to one, false
   * otherwise.
   * @see RealNumber::exactlyOne
   * @see RealNumber::exactlyZero
   */
  [[nodiscard]] constexpr bool exactlyOne() const noexcept {
    return RealNumber::exactlyOne(r) && RealNumber::exactlyZero(i);
  }

  /**
   * @brief Check whether the complex number is approximately equal to the
   * given complex number.
   * @param c The complex number to compare to.
   * @returns True if the complex number is approximately equal to the given
   * complex number, false otherwise.
   * @see RealNumber::approximatelyEquals
   */
  [[nodiscard]] bool approximatelyEquals(const Complex& c) const noexcept;

  /**
   * @brief Check whether two complex numbers are approximately equal
   * @param c1 The first complex number
   * @param c2 The second complex number
   * @return True if the complex numbers are approximately equal, false
   * otherwise.
   */
  [[nodiscard]] static bool
  approxiximatelyEquals(const std::complex<fp>& c1,
                        const std::complex<fp>& c2) noexcept;

  /**
   * @brief Check whether the complex number is approximately equal to zero.
   * @returns True if the complex number is approximately equal to zero, false
   * otherwise.
   * @see RealNumber::approximatelyZero
   */
  [[nodiscard]] bool approximatelyZero() const noexcept;

  /**
   * @brief Check whether the complex number is approximately equal to zero.
   * @param c The complex number to compare to.
   * @return True if the complex number is approximately equal to zero, false
   * otherwise.
   * @see RealNumber::approximatelyZero
   */
  [[nodiscard]] static bool
  approximatelyZero(const std::complex<fp>& c) noexcept;

  /**
   * @brief Check whether the complex number is approximately equal to one.
   * @returns True if the complex number is approximately equal to one, false
   * otherwise.
   * @see RealNumber::approximatelyOne
   * @see RealNumber::approximatelyZero
   */
  [[nodiscard]] bool approximatelyOne() const noexcept;

  /**
   * @brief Check for exact equality.
   * @param other The complex number to compare to.
   * @returns True if the complex numbers are exactly equal, false otherwise.
   * @note Boils down to a pointer comparison.
   */
  [[nodiscard]] bool operator==(const Complex& other) const noexcept;

  /// @see operator==
  [[nodiscard]] bool operator!=(const Complex& other) const noexcept;

  /**
   * @brief Get the closest fraction to the given number.
   * @param x The number to approximate.
   * @param maxDenominator The maximum denominator to use.
   * @returns The closest fraction to the given number as a pair of numerator
   * and denominator.
   */
  static std::pair<std::uint64_t, std::uint64_t>
  getLowestFraction(fp x, std::uint64_t maxDenominator = 1U << 10);

  /**
   * @brief Pretty print the given real number to the given output stream.
   * @param os The output stream to write to.
   * @param num The number to print.
   * @param imaginary Whether the number is imaginary.
   */
  static void printFormatted(std::ostream& os, fp num, bool imaginary = false);

  /**
   * @brief Convert the complex number to a string.
   * @param formatted Whether to apply special formatting to the numbers.
   * @param precision The precision to use for the numbers.
   * @returns The string representation of the complex number.
   */
  [[nodiscard]] std::string toString(bool formatted = true,
                                     int precision = -1) const;

  /**
   * @brief Convert the complex number to a string.
   * @param c The complex number to convert.
   * @param formatted Whether to apply special formatting to the numbers.
   * @param precision The precision to use for the numbers.
   * @return The string representation of the complex number.
   */
  [[nodiscard]] static std::string toString(const std::complex<fp>& c,
                                            bool formatted = true,
                                            int precision = -1);

  /**
   * @brief Construct a complex number from a string.
   * @param realStr The string representation of the real part.
   * @param imagStr The string representation of the imaginary part.
   * @param c The complex number to write into.
   */
  static void fromString(const std::string& realStr, std::string imagStr,
                         std::complex<fp>& c);

  /**
   * @brief Write the complex number to a binary stream.
   * @param os The output stream to write to.
   * @see RealNumber::writeBinary
   */
  void writeBinary(std::ostream& os) const;

  /**
   * @brief Write the complex number to a binary stream.
   * @param c The complex number to write.
   * @param os The output stream to write to.
   */
  static void writeBinary(const std::complex<fp>& c, std::ostream& os);

  /**
   * @brief Read the complex number from a binary stream.
   * @param is The input stream to read from.
   * @param c The complex number to read into.
   */
  static void readBinary(std::istream& is, std::complex<fp>& c);

  /**
   * @brief Convert the Complex number to an std::complex<fp>.
   * @returns The std::complex<fp> representation of the Complex number.
   */
  [[nodiscard]] explicit operator std::complex<fp>() const noexcept;
};

/**
 * @brief Print a complex number to a stream.
 * @param os The output stream to write to.
 * @param c The complex number to print.
 * @returns The output stream.
 */
std::ostream& operator<<(std::ostream& os, const Complex& c);

std::complex<fp> operator*(const Complex& c1, const std::complex<fp>& c2);
std::complex<fp> operator*(const std::complex<fp>& c1, const Complex& c2);
std::complex<fp> operator*(const Complex& c1, const Complex& c2);

std::complex<fp> operator/(const Complex& c1, const std::complex<fp>& c2);
std::complex<fp> operator/(const std::complex<fp>& c1, const Complex& c2);
std::complex<fp> operator/(const Complex& c1, const Complex& c2);

} // namespace dd

namespace std {
/// Hash function for complex numbers.
template <> struct hash<dd::Complex> {
  /**
   * @brief Compute the hash value for a complex number.
   * @details Reinterprets the pointers to the real and imaginary part as
   * integers and computes the hash value for those. Afterwards, the two hash
   * values are combined.
   * @param c The complex number to compute the hash value for.
   * @returns The hash value.
   * @see dd::murmur64
   * @see dd::combineHash
   */
  std::size_t operator()(dd::Complex const& c) const noexcept {
    const auto h1 = dd::murmur64(reinterpret_cast<std::size_t>(c.r));
    const auto h2 = dd::murmur64(reinterpret_cast<std::size_t>(c.i));
    return dd::combineHash(h1, h2);
  }
};
} // namespace std
