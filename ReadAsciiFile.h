#pragma once

#include <vector>
#include <fstream>
#include "ColorChar.h"

static std::vector<std::vector<std::vector<ColorChar>>> readAsciiFile(std::string fileName){
	std::vector<std::vector<std::vector<ColorChar>>> output;
	
	std::ifstream file(fileName);
	
	if(!file){
		std::cout << "Can not open file \"" << fileName << "\"!\n";
		return output;
	}
	
	ColorChar current;
	bool lastNew = false;
	
	std::vector<std::vector<ColorChar>> currentFrame;
	std::vector<ColorChar> currentLine;
	
	while(file.get(current.character)){
		if(current.character == '\033'){
			file.get(current.character);
			std::string colorS;
			file.get(current.character);//load the first character
			if(current.character == '0'){
				file.get(current.character);
				if(current.character == 'm'){
					colorS = "97";
				}
				else{
					file.get(current.character);
				}
			}
			for(; current.character != 'm'; file.get(current.character)){
				colorS += current.character;
			}
			current.color = std::stoi(colorS);
		}
		else if(current.character == '\n'){
			if(lastNew){
				output.push_back(currentFrame);
				currentFrame.clear();
			}
			else{
				currentFrame.push_back(currentLine);
				currentLine.clear();
			}
			lastNew = true;
		}
		else{
			lastNew = false;
			currentLine.push_back(current);
		}
	}
	
	return output;
}