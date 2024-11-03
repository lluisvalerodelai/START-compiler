#pragma once

#include "../lexerpp.hpp"

#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <filesystem>

std::string ToJSON(const Token& token);
void ToJSON(const std::vector<Token>& tokens);