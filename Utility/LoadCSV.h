#pragma once
#include <cassert>
#include <fstream>
#include <sstream>
#include <string>

/// <summary>
/// CSVを読み込む
/// </summary>
/// <param name="filename">ファイル名</param>
/// <returns></returns>
std::stringstream LoadCSVData(const std::string& filename);
