#pragma once
#include <cassert>
#include <fstream>
#include <sstream>
#include <string>

/// <summary>
/// CSV‚ğ“Ç‚İ‚Ş
/// </summary>
/// <param name="filename">ƒtƒ@ƒCƒ‹–¼</param>
/// <returns></returns>
std::stringstream LoadCSVData(const std::string& filename);
