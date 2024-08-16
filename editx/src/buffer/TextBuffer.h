#pragma once
#include <string>

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
