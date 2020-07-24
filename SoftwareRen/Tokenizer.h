#pragma once
#include "stdafx.h"
#include <string>
#include <vector>
#include <sstream>

template <class delimiterT = char>
class Tokenizer
{
private:
	std::vector<std::string> tokens;

public:
	Tokenizer()
	{
	}

	Tokenizer(const std::string& tokenizeMe, delimiterT delimiter)
	{
		TokenizeString(tokenizeMe, delimiter);
	}

	void TokenizeString(const std::string& str, delimiterT delimiter)
	{
		std::stringstream ss;
		ss << str;

		std::string token;
		while (std::getline(ss, token, delimiter))
		{
			tokens.push_back(token);
		}
	}

	void Clear()
	{
		tokens.clear();
	}

	const std::vector<std::string>& GetTokens()
	{
		return tokens;
	}
};