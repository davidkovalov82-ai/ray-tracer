#pragma once
#include <string>
#include <vector>

#include "color.h"

bool save_ppm(const std::string& filename, int width, int height, const std::vector<Color>& pixels);
