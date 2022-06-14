#pragma once

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
auto to_little_endianness_bytes(std::integral auto n) -> std::vector<std::uint8_t> {
    std::vector<std::uint8_t> bytes(size);

	for(auto& b : bytes){
		b=n&0xFFu;
		n>>=8u;
	}

	if constexpr (std::endian::native==std::endian::little) std::ranges::reverse(bytes);

    return bytes;
}

auto to_little_endianness_bytes(std::integral auto n) -> std::vector<std::uint8_t> { return to_little_endianness_bytes<sizeof(n)>(n); }


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

}

