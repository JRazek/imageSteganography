#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>

#include "header_utils.hpp"
#include "image_formatters.hpp"
#include "ui.hpp"

namespace jr {}

auto main(int argc, char **argv) -> int {
  jr::terminal_ui ui;

  ui.run(argc, argv);
}
