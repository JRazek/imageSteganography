#pragma once

#include <bitset>
#include <cassert>
#include <fstream>
#include <istream>
#include <iterator>
#include <ostream>
#include <ranges>
#include <span>
#include <string>
#include <vector>

#include "header_utils.hpp"
#include "utils.hpp"

namespace jr::img {

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
      throw std::invalid_argument("invalid file contents!");

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
   * @param in_target - range od encoded data
   * @return information object
   */
  static auto create_metadata_for_decrypting(InputByteRange auto in_target)
      -> SteganographyImageMetadata {
    auto target_size = std::ranges::distance(in_target);
    auto message_size = bytes_to_little_endianness<std::size_t>(
        in_target | std::views::take(8));

    if (!target_size || !message_size)
      throw std::invalid_argument("invalid file contents!");

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
 * @brief - which of bits edited. Possible to implement with increasing
 * importance - this would allow bigger messages.
 */
constexpr auto steganography_bit = 0u;

/**
 * @brief - encodes message into output_it
 * @param in_target - input range, containing data to be encoded
 * @param in_message - input range, containing message to encrypt
 * @param output_it - output iterator to save to result
 */
auto encode_message(InputByteRange auto in_target,
                    InputByteRange auto in_message,
                    OutputByteIterator auto output_it) -> void {
  std::ranges::copy(in_target, output_it);

  auto encoding_data =
      SteganographyImageMetadata::create_metadata_for_encrypting(in_target,
                                                                 in_message);

  auto input_target_it = std::begin(in_target);

  for (auto i = 0u; i < 8u; i++, ++output_it, ++input_target_it)
    *output_it = encoding_data.header_bytes_[i];

  for (auto message_byte : in_message) {
    std::bitset<8> message_byte_bitset(message_byte);

    for (auto i = 0u; i < 8u; i++,
              std::ranges::advance(output_it, encoding_data.shift_),
              std::ranges::advance(input_target_it, encoding_data.shift_)) {
      auto bitmap_byte = std::bitset<8>(*input_target_it);
      bitmap_byte[steganography_bit] = message_byte_bitset[i];

      *output_it = bitmap_byte.to_ulong();
    }
  }
}

/**
 * @brief - decodes message into output_it
 * @param in_target - input range, containing data to be decoded
 * @param output_it - output iterator to save to result
 */
auto decode_message(InputByteRange auto in_target,
                    OutputByteIterator auto output_it) -> void {
  auto decoding_data =
      SteganographyImageMetadata::create_metadata_for_decrypting(in_target);

  auto in_it = std::begin(in_target);
  std::ranges::advance(in_it, 8);

  for (auto j = 0u; j < decoding_data.message_size_; j++, ++output_it) {
    std::bitset<8> data_byte;

    for (auto i = 0u; i < 8;
         i++, std::ranges::advance(in_it, decoding_data.shift_)) {
      auto bitmap_byte = std::bitset<8>(*in_it);
      data_byte[i] = bitmap_byte[steganography_bit];
    }

    *output_it = data_byte.to_ulong();
  }
}

/**
 * @tparam template specializations for different header types
 */
template <img::ImageFormat format>
struct ImageFormatHeader {};

template <>
struct ImageFormatHeader<img::ImageFormat::bmp> {
  using value_type = BMPHeader;
};

template <>
struct ImageFormatHeader<img::ImageFormat::ppm> {
  using value_type = PPMHeader;
};

/**
 *
 * @tparam format - format of the input
 * @param input_target - input range to encode
 * @param message - message to encode
 * @param
 *
 * @return
 */
template <img::ImageFormat format>
auto encode(InputByteRange auto input_target, InputByteRange auto message,
            OutputByteIterator auto output) -> void {
  std::vector<std::uint8_t> image_buffered(input_target);

  // since application could be expanded and more image types may be added,
  // conditional_t would not work here.
  using HeaderType = typename ImageFormatHeader<format>::value_type;

  HeaderType header{image_buffered};

  auto it = image_buffered.begin() + header.first_data_byte;

  auto span = std::span{it, image_buffered.end()};

  encode_message(span, message, it);

  std::ranges::copy(image_buffered, output);
}

template <img::ImageFormat format>
auto decode(InputByteRange auto input_target, OutputByteIterator auto output)
    -> void {
  std::vector<std::uint8_t> image_buffered(input_target);

  using HeaderType = typename ImageFormatHeader<format>::value_type;

  HeaderType header{image_buffered};

  auto span = std::span{image_buffered.begin() + header.first_data_byte,
                        image_buffered.end()};

  decode_message(span, output);
}

/**
 * @brief encodes message
 *
 * @tparam format - format of the file
 * @param input_path - path of file to encode
 * @param output_path - path of file to save the result (may be the same as
 * input)
 * @param message - message to encode
 */
template <ImageFormat format>
auto encode_format(std::string const& input_path,
                   std::string const& output_path,
                   std::vector<std::uint8_t> const& message) -> void {
  std::ifstream input_str(input_path, std::ios::binary);
  std::istreambuf_iterator<char> input_stream_it(input_str);

  std::vector<std::uint8_t> buffer_input(input_stream_it,
                                         std::istreambuf_iterator<char>());
  std::vector<std::uint8_t> buffer_output;

  encode<format>(buffer_input, message, std::back_inserter(buffer_output));

  std::ofstream output_str(output_path, std::ios::binary);
  std::ostreambuf_iterator<char> output_stream_it(output_str);

  std::ranges::copy(buffer_output, output_stream_it);
}

/**
 * @brief decodes message
 * @tparam format - format of the file
 * @param input_path - path of file to encode
 * @return message decoded
 */
template <ImageFormat format>
auto decode_format(std::string const& input_path) -> std::vector<std::uint8_t> {
  std::ifstream input_stream(input_path, std::ios::binary);

  std::istreambuf_iterator<char> input_stream_it(input_stream);
  std::vector<std::uint8_t> buffer_input(input_stream_it,
                                         std::istreambuf_iterator<char>());

  std::vector<std::uint8_t> result;
  decode<format>(buffer_input, std::back_inserter(result));

  return result;
}

template <ImageFormat format>
auto check_encodable_format(std::string const& input_path,
                            std::string const& output_path,
                            std::vector<std::uint8_t> const& message) -> bool {
  std::ifstream input_str(input_path, std::ios::binary);
  std::istreambuf_iterator<char> input_stream_it(input_str);

  std::vector<std::uint8_t> image_buffered(input_stream_it,
                                           std::istreambuf_iterator<char>());

  using HeaderType = typename ImageFormatHeader<format>::value_type;

  try {
    HeaderType header{image_buffered};

    std::vector<std::uint8_t> result;

    auto span = std::span{image_buffered.begin() + header.first_data_byte,
                          image_buffered.end()};

    SteganographyImageMetadata::create_metadata_for_encrypting(image_buffered,
                                                               message);
  } catch (...) {
    return false;
  }

  return true;
}

}  // namespace jr::img

