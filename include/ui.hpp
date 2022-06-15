#pragma once

#include "bitmap.hpp"
#include "utils.hpp"
#include <chrono>
#include <fstream>
#include <ostream>
#include <iostream>
#include <filesystem>
#include <chrono>
#include <string>


namespace jr{

class terminal_ui{
public:
	terminal_ui(std::ostream& output_stream = std::cout):
	output_stream_(output_stream)
	{}


auto show_help(){
	constexpr std::string_view s=
		"Usage: imagesteganography [OPTION] ...\n\n"

		"-i, --info                               shows size, last modification date, image size\n"
		"-e, --encrypt [FILE] \"[MESSAGE]\"         encrypts file with specified message\n"
		"-d, --decrypt [FILE]                     decrypts file\n"
		"-c, --check   [FILE] \"[MESSAGE]\"         checks whether message can be encrypted into specified file\n"
		"-h, --help                               shows help\n";

	output_stream_<<s;	
}

auto show_info(std::string const& file){
	std::ifstream input_stream(file, std::ios::binary);
	auto header=img::detectAndCreate(std::move(input_stream));

	auto time_string=get_last_write_as_string(file);


	//couldnt use std::format in my compiler :<<

	std::string s=
		"last modification:                    "+time_string+'\n'+
		"file size:                            "+std::to_string(header->get_file_size())+'\n'+
		"image size                            "+std::to_string(header->get_image_size())+'\n'+
		"\n";

	output_stream_<<s;	
}

auto invalid_argument(){
	output_stream_<<"invalid run. use -h or --help for help\n";
}

auto run(int argc, char **argv){
	if(argc>=2){
		std::string first=argv[1];
		if((first=="-i" || first=="--info") && argc==3){
			std::string file=argv[2];
			show_info(file);
		}
		else if((first=="-e" || first=="--encrypt") && argc==4){
			std::string file=argv[2];
			std::string message=argv[3];
			//encrypt message
		}
		else if((first=="-d" || first=="--decrypt") && argc==3){
			std::string file=argv[1];
			//decrypt message
		}
		else if((first=="-c" || first=="--check") && argc==4){
			std::string file=argv[2];
			std::string message=argv[3];
			//check if file can be encrypted with specified message
		}
		else if(first=="-h" || first=="--help"){
			show_help();
		}
		else if(first=="--letsplay"){
			//easter egg?
		}
		else{
			invalid_argument();
		}
	}else{
		invalid_argument();
	}
}

private:
	std::ostream& output_stream_;
};

}
