#pragma once

#include <vector>
#include "bitmap.hpp"
#include "range/v3/view/zip_with.hpp"
#include "utils.hpp"
#include <string>
#include <iterator>
#include <bitset>
#include <iterator>
#include <cassert>
#include <span>
#include <istream>
#include <ostream>
#include <fstream>
#include <ranges>
#include <range/v3/view/zip.hpp>



namespace jr{
namespace img{

constexpr auto steganography_bit=0u;

template<typename InputTargetRange, typename InputMessageRange, typename OutputIterator>
auto encode_message(InputTargetRange in_target, InputMessageRange in_message, OutputIterator output_it) -> void
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
	std::ranges::copy(in_target, output_it);

	auto encoding_data=SteganographyImageMetadata::create_metadata_for_encrypting(in_target, in_message);

	auto input_target_it=std::begin(in_target);

	for(auto i=0u;i<8u;i++, ++output_it, ++input_target_it) *output_it=encoding_data.header_bytes_[i];

	for(auto message_byte : in_message){
		std::bitset<8> message_byte_bitset(message_byte);
		
		for(auto i=0u;i<8u;i++,std::ranges::advance(output_it, encoding_data.shift_), std::ranges::advance(input_target_it, encoding_data.shift_)){
			auto bitmap_byte=std::bitset<8>(*input_target_it);
			bitmap_byte[steganography_bit]=message_byte_bitset[i];
			
			*output_it=bitmap_byte.to_ulong();
		}
	}

}


template<typename Range, typename OutputIterator>
auto decode_message(Range in_target, OutputIterator output_it) -> void
requires
std::ranges::input_range<Range>
&&
std::same_as<typename Range::iterator::value_type, std::uint8_t>
&&
std::output_iterator<OutputIterator, std::uint8_t>
{
	auto decoding_data=SteganographyImageMetadata::create_metadata_for_decrypting(in_target);

	auto in_it=std::begin(in_target); std::ranges::advance(in_it, 8);

	for(auto i=0u;i<decoding_data.message_size_;i++, ++output_it){
		std::bitset<8> data_byte;

		for(auto i=0u;i<8;i++, std::ranges::advance(in_it, decoding_data.shift_)){
			auto bitmap_byte=std::bitset<8>(*in_it);
			data_byte[i]=bitmap_byte[steganography_bit];
		}

		*output_it=data_byte.to_ulong();
	}
}

template<ImageFormat format>
auto encode_format(std::string const& input_path, std::string const& output_path, std::vector<std::uint8_t> const& message) -> void{
	std::ifstream input_str(input_path, std::ios::binary);
	std::istreambuf_iterator<char> input_stream_it(input_str);

	std::vector<std::uint8_t> buffer_input(input_stream_it, std::istreambuf_iterator<char>());
	std::vector<std::uint8_t> buffer_output;

	encode<format>(buffer_input, message, std::back_inserter(buffer_output));

	std::ofstream output_str(output_path, std::ios::binary);
	std::ostreambuf_iterator<char> output_stream_it(output_str);

	std::ranges::copy(buffer_output, output_stream_it);
}

template<ImageFormat format>
auto decode_format(std::string const& input_path) -> std::vector<std::uint8_t>{
	std::ifstream input_stream(input_path, std::ios::binary);

	std::istreambuf_iterator<char> input_stream_it(input_stream);
	std::vector<std::uint8_t> buffer_input(input_stream_it, std::istreambuf_iterator<char>());

	std::vector<std::uint8_t> result;
	return decode<format>(buffer_input, std::back_inserter(result));
}


template<img::ImageFormat format> struct ImageFormatHeader{ };

template<> struct ImageFormatHeader<img::ImageFormat::bmp>{ using value_type=BMPHeader; };

template<> struct ImageFormatHeader<img::ImageFormat::ppm>{ using value_type=PPMHeader; };

/*
 * generates format specfic
 *
 */
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


template<ImageFormat format>
auto check_encodable_format(std::string const& input_path, std::string const& output_path, std::vector<std::uint8_t> const& message) -> bool{
	std::ifstream input_str(input_path, std::ios::binary);
	std::istreambuf_iterator<char> input_stream_it(input_str);

	std::vector<std::uint8_t> image_buffered(input_stream_it, std::istreambuf_iterator<char>());

	using HeaderType=typename ImageFormatHeader<format>::value_type;

	try{
		HeaderType header{image_buffered};

		std::vector<std::uint8_t> result;

		auto span=std::span{image_buffered.begin()+header.first_data_byte, image_buffered.end()};

		SteganographyImageMetadata::create_metadata_for_encrypting(image_buffered, message);
	}catch(...){ 
		return false; 
	}

	return true;
}


}
}

