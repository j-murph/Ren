#pragma once
#include "stdafx.h"
#include <sstream>
#include <fstream>


template <class delimiterT = char>
class Tokenizer
{
private:
	std::vector<std::string> tokens;

	bool enableParserThrow;

	const static inline std::string emptyString = "";


public:
	Tokenizer(bool throwOnParseError = true) : enableParserThrow(throwOnParseError)
	{
	}

	Tokenizer(const std::string& tokenizeMe, delimiterT delimiter, bool throwOnParseError = true) : Tokenizer(throwOnParseError)
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
			tokens.push_back(std::move(token));
		}
	}

	template <class T>
	T ParseToken(size_t tokenIndex)
	{
		if (tokenIndex < 0 || tokenIndex >= tokens.size())
		{
			ThrowParserExceptionIfEnabled<std::out_of_range>("Index out of range.");
			return T();
		}

		T temp;
		std::stringstream ss;
		ss << tokens[tokenIndex];
		ss >> temp;

		if (ss.fail())
			ThrowParserExceptionIfEnabled<std::exception>("Parse failure.");

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

	template<class exT>
	void ThrowParserExceptionIfEnabled(const char* message)
	{
		if (enableParserThrow)
		{
			throw exT(message);
		}
	}

	const std::string& operator[](size_t tokenIndex)
	{
		if (tokenIndex < 0 || tokenIndex >= tokens.size())
		{
			ThrowParserExceptionIfEnabled<std::out_of_range>("Index out of range.");
			return emptyString;
		}

		return tokens[tokenIndex];
	}
};