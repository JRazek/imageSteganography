
#include "ui.hpp"

#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include "header_utils.hpp"
#include "image_formatters.hpp"
#include "utils.hpp"

namespace jr {

TerminalUI::TerminalUI(std::ostream& output_stream)
    : output_stream_(output_stream) {}

auto TerminalUI::show_help() -> void {
  constexpr std::string_view s =
      "Usage: imagesteganography [OPTION] ...\n"
      "Supported formats:                       PPM-P6, Windows-BMP\n\n"
      "-i, --info                               shows size, last "
      "modification date, image size\n"
      "-e, --encrypt [FILE] \"[MESSAGE]\"         encrypts file with "
      "specified message\n"
      "-d, --decrypt [FILE]                     decrypts file\n"
      "-c, --check   [FILE] \"[MESSAGE]\"         checks whether message can "
      "be encrypted into specified file\n"
      "-h, --help                               shows help\n";

  output_stream_ << s;
}

auto TerminalUI::show_info(std::string const& file) -> void {
  auto header = img::detectAndCreate(file);

  auto time_string = get_last_write_as_string(file);

  // couldnt use std::format in my compiler :<<
  std::string s = "last modification:                    " + time_string +
                  '\n' + "file size:                            " +
                  std::to_string(header->get_file_size()) + '\n' +
                  "image size                            " +
                  std::to_string(header->get_image_size()) + '\n' + "\n";

  output_stream_ << s;
}

auto TerminalUI::encrypt_message(std::string const& file,
                                 std::vector<std::uint8_t> const& message)
    -> void {
  try {
    auto header = img::detectAndCreate(file);
    if (header->get_file_format() == img::ImageFormat::bmp) {
      img::encode_format<img::ImageFormat::bmp>(file, file, message);
    } else if (header->get_file_format() == img::ImageFormat::ppm) {
      img::encode_format<img::ImageFormat::ppm>(file, file, message);
    } else
      assert(false);

    output_stream_ << "successfully encrypted " << file << '\n';
  } catch (std::exception const& e) {
    output_stream_ << e.what() << '\n';
  }
}

auto TerminalUI::decrypt_message(std::string const& file) -> void {
  try {
    auto header = img::detectAndCreate(file);

    std::vector<std::uint8_t> result;

    if (header->get_file_format() == img::ImageFormat::bmp) {
      result = img::decode_format<img::ImageFormat::bmp>(file);
    } else if (header->get_file_format() == img::ImageFormat::ppm) {
      result = img::decode_format<img::ImageFormat::ppm>(file);
    } else
      assert(false);
    std::string str(result.begin(), result.end());

    output_stream_ << "successfully decrypted:\t\t\t" << str << '\n';
  } catch (std::exception const& e) {
    output_stream_ << e.what() << '\n';
  }
}

auto TerminalUI::check_encodable(std::string const& file,
                                 std::vector<std::uint8_t> const& message)
    -> void {
  auto header = img::detectAndCreate(file);

  bool ok = false;
  if (header->get_file_format() == img::ImageFormat::bmp) {
    ok =
        img::check_encodable_format<img::ImageFormat::bmp>(file, file, message);
  } else if (header->get_file_format() == img::ImageFormat::ppm) {
    ok =
        img::check_encodable_format<img::ImageFormat::ppm>(file, file, message);
  } else
    assert(false);

  if (ok)
    output_stream_ << file << " can be encoded with \""
                   << std::string(message.begin(), message.end()) << "\"\n";
  else
    output_stream_ << "cannot encode message to:" << file << '\n';
}

auto TerminalUI::run(int argc, char** argv) -> void {
  if (argc >= 2) {
    std::string first = argv[1];
    if ((first == "-i" || first == "--info") && argc == 3) {
      std::string file = argv[2];
      show_info(file);
    } else if ((first == "-e" || first == "--encrypt") && argc == 4) {
      std::string file = argv[2];
      std::vector<std::uint8_t> message(argv[3],
                                        argv[3] + std::strlen(argv[3]));

      encrypt_message(file, message);
    } else if ((first == "-d" || first == "--decrypt") && argc == 3) {
      std::string file = argv[2];
      decrypt_message(file);
    } else if ((first == "-c" || first == "--check") && argc == 4) {
      std::string file = argv[2];
      std::vector<std::uint8_t> message(argv[3],
                                        argv[3] + std::strlen(argv[3]));

      check_encodable(file, message);
    } else if (first == "-h" || first == "--help") {
      show_help();
    } else if (first == "--helpmepls" && is_linux_machine()) {
      auto res=std::system("vim --cmd \"help 42\"");
	  if(res) std::cerr<<"rip :<<";
    } else {
      invalid_argument();
    }
  } else {
    invalid_argument();
  }
}

auto TerminalUI::invalid_argument() -> void {
  output_stream_ << "invalid run. use -h or --help for help\n";
}

}  // namespace jr
