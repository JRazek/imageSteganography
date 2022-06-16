#pragma once

#include <algorithm>
#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iterator>
#include <numeric>
#include <ranges>

namespace jr {


template<typename T>
concept InputByteRange =
std::ranges::input_range<T>
&&
std::same_as<std::ranges::range_value_t<T>, std::uint8_t>;

template<typename T>
concept OutputByteIterator = 
std::output_iterator<T, std::uint8_t>;

template<typename T>
concept InputByteIterator = 
std::input_iterator<T>
&&
std::same_as<typename T::value_type, std::uint8_t>;
/**
 * @brief encodes regardless of architecture to bytes array
 *
 * @tparam size - byte size of output
 * @param n number to encode
 *
 * @return array of size `size` in LSB order
 */
template <std::size_t size>
auto to_little_endianness_bytes(std::integral auto n)
    -> std::array<std::uint8_t, size> {
  std::array<std::uint8_t, size> bytes;

  for (auto& b : bytes) {
    b = n & 0xFFu;
    n >>= 8u;
  }

  if constexpr (std::endian::native != std::endian::little)
    std::ranges::reverse(bytes);

  return bytes;
}

auto to_little_endianness_bytes(std::integral auto n)
    -> std::array<std::uint8_t, sizeof(n)> {
  return to_little_endianness_bytes<sizeof(n)>(n);
}

/**
 * @brief decodes regardless of architecture to number represented by little
 * endianness bytes passed to function
 *
 * @tparam T
 * @tparam Range
 * @param range
 *
 * @return number decoded
 */
template <std::integral T, InputByteRange Range>
auto bytes_to_little_endianess(Range range) -> T requires std::ranges::bidirectional_range<Range> {
  auto n = T(0);

  if constexpr (std::endian::native == std::endian::little) {
    for (auto const b : range | std::views::reverse) {
      n <<= 8u;
      n |= b;
    }
  } else {
    for (auto const b : range) {
      n <<= 8u;
      n |= b;
    }
  }

  return n;
}

/**
 * @brief
 *
 * @param input_it - iterator with a bytes to decode
 * @param size - size of range
 *
 * @return
 */
template <std::integral T>
auto bytes_to_little_endianess(InputByteIterator auto input_it, std::size_t size) -> T {
  auto high = input_it;
  std::ranges::advance(high, size);
  return bytes_to_little_endianess<T>(std::ranges::subrange{input_it, high});
}

/**
 * @brief - last modification date of a file
 *
 * @param file - file path to check
 *
 * @return - modification date
 */
auto get_last_write_as_string(std::string const& file) -> std::string;

/**
 * @brief Added in order to use the same system for encoding and checking
 */
struct SteganographyImageMetadata {
  std::size_t const target_size_;
  std::uint8_t message_size_;
  std::array<std::uint8_t, 8> const header_bytes_;
  std::size_t const shift_;

  /**
   * @brief - creates information object for encrypting and decrypting from
   * input and message
   *
   * @tparam InputTargetRange
   * @tparam InputMessageRange
   * @param in_target - range of data for encoding
   * @param in_message - message to encode
   *
   * @return information object
   */
  static auto create_metadata_for_encrypting(InputByteRange auto in_target,
                                             InputByteRange auto in_message)
      -> SteganographyImageMetadata {
    auto target_size = std::ranges::distance(in_target);
    auto message_size = std::ranges::distance(in_message);

    if (!target_size || !message_size)
      std::invalid_argument("invalid file contents!");

    auto header_bytes = to_little_endianness_bytes<8>(message_size);
    auto shift = (target_size - 8) / (message_size * 8);

    if (!shift) throw std::invalid_argument("message too big!");

    return SteganographyImageMetadata(target_size, message_size, header_bytes,
                                      shift);
  }

  /**
   * @brief - creates information object for encrypting and decrypting from
   * encrypted message
   *
   * @tparam InputTargetRange
   * @param in_target - range od encoded data
   * @return information object
   */
  static auto create_metadata_for_decrypting(InputByteRange auto in_target)
      -> SteganographyImageMetadata{
    auto target_size = std::ranges::distance(in_target);
    auto message_size =
        bytes_to_little_endianess<std::size_t>(in_target | std::views::take(8));

    if (!target_size || !message_size)
      std::invalid_argument("invalid file contents!");

    auto header_bytes = to_little_endianness_bytes<8>(message_size);
    auto shift = (target_size - 8) / (message_size * 8);

    if (!shift) throw std::invalid_argument("message too big!");

    return SteganographyImageMetadata(target_size, message_size, header_bytes,
                                      shift);
  }

 private:
  SteganographyImageMetadata(std::size_t const target_size,
                             std::uint8_t message_size,
                             std::array<std::uint8_t, 8> const header_bytes,
                             std::size_t const shift)
      : target_size_(target_size),
        message_size_(message_size),
        header_bytes_(header_bytes),
        shift_(shift) {}
};

/**
 * @brief detects if target architecture is linux
 *
 */
constexpr auto is_linux_machine() noexcept -> bool {
#ifdef __linux
#define IS_LINUX_MACHINE true
#else
#define IS_LINUX_MACHINE false
#endif
  bool kIs_linux = IS_LINUX_MACHINE;
#undef IS_LINUX_MACHINE
  return kIs_linux;
}

class JRException : public std::exception {
 public:
  JRException(const std::string& message) : message_(message){};
  const char* what() const noexcept override { return message_.c_str(); }

 private:
  std::string message_;
};



}  // namespace jr

