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

	inline string getText() const {
		return text;
	}

	inline Vector4 getColor() const {
		return color;
	}

	inline std::pair<int, int> getPos() const {
		return pos;
	}

	inline void* getFont() const {
		return font;
	}

	inline void setText(string newText) {
		this->text = newText;
	}

	void release();
};