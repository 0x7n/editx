#pragma once
#include <string>

/*

	THIS IS OLD AND SHOULD ACTUALLY BE DELETED, IM JUST LAZY

*/

class TextBuffer {
public:
	TextBuffer();

	void appendText(const std::string& text);
	void deleteLastCharacter();
	void setText(const std::string& text);
	const std::string& getText() const;

private:
	std::string text;
};
