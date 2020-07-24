#pragma once
#include "stdafx.h"
#include <string>
#include <vector>
#include <sstream>

const std::string emptyString = "";

template <class delimiterT = char>
class Tokenizer
{
private:
	std::vector<std::string> tokens;

	bool enableParserThrow;

public:
	Tokenizer(bool throwOnParseError) : enableParserThrow(throwOnParseError)
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

	template <class T>
	T ParseToken(int tokenIndex)
	{
		if (tokenIndex < 0 || tokenIndex >= tokens.size())
		{
			if (enableParserThrow) ThrowParserException("Index out of range.");
			return T();
		}

		T temp;
		std::stringstream ss;
		ss << tokens[tokenIndex];
		ss >> temp;

		if (ss.fail() && enableParserThrow)
			ThrowParserException("Parse failure.");

		return temp;
	}

	void Clear()
	{
		tokens.clear();
	}

	const std::vector<std::string>& GetTokens()
	{
		return tokens;
	}

	void ThrowParserException(const char* message)
	{
		throw std::exception(message);
	}

	const std::string& operator[](int tokenIndex)
	{
		if (tokenIndex < 0 || tokenIndex >= tokens.size())
		{
			if (enableParserThrow) ThrowParserException("Index out of range.");
			return emptyString;
		}

		return tokens[tokenIndex];
	}
};