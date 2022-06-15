#pragma once

#include <ostream>
#include <iostream>


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

auto invalid_argument(){
	output_stream_<<"invalid run. use -h or --help for help\n";
}

auto run(int argc, char **argv){
	if(argc>=2){
		std::string first=argv[1];
		if((first=="-i" || first=="--info") && argc==3){
			std::string file=argv[2];
			//read file
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
