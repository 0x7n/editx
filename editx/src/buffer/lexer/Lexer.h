#pragma once
#include <string>
#include <vector>
#include <map>
#include <iostream>
// thanks https://medium.com/@tharunappu2004/writing-a-lexer-in-c-a-step-by-step-guide-a1d5c55ac04d for the guide

enum TokenType {
	NUMBER,
	IDENTIFIER,
	EQUALS,
	PARENTHESES,
	BINARYOPERATOR,
	LET,
	CONDITION,
	WHITESPACE,
	NEWLINE,
	TAB,
	STRING
};

struct Token
{
	std::string value;
	TokenType type;
};

typedef std::map<std::string, TokenType> ReservedIdMap;
inline ReservedIdMap reservedId;


inline std::vector<std::string> splitString(const std::string& sourceCode) {
	std::vector<std::string> words;
	std::string word;

	for (char ch : sourceCode) {
		if (ch != ' ') {
			word += ch;
		}
		else if (!word.empty()) {
			words.push_back(word);
			word.clear();
		}
	}

	if (!word.empty()) {
		words.push_back(word);
	}

	return words;
}

inline std::string shift(std::vector<std::string>& src)
{
	std::string current = src.front();
	src.erase(src.begin());
	return current;
}

inline bool isNumber(const std::string& str)
{
	for (char ch : str)
	{
		if (!isdigit(ch))
			return false;
	}
	return true;
}

// checks to see if the character is alphabetic
inline bool isAlpha(const std::string& str)
{
	for (char ch : str)
	{
		if (!isalpha(ch))
			return false;
	}
	return true;
}

inline bool isSkippable(char ch)
{
	return ch == ' ' || ch == '\t' || ch == '\n';
}

inline Token token(std::string value, TokenType tokentype)
{
	return { value, tokentype };
}

inline std::vector<std::string> splitStringWithSpaces(const std::string& sourceCode) {
	std::vector<std::string> words;
	std::string word;

	for (char ch : sourceCode) {
		if (ch == ' ') {
			if (!word.empty()) {
				words.push_back(word);
				word.clear();
			}
			words.push_back(" "); // Space as a token
		}
		else if (ch == '\n') {
			if (!word.empty()) {
				words.push_back(word);
				word.clear();
			}
			words.push_back("\n"); // Newline as a token
		}
		else if (ch == '\t') {
			if (!word.empty()) {
				words.push_back(word);
				word.clear();
			}
			words.push_back("\t"); // Tab as a token
		}
		else if (ch == '_' || isalnum(ch))
		{
			word += ch;
		}
		else if (!isSkippable(ch))
		{
			if (!word.empty()) {
				words.push_back(word);
				word.clear();
			}

			words.push_back(std::string(1, ch));
		}
		else {
			word += ch;
		}
	}

	if (!word.empty()) {
		words.push_back(word);
	}

	return words;
}

namespace lexer
{
	inline void initReservedIdentifier()
	{
		reservedId["const"] = TokenType::LET;
		reservedId["int"] = TokenType::LET;
		reservedId["float"] = TokenType::LET;
		reservedId["while"] = TokenType::CONDITION;
		reservedId["if"] = TokenType::CONDITION;
		reservedId["else"] = TokenType::CONDITION;
	}

	inline std::vector<Token> tokenize(std::string& sourceCode)
	{
		std::vector<Token> tokens;
		std::vector<std::string> src = splitStringWithSpaces(sourceCode);

		while (!src.empty()) {
			std::string current = src.front();
			//std::cout << "Processing Token: " << current << std::endl;

			if (current == "(" || current == ")" || current == "{" || current == "}") {
				tokens.push_back(token(shift(src), TokenType::PARENTHESES));
			}
			else if (current == "+" || current == "-" || current == "*" || current == "/")
			{
				tokens.push_back(token(shift(src), TokenType::BINARYOPERATOR));
			}
			else if (current == "=")
			{
				tokens.push_back(token(shift(src), TokenType::EQUALS));
			}
			else if (current == " ") { 
				tokens.push_back(token(shift(src), TokenType::WHITESPACE));
			}
			else if (current == "\n") { 
				tokens.push_back(token(shift(src), TokenType::NEWLINE));
			}
			else if (current == "\t") { 
				tokens.push_back(token(shift(src), TokenType::TAB));
			}
			else if (current == "\"")
			{
				shift(src);
				std::string strValue;
				while (!src.empty()) {
					if (src.front() == "\\") {
						shift(src);
						if (!src.empty())
						{
							strValue += shift(src);
						}
					}
					else if (src.front() == "\"")
					{
						break;
					}
					else {
						strValue += shift(src);
					}
				}
				if (!src.empty() && src.front() == "\"")
				{
					shift(src);
				}

				tokens.push_back(token(strValue, TokenType::STRING));
			}
			else
			{ // handle multichar token & number tokens
				if (isNumber(current))
				{
					std::string number;
					while (!src.empty() && isNumber(current))
					{
						number += shift(src);
					}

					tokens.push_back(token(number, TokenType::NUMBER));
				} // Handling Identifier tokens
				else if (isAlpha(current))
				{
					std::string ident = shift(src);
					// check for reserved tokens like let etc
					ReservedIdMap::iterator it = reservedId.find(ident);
					if (it != reservedId.end())
					{
						tokens.push_back(token(ident, it->second));
					}
					else
					{
						tokens.push_back(token(ident, TokenType::IDENTIFIER));
					}
				}
				else if (current == ":" || current == ";" || current == "#" || current == "<" || current == ">" )
				{
					tokens.push_back(token(shift(src), TokenType::IDENTIFIER));
				}
				else
				{
					std::cout << "Unknown token, skipping: " << current << std::endl;
					shift(src);
				}
			}
		}

		return tokens ;
	}
}

