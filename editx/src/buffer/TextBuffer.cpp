#include "TextBuffer.h"

TextBuffer::TextBuffer() : text("") {}

void TextBuffer::appendText(const std::string& text)
{
	this->text += text;
}

void TextBuffer::deleteLastCharacter()
{
	if (!text.empty())
		text.pop_back();
}

void TextBuffer::setText(const std::string& text)
{
	this->text = text;
}

const std::string& TextBuffer::getText() const
{
	return text;
}