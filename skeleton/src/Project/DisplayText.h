#pragma once
#include <iostream>
#include "../../core.hpp"

using namespace std;

class DisplayText {
private:
	std::string text;
	Vector4 color;
	std::pair<int, int> pos;
	void* font;

public:
	DisplayText(const string& text, const Vector4& color, const std::pair<int, int>& pos, void* font) :
		color(color), text(text), pos(pos), font(font) {}

	string getText() const {
		return text;
	}

	Vector4 getColor() const {
		return color;
	}

	std::pair<int, int> getPos() const {
		return pos;
	}

	void* getFont() const {
		return font;
	}

	void setText(string newText) {
		this->text = newText;
	}

	void release();
};