#include "utils.hpp"

#include <chrono>
#include <filesystem>
#include <string>

namespace jr {

auto get_last_write_as_string(std::string const& file) -> std::string {
  auto time = std::filesystem::last_write_time(file);
  std::time_t cftime = std::chrono::system_clock::to_time_t(
      std::chrono::file_clock::to_sys(time));  // lesser precision cast

  return std::asctime(std::localtime(&cftime));
}

}  // namespace jr

