#pragma once
#include <cassert>
#include <fstream>
#include <sstream>
#include <string>

/// <summary>
/// CSV��ǂݍ���
/// </summary>
/// <param name="filename">�t�@�C����</param>
/// <returns></returns>
std::stringstream LoadCSVData(const std::string& filename);
