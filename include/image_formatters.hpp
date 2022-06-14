#pragma once

#include <vector>
#include "bitmap.hpp"
#include <string>
#include <iterator>
#include <bitset>
#include <iterator>
#include <cassert>


namespace jr{
namespace img{

auto encode_bmp(std::string const& input_path, std::string const& output_path, std::vector<std::uint8_t> const& data) -> void;

auto encode_bmp(std::istream&& input, std::ostream&& output, std::vector<std::uint8_t> const& message) -> void;

auto decode_bmp(std::string const& input_path) -> std::vector<std::uint8_t>;

auto decode_bmp(std::istream&& input) -> std::vector<std::uint8_t>;

template<typename InputTargetRange, typename InputMessageRange, typename OutputIterator>
auto encode_message(InputTargetRange in_target, InputMessageRange in_message, OutputIterator out_target) -> void
requires 
std::ranges::input_range<InputTargetRange>
&&
std::ranges::input_range<InputMessageRange>
&&
std::same_as<typename InputTargetRange::iterator::value_type, std::uint8_t>
&&
std::same_as<typename InputMessageRange::iterator::value_type, std::uint8_t>
&&
std::output_iterator<OutputIterator, std::uint8_t>
{
	auto target_size=std::ranges::distance(in_target);
	auto message_size=std::ranges::distance(in_message);

	assert(target_size && message_size);

	std::ranges::copy(std::begin(in_target), std::end(in_target), out_target);

	auto header_bytes=to_little_endianness_bytes<8>(message_size);
	auto shift=(target_size-8)/(message_size*8);

	if(!shift) throw std::invalid_argument("message too big!");

	for(auto i=0u;i<8u;i++, ++out_target) *out_target=header_bytes[i];

	for(auto message_byte : in_message){
		std::bitset<8> message_byte_bitset(message_byte);

		for(auto i=0u;i<8u;i++,std::ranges::advance(out_target, shift)){
			auto bitmap_byte=std::bitset<8>(*out_target);
			bitmap_byte[0]=message_byte_bitset[i];
			
			*out_target=bitmap_byte.to_ulong();
		}
	}

}


template<typename Range, typename OutputIterator>
auto decode_message(Range in, OutputIterator out_it) -> void
requires
std::ranges::input_range<Range>
&&
std::same_as<typename Range::iterator::value_type, std::uint8_t>
&&
std::output_iterator<OutputIterator, std::uint8_t>
{

	auto target_size=std::ranges::distance(in);

	auto data_size=bytes_to_little_endianess<std::size_t>(in | std::views::take(8));

	if(!data_size) throw std::invalid_argument("encoded image is corrupted!");

	auto shift=(target_size-8)/(data_size*8);

	if(!shift) throw std::invalid_argument("encoded image is corrupted!");

	auto in_it=std::begin(in); std::ranges::advance(in_it, 8);


	for(auto i=0u;i<data_size;i++, ++out_it){
		std::bitset<8> data_byte;

		for(auto i=0u;i<8;i++, std::ranges::advance(in_it, shift)){
			auto bitmap_byte=std::bitset<8>(*in_it);
			data_byte[i]=bitmap_byte[0];
		}

		*out_it=data_byte.to_ulong();
	}
}



}
}

