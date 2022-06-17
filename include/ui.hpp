#pragma once

#include <iostream>
#include <ostream>
#include <string>
#include <vector>

namespace jr {

/**
 * @brief class implementing communication with specified output stream
 */
class TerminalUI {
 public:
  explicit TerminalUI(std::ostream& output_stream = std::cout);

  auto show_help() -> void;

  auto show_info(std::string const& file) -> void;

  auto encrypt_message(std::string const& file,
                       std::vector<std::uint8_t> const& message) -> void;

  auto decrypt_message(std::string const& file) -> void;

  auto check_encodable(std::string const& file,
                       std::vector<std::uint8_t> const& message) -> void;

  auto run(int argc, char** argv) -> void;

  auto invalid_argument() -> void;

 private:
  std::ostream& output_stream_;
};

}  // namespace jr

