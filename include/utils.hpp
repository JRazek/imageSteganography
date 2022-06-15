#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iterator>
#include <numeric>
#include <algorithm>
#include <ranges>
#include <bit>
#include <iterator>


namespace jr{

template<std::size_t size>
auto to_little_endianness_bytes(std::integral auto n) -> std::array<std::uint8_t, size> {
	 std::array<std::uint8_t, size> bytes;

	for(auto& b : bytes){
		b=n&0xFFu;
		n>>=8u;
	}

	if constexpr (std::endian::native!=std::endian::little) std::ranges::reverse(bytes);

    return bytes;
}

auto to_little_endianness_bytes(std::integral auto n) -> std::array<std::uint8_t, sizeof(n)> { return to_little_endianness_bytes<sizeof(n)>(n); }


template<typename T,typename Range>
auto bytes_to_little_endianess(Range range) -> T 
requires
std::integral<T>
&&
std::ranges::input_range<Range>
&&
std::ranges::bidirectional_range<Range>
{
	auto n=T(0);

	if constexpr (std::endian::native==std::endian::little){
		for(auto const b : range | std::views::reverse){
			n<<=8u;
			n|=b;
		}
	}else{
		for(auto const b : range){
			n<<=8u;
			n|=b;
		}
	}

	return n;
}

template<
std::integral T,
std::input_iterator Iterator 
>
auto bytes_to_little_endianess(Iterator low, std::size_t size) -> T {
	auto high=low; std::ranges::advance(high, size);
	return bytes_to_little_endianess<T>(std::ranges::subrange{low, high}); 
}

auto get_last_write_as_string(std::string const& file) -> std::string;

struct SteganographyImageMetadata{
	std::size_t const target_size_;
	std::uint8_t message_size_;
	std::array<std::uint8_t, 8> const header_bytes_;
	std::size_t const shift_;

	template<typename InputTargetRange, typename InputMessageRange>
	static auto create_metadata_for_encrypting(InputTargetRange in_target, InputMessageRange in_message) -> SteganographyImageMetadata
	requires 
	std::ranges::input_range<InputTargetRange>
	&&
	std::ranges::input_range<InputMessageRange>
	&&
	std::same_as<typename InputTargetRange::iterator::value_type, std::uint8_t>
	&&
	std::same_as<typename InputMessageRange::iterator::value_type, std::uint8_t>
	{
		auto target_size=std::ranges::distance(in_target);
		auto message_size=std::ranges::distance(in_message);

		if(!target_size || !message_size) std::invalid_argument("invalid file contents!");

		auto header_bytes=to_little_endianness_bytes<8>(message_size);
		auto shift=(target_size-8)/(message_size*8);

		if(!shift) throw std::invalid_argument("message too big!");

		return SteganographyImageMetadata(
			target_size,
			message_size,
			header_bytes,
			shift
		);
	}

private:
	SteganographyImageMetadata(
		std::size_t const target_size,
		std::uint8_t message_size,
		std::array<std::uint8_t, 8> const header_bytes,
		std::size_t const shift
	):
	target_size_(target_size),
	message_size_(message_size),
	header_bytes_(header_bytes),
	shift_(shift){}
};

}

