#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>

#include "header_utils.hpp"
#include "image_formatters.hpp"
#include "ui.hpp"

auto main(int argc, char **argv) -> int {
  jr::TerminalUI ui;

  ui.run(argc, argv);
}

