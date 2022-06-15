#include <cstdint>
#include <iostream>
#include "bitmap.hpp"
#include "image_formatters.hpp"
#include <fstream>
#include <cassert>

auto main()->int{
    using namespace jr::img;

    auto file="/home/user/CLionProjects/imageSteganography/test_resources/LAND.BMP";

    auto file2="/home/user/CLionProjects/imageSteganography/test_resources/output_LAND.BMP";

	std::vector<std::uint8_t> message;
	for(int i=0;i<10;i++) message.push_back(('A'+i)%('z'+1));

	//encode_bmp(file, file2, message);

	auto res=decode_bmp(file2);

	std::string s(res.begin(), res.end());
	std::cout<<s<<'\n';


    auto file3="/home/user/CLionProjects/imageSteganography/test_resources/sample_640×426.ppm";
    auto file4="/home/user/CLionProjects/imageSteganography/test_resources/output_ppm.ppm";

	encode_ppm(file3, file4, message);
}

