#pragma once

#include <vector>
#include <bitmap.hpp>
#include <string>
#include <iterator>
#include <bitset>
#include <iterator>


namespace jr{
namespace img{

auto encode_bmp(std::string const& path, std::vector<std::uint8_t> const& data) -> void;

auto encode_bmp(std::iostream&& str, std::vector<std::uint8_t> const& data) -> void;

auto decode_bmp(std::istream&& str) -> std::vector<std::uint8_t>;

template<typename Input, typename Output>
auto encode_message(Input in, std::uint32_t target_size, Output out, std::vector<std::uint8_t> const& data) -> void;


template<typename Range, typename OutputIterator>
auto decode_message(Range in, OutputIterator out_it) -> void;





////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////VIEWER DISCRETION IS ADVISED/////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



template<typename Input, typename Output
>
auto encode_message(Input in, std::uint32_t target_size, Output out, std::vector<std::uint8_t> const& data) -> void
requires 
std::input_iterator<Input>
&&
std::output_iterator<Output, std::uint8_t>
&&
std::same_as<typename Input::value_type, std::uint8_t>
&& 
std::same_as<typename Output::value_type, std::uint8_t>
{
	auto header_bytes=to_little_endianness_bytes<8>(data.size());
	
	auto shift=(target_size-8)/(data.size()*8);

	for(auto i=0;i<8;i++, ++out) *out=header_bytes[i];

	for(auto b : data){
		std::bitset<8> byte(b);

		if constexpr (std::bidirectional_iterator<Input> && std::bidirectional_iterator<Output>){
			for(auto i=0u; i<8; i++, std::ranges::advance(out, shift), std::ranges::advance(in, shift)){//add constexpr if for advance!
				auto bitmap_byte=std::bitset<8>(*in);
				bitmap_byte[7]=byte[i];
				
				*out=bitmap_byte.to_ulong();
			}
		}else{
			auto advance=[](auto& it, auto const n){ while(n--) ++it; };

			for(auto i=0u; i<8; i++, advance(out, shift), advance(in, shift)){//add constexpr if for advance!
				auto bitmap_byte=std::bitset<8>(*in);
				bitmap_byte[7]=byte[i];
				
				*out=bitmap_byte.to_ulong();
			}
		}
	}
}

template<typename Range, typename OutputIterator>
auto decode_message(Range in, OutputIterator out_it) -> void
requires
std::ranges::input_range<Range>
&&
std::output_iterator<OutputIterator, std::uint8_t>
{

	auto target_size=std::ranges::distance(in);

	auto data_size=bytes_to_little_endianess<std::size_t>(in | std::views::take(8));
	
	auto shift=(target_size-8)/(data_size*8);

	if(!shift) throw std::invalid_argument("encoded image is corrupted!");

	auto in_it=std::begin(in); std::ranges::advance(in_it, 8);


	for(auto i=0u;i<data_size;i++, ++out_it){
		std::bitset<8> data_byte;

		for(auto i=0u;i<8;i++, std::ranges::advance(in_it, shift)){
			auto bitmap_byte=std::bitset<8>(*in_it);
			data_byte[i]=bitmap_byte[7];
		}

		*out_it=data_byte.to_ulong();
	}
}

}
}

