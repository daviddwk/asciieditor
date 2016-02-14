#pragma once

struct ColorChar{
	friend std::ostream& operator<<(std::ostream& os, const ColorChar cc);

	char color;
	char character;
	
	ColorChar():
	color(97),
	character(' '){};
};

std::ostream& operator<<(std::ostream& os, const ColorChar cc){
    os << "\033[0;" << (int)cc.color << 'm' << cc.character;
    return os;
}