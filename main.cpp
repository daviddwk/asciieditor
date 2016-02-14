#include <iostream>

#include "Editor.h"
#include "getch.h"

int main(){
    
    char input;
    std::cout << "Open file or create new?(o/n)";
    input = getch();
    if(input == 'o'){
		std::cout << "Filename: ";
		std::string fileName;
		std::cin >> fileName;
		initEditor(fileName);
    }
    else if(input == 'n'){
		std::string newHeightS;
		std::string newWidthS;
		std::string newFramesS;
		int newHeight = -1;
		int newWidth = -1;
		int newFrames = -1;
		std::cout << "New Height:";
		std::cin >> newHeightS;
		try{
			newHeight = std::stoi(newHeightS);
		}
		catch(std::invalid_argument e){
		}
		if(newHeight < 1){
			std::cout << "\aInvalid Height\n";
			return -1;
		}
		else{
			std::cout << "New Width:";
			std::cin >> newWidthS;
			try{
				newWidth = std::stoi(newWidthS);
			}
			catch(std::invalid_argument e){
			}
			if(newWidth < 1){
				std::cout << "\aInvalid Width\n";
				return -1;
			}
			else{
				std::cout << "New Frame Count:";
				std::cin >> newFramesS;
				try{
					newFrames = std::stoi(newFramesS);
				}
				catch(std::invalid_argument e){
				}
				if(newFrames < 1){
					std::cout << "\aInvalid Frame Count\n";
					return -1;
				}
				else{
					initEditor(newHeight, newWidth, newFrames);
				}
			}
		}
    }
    else{
		return -1;
    }
    
    editorLoop();
    destroyEditor();
    
    
    return 0;
}
