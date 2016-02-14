#pragma once

#include <string>
#include <stdio.h>
#include <fstream>
#include <stdexcept>
#include "getch.h"
#include "ColorChar.h"
#include "ReadAsciiFile.h"

struct EditorState{
	std::string fileName;
	
	std::size_t frames;
	std::size_t height;
	std::size_t width;
	
	int currentFrame;
	int currentX;
	int currentY;

	char currentColor;
	
	bool painting;
	
	ColorChar*** charInfo;//Animation, X, Y
};
static EditorState editorState;

static void initEditor(std::string fileName){
	std::vector<std::vector<std::vector<ColorChar>>> charMap = readAsciiFile(fileName);
	
	editorState.painting = false;
	editorState.frames = charMap.size();
	editorState.height = charMap[0].size();
	editorState.width = charMap[0][0].size();
	
	editorState.currentFrame = 0;
	editorState.currentX = 0;
	editorState.currentY = 0;
	
	editorState.currentColor = 97;
	editorState.fileName = fileName;
	
	editorState.charInfo = new ColorChar**[editorState.frames];
	for(int f = 0; f < editorState.frames; f++){
		editorState.charInfo[f] = new ColorChar*[editorState.width];
		for(int x = 0; x < editorState.width; x++){
			editorState.charInfo[f][x] = new ColorChar[editorState.height];
		}
	}
	
	for(int f = 0; f < editorState.frames; f++){
		for(int x = 0; x < editorState.width; x++){
			for(int y = 0; y < editorState.height; y++){
				editorState.charInfo[f][x][y] = charMap[f][y][x];
			}
		}
	}
	
}

static void initEditor(int height, int width, int frames){
	editorState.painting = false;
	editorState.fileName = "newAscii.ascii";
	editorState.frames = frames;
	editorState.height = height;
	editorState.width = width;
	
	editorState.currentFrame = 0;
	editorState.currentX = 0;
	editorState.currentY = 0;
	
	editorState.currentColor = 97;//97
	
	editorState.charInfo = new ColorChar**[frames];
	for(int f = 0; f < frames; f++){
		editorState.charInfo[f] = new ColorChar*[width];
		for(int x = 0; x < width; x++){
			editorState.charInfo[f][x] = new ColorChar[height];
		}
	}
}

static void printEditor(){
	std::cout << "\033[2J\033[1;1H";

	printf("\033[0;97mF: %i X: %i Y: %i\n\033[1;97m", editorState.currentFrame, editorState.currentX + 1, editorState.currentY + 1);//print header
	
	for(int i = 0; i < editorState.width; i++){
		std::cout << '-';
	}
	
	std::cout << "--" << std::endl;
	
	for(int y = 0; y < editorState.height; y++){
	std::cout << "\033[1;97m|";
		for(int x = 0; x < editorState.width; x++){
			if(x == editorState.currentX && y == editorState.currentY){
				if(editorState.charInfo[editorState.currentFrame][x][y].character != ' '){
					std::cout << "\033[1;" << (int)editorState.charInfo[editorState.currentFrame][x][y].color << 'm' << editorState.charInfo[editorState.currentFrame][x][y].character;
				}
				else{
					std::cout << "\033[0;97m*";				
				}
			}
			else{
				std::cout << editorState.charInfo[editorState.currentFrame][x][y];
			}
		}
		std::cout << "\033[1;97m|" << std::endl;
	}
	for(int i = 0; i < editorState.width; i++){
		std::cout << '-';
	}
	std::cout << "--" << std::endl;
}

static void resizeEditor(std::size_t height, std::size_t width, std::size_t frames){
	ColorChar*** newChar = new ColorChar**[frames];
	for(int f = 0; f < frames; f++){
		newChar[f] = new ColorChar*[width];
		for(int x = 0; x < width; x++){
			newChar[f][x] = new ColorChar[height];
		}
	}
	
	for(int f = 0; f < ((frames <= editorState.frames)?frames:editorState.frames); f++){
		for(int x = 0; x < ((width <= editorState.width)?width:editorState.width); x++){
			for(int y = 0; y < ((height <= editorState.height)?height:editorState.height); y++){
				newChar[f][x][y] = editorState.charInfo[f][x][y];
			}
		}
	}
	
	for(int f = 0; f < editorState.frames; f++){
		for(int x = 0; x < editorState.width; x++){
			delete[] editorState.charInfo[f][x];
		}
		delete[] editorState.charInfo[f];
	}
	delete[] editorState.charInfo;
	
	editorState.charInfo = newChar;
	
	editorState.height = height;
	editorState.width = width;
	editorState.frames = frames;
}

static void saveEditorToFile(){
	std::ofstream file(editorState.fileName);
	
	char last = 0;
	
	for(int f = 0; f < editorState.frames; f++){
		for(int y = 0; y < editorState.height; y++){
			for(int x = 0; x < editorState.width; x++){
				if(editorState.charInfo[f][x][y].character == ' '){
					file << editorState.charInfo[f][x][y].character;
				}
				else{
					if(editorState.charInfo[f][x][y].color == last){
						file << editorState.charInfo[f][x][y].character;
					}
					else{
						file << "\033[" << (int)editorState.charInfo[f][x][y].color << 'm' << editorState.charInfo[f][x][y].character;
						last = editorState.charInfo[f][x][y].color;
					}
				}
			}
			file << std::endl;
		}
		file << std::endl;
	}
}

static void editorLoop(){
	while(true){
		printEditor();
		char input = getch();
		
		if(input == '\033'){//if recieved escape
			input = getch();
			if(input == '['){//if recieved arrow input
				input = getch();
				switch(input) {
					case 'A'://up
						editorState.currentY--;
						break;
					case 'B'://down
						editorState.currentY++;
						break;
					case 'C'://right
						editorState.currentX++;
						break;
					case 'D'://left
						editorState.currentX--;
						break;
					default:
						std::cout << '\a';
						break;
				}
			}
			else{
				switch(input){
					case 'q':
						return;
						break;
					case '\033'://received move frame
					{
							getch();
							input = getch();
							if(input == 'C'){
								editorState.currentFrame++;
							}
							else if(input == 'D'){
								editorState.currentFrame--;
							}
							else{
								std::cout << '\a';
							}
						break;
					}
					case 'f'://change foreground color
					{
						std::cout << "Black:30 \033[0;31;40mRed:31 \033[0;32;40mGreen:32 \033[0;33;40mYellow:33\n\033[0;34;40mBlue:34 \033[0;35;40mMagenta:35 \033[0;36;40mCyan:36 \033[0;37;40mLightGray:37\n\033[0;90;40mDarkGray:90 \033[0;91;40mLightRed:91 \033[0;92;40mLightGreen:92 \033[0;93;40mLightYellow:93\n\033[0;94;40mLightBlue:94 \033[0;95;40mLightMagenta:95 \033[0;96;40mLightCyan:96 \033[0;97;40mWhite:97\n";
						std::cout << "What to change the foreground color to:";
						std::string input;
						std::cin >> input;
						try{
							editorState.currentColor = std::stoi(input);
						}
						catch(std::invalid_argument e){
							std::cout << "\a";
						}
						break;
					}
					case 'r':
					{	
						std::string newHeightS;
						std::string newWidthS;
						std::string newFramesS;
						int newHeight = -1;
						int newWidth = -1;
						int newFrames = -1;
						printf("Height:%i Width:%i Frames:%i\n", editorState.height, editorState.width, editorState.frames);
						std::cout << "New Height:";
						std::cin >> newHeightS;
						try{
							newHeight = std::stoi(newHeightS);
						}
						catch(std::invalid_argument e){
						}
						if(newHeight < 1){
							std::cout << "\aInvalid Height\n";
							getch();
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
								getch();
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
									getch();
								}
								else{
									resizeEditor(newHeight, newWidth, newFrames);
								}
							}
						}
						break;
					}
					case 'i':
						for(int y = 0; y < editorState.height - 1; y++){
							for(int x = 0; x < editorState.width; x++){
								editorState.charInfo[editorState.currentFrame][x][y] = editorState.charInfo[editorState.currentFrame][x][y + 1];
								if(y == editorState.height - 2){
									editorState.charInfo[editorState.currentFrame][x][y + 1] = ColorChar();
								}
							}
						}
						break;
					case 'j':
						for(int y = 0; y < editorState.height; y++){
							for(int x = 0; x < editorState.width - 1; x++){
								editorState.charInfo[editorState.currentFrame][x][y] = editorState.charInfo[editorState.currentFrame][x + 1][y];
								if(x == editorState.width - 2){
									editorState.charInfo[editorState.currentFrame][x + 1][y] = ColorChar();
								}
							}
						}
						break;
					case 'k':
						for(int y = editorState.height - 1; y > 0; y--){
							for(int x = 0; x < editorState.width; x++){
								editorState.charInfo[editorState.currentFrame][x][y] = editorState.charInfo[editorState.currentFrame][x][y - 1];
								if(y == 1){
									editorState.charInfo[editorState.currentFrame][x][y - 1] = ColorChar();
								}
							}
						}
						break;
					case 'l':
						for(int y = 0; y < editorState.height; y++){
							for(int x = editorState.width - 1; x > 0; x--){
								editorState.charInfo[editorState.currentFrame][x][y] = editorState.charInfo[editorState.currentFrame][x - 1][y];
								if(x == 1){
									editorState.charInfo[editorState.currentFrame][x - 1][y] = ColorChar();
								}
							}
						}
						break;
					case 'c':
					{
						std::string sourceS;
						std::string destinationS;
						int source = -1;
						int destination = -1;
					
						std::cout << "Copy frame source:";
						std::cin >> sourceS;
						try{
							source = std::stoi(sourceS);
						}
						catch(std::invalid_argument e){
						}
						if(source < 0 || source >= editorState.frames){
							std::cout << "\aInvalid Frame!\n";
							getch();
						}
						else{
							std::cout << "Destination:";
							std::cin >> destinationS;
							try{
								destination = std::stoi(destinationS);
							}
							catch(std::invalid_argument e){
							}
							if(destination < 0 || destination >= editorState.frames){
								std::cout << "\aInvalid Frame!\n";
								getch();
							}
							else{
								for(int x = 0; x < editorState.width; x++){
									for(int y = 0; y < editorState.height; y++){
										editorState.charInfo[destination][x][y] = editorState.charInfo[source][x][y];
									}
								}
							}
						}
						break;
					}
					case 'w':
					{
						std::cout << "Are you sure you want to save?(y/n)\n";
						input = getch();
						if(input == 'y'){
							std::cout << "Save to \"" << editorState.fileName << "\"?(y/n)\n";
							input = getch();
							if(input != 'y'){
								std::cout << "New Filename:";
								std::string fileName;
								std::cin >> fileName;
								editorState.fileName = fileName;
							}
							saveEditorToFile();
						}
						break;
					}
					case 'p':
						editorState.painting = !editorState.painting;
						break;
					default:
						std::cout << '\a';
						break;
				}
			}
			//stop the editor from selecting non existant characters
			if(editorState.currentFrame < 0){
				editorState.currentFrame = 0;
			}
			if(editorState.currentFrame >= editorState.frames){
				editorState.currentFrame = editorState.frames - 1;
			}
			
			if(editorState.currentX < 0){
				editorState.currentX = 0;
			}
			if(editorState.currentX >= editorState.width){
				editorState.currentX = editorState.width - 1;
			}
			if(editorState.currentY < 0){
				editorState.currentY = 0;
			}
			if(editorState.currentY >= editorState.height){
				editorState.currentY = editorState.height - 1;
			}
			
		}
		else if(input == '\n'){//apply foreground color
			editorState.charInfo[editorState.currentFrame][editorState.currentX][editorState.currentY].color = editorState.currentColor;
		}
		else{//write character to ascii
			if(input == 127 || input == '\b'){
				input = ' ';
			}
			editorState.charInfo[editorState.currentFrame][editorState.currentX][editorState.currentY].character = input;
			editorState.charInfo[editorState.currentFrame][editorState.currentX][editorState.currentY].color = 97;
		}
		if(editorState.painting){
			editorState.charInfo[editorState.currentFrame][editorState.currentX][editorState.currentY].color = editorState.currentColor;
		}
	}
}

static void destroyEditor(){
	for(int f = 0; f < editorState.frames; f++){
		for(int x = 0; x < editorState.width; x++){
			delete[] editorState.charInfo[f][x];
		}
		delete[] editorState.charInfo[f];
	}
	delete[] editorState.charInfo;
}