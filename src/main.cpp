#include <cstdint>
#include <iostream>
#include "header_utils.hpp"
#include "image_formatters.hpp"
#include "ui.hpp"
#include <fstream>
#include <cassert>

namespace jr{
}

auto main(int argc, char **argv)->int{
	jr::terminal_ui ui;

	ui.run(argc, argv);
}
