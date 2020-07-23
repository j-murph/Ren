#pragma once
#include "stdafx.h"
#include <string>
#include <vector>

class Tokenizer
{
private:
	std::vector<std::string> tokens;
public:
	void TokenizeString(const std::string& str, char delimiter);
	void Clear();
};