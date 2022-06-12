#include <iostream>
#include <bitmap.hpp>
#include <image_formatters.hpp>
#include <fstream>
#include <cassert>

auto main()->int{
    using namespace jr::img;
    PPM img(10, 10, 3);
    auto file=std::fstream("/home/user/CLionProjects/imageSteganography/test_resources/image.ppm");

    auto file2=std::ofstream("/home/user/CLionProjects/imageSteganography/test_resources/image2.ppm");


    img=read_ppm(std::move(file));

	write_ppm(img, std::move(file2));

//	img=encode_message(std::move(img), {1, 1, 2, 3, 45});

	auto message=decode_message(img);

	for(auto i : message) std::cout<<int(i)<<' ';

	std::cout<<'\n';

	auto bytes=jr::to_little_endianness_bytes(10);
	
	for(auto i : bytes) std::cout<<int(i)<<' ';


//	PPM ppm2=encode_message<PPM>(img, {'1', '1', '1'});

	PPM ppm(Bitmap(0,0,0));


}

