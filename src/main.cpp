#include <cstdint>
#include <iostream>
#include <bitmap.hpp>
#include <image_formatters.hpp>
#include <fstream>
#include <cassert>

auto main()->int{
    using namespace jr::img;

    auto file="/home/user/CLionProjects/imageSteganography/test_resources/LAND.BMP";

    auto file2="/home/user/CLionProjects/imageSteganography/test_resources/output_LAND.BMP";

	std::vector<std::uint8_t> message{'2','1','3','7'};

	encode_bmp(file, file2, message);

	auto res=decode_bmp(file2);

	std::string s(res.begin(), res.end());
	std::cout<<s<<'\n';
}

