#pragma once

#include <vector>
#include "bitmap.hpp"
#include <string>
#include <iterator>
#include <bitset>
#include <iterator>
#include <cassert>
#include <span>


namespace jr{
namespace img{

auto encode_bmp(std::string const& input_path, std::string const& output_path, std::vector<std::uint8_t> const& message) -> void;

auto decode_bmp(std::string const& input_path) -> std::vector<std::uint8_t>;

auto encode_ppm(std::string const& input_path, std::string const& output_path, std::vector<std::uint8_t> const& message) -> void;

auto decode_ppm(std::string const& input_path) -> std::vector<std::uint8_t>;

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

	if(!target_size || !message_size) std::invalid_argument("invalid file contents!");

	std::ranges::copy(std::begin(in_target), std::end(in_target), out_target);

	auto header_bytes=to_little_endianness_bytes<8>(message_size);
	auto shift=(target_size-8)/(message_size*8);

	if(!shift) throw std::invalid_argument("message too big!");

	for(auto i=0u;i<8u;i++, ++out_target) *out_target=header_bytes[i];

	for(auto message_byte : in_message){
		std::bitset<8> message_byte_bitset(message_byte);

		for(auto i=0u;i<8u;i++,std::ranges::advance(out_target, shift)){
			auto bitmap_byte=std::bitset<8>();
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

	if(!data_size) throw std::invalid_argument("encoded image is corrupted, data size is 0!");

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

template<img::ImageFormat format>
struct ImageFormatHeader{};

template<>
struct ImageFormatHeader<img::ImageFormat::bmp>{
	using value_type=BMPHeader;
};

template<>
struct ImageFormatHeader<img::ImageFormat::ppm>{
	using value_type=PPMHeader;
};

template<img::ImageFormat format, typename InputTargetRange, typename InputMessageRange, typename OutputIterator>
auto encode(InputTargetRange input_target, InputMessageRange message, OutputIterator output) -> void
requires
std::ranges::input_range<InputTargetRange>
&&
std::ranges::input_range<InputMessageRange>
&&
std::same_as<typename InputTargetRange::iterator::value_type, std::uint8_t>
&&
std::same_as<typename InputMessageRange::iterator::value_type, std::uint8_t>
&&
std::output_iterator<OutputIterator, std::uint8_t>{
	std::vector<std::uint8_t> image_buffered(input_target);

	//since application could be expanded and more image types may be added, conditional_t would not work here.
	using HeaderType=typename ImageFormatHeader<format>::value_type;

	HeaderType header{image_buffered};

	auto it=image_buffered.begin()+header.first_data_byte;

	auto span=std::span{it, image_buffered.end()};

	encode_message(span, message, it);

	std::ranges::copy(image_buffered, output);
}

template<img::ImageFormat format, typename InputTargetRange, typename OutputIterator>
auto decode(InputTargetRange input_target, OutputIterator output) -> std::vector<std::uint8_t>
requires 
std::ranges::input_range<InputTargetRange>
&&
std::same_as<typename InputTargetRange::iterator::value_type, std::uint8_t>
&&
std::output_iterator<OutputIterator, std::uint8_t>{
	std::vector<std::uint8_t> image_buffered(input_target);

	using HeaderType=typename ImageFormatHeader<format>::value_type;

	HeaderType header{image_buffered};

	std::vector<std::uint8_t> result;

	auto span=std::span{image_buffered.begin()+header.first_data_byte, image_buffered.end()};

	decode_message(span, std::back_inserter(result));

	return result;
}


}
}

