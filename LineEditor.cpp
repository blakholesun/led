#include "LineEditor.h"
#include <fstream>
#include <iostream>
#include <sstream>

//Constructor
LineEditor::LineEditor(const std::string& filename) {
	this->filename = filename;
	// if file doesnt exist create and load empty buffer
	if (!fileExists(filename)) {
		std::cout << "Unable to open " << filename << std::endl;
		std::cout << "\"" << filename << "\"" << "[New File]" << std::endl;
	}
	else {
		// else load all lines to buffer
		loadToBuffer(filename);
		std::cout << "\"" << filename << "\" " << numlines << " lines" << std::endl;
	}
}

// Loads all file lines to the buffer
void LineEditor::loadToBuffer(const std::string& filename) {
	std::fstream fs;
	fs.open(filename, std::fstream::in);

	std::string line;
	while (std::getline(fs, line)) {
		buffer.push_back(line);
		++numlines;
	}
	current = numlines;
}

// Checks file existence
bool LineEditor::fileExists(const std::string& filename) {
	std::ifstream f(filename);
	return f.good();
}

// Main method that controls the text editor
void LineEditor::run() {
	std::cout << "Entering command mode.\n:";
	std::string usercommand;
	//newlineLoop();
	std::getline(std::cin,usercommand);
	//continue running while stream is good
	//usercommand + " ";
	while (std::cin) {

		//if (usercommand.empty()) {
		//	usercommand == "1d";
		//}
		//parse command
		command.setCommand(usercommand);
		command.parse();
		std::string a1 = command.getAddress1();
		std::string a2 = command.getAddress2();
		
		//std::cout << command.getCsymbol();

		char csymbol = command.getCsymbol(); 
		int ad1 = mapAddressString(a1);
		int ad2 = mapAddressString(a2);

		// check if file was empty and only allow q a i
		if (numlines == 0 && command.isValid()){
			if (csymbol == 'a'){
				//std::cout << "IN" << std::endl;
				append(ad1);
				isSaved = false;
			}
			else if (csymbol == 'i'){
				insert(ad1);
				isSaved = false;
			}
			else if (csymbol == 'q'){
				quit();
				break;
			}
			else{
				std::cout << "error: file empty - enter 'q' to quit, 'a' to append or 'i' to insert" << std::endl;
			}
		}
		else if (command.isValid()){
			//command checker

			if (csymbol == 'p'){
				//std::cout << "IN" << std::endl;
				print(ad1, ad2);
			}
			else if (csymbol == 'a'){
				//std::cout << "IN" << std::endl;
				append(ad1);
				isSaved = false;
			}
			else if (csymbol == 'n'){
				//std::cout << "IN" << std::endl;
				nprint(ad1,ad2);
			}
			else if (csymbol == 'i'){
				insert(ad1);
				isSaved = false;
			}
			else if (csymbol == 'r'){
				removel(ad1,ad2);
				isSaved = false;
			}
			else if (csymbol == 'c'){
				change(ad1,ad2);
				isSaved = false;
			}
			else if (csymbol == 'u'){
				up(ad1);
			}
			else if (csymbol == 'd'){
				down(ad1);
			}
			else if (csymbol == 'w'){
				write();
			}
			else if (csymbol == 'q'){
				quit();
				break;
			}
			else if (csymbol == '='){
				std::cout << current << std::endl;
			}
		}
		//std::cout<< "current is : "<< current << std::endl;
		//std::cout<< "numlines is : "<< numlines << std::endl;
		std::cout << ":";
		//newlineLoop();
		//usercommand = " ";
		std::getline(std::cin, usercommand);
	}

}

void LineEditor::newlineLoop(){
	//std::cout << ":";
	std::cin.ignore();
	while (std::cin.get()=='\n'){
		down(1);
		std::cout << ":";
	}
	std::cin.unget();
}

// Mapping for . and $ to current and last line
int LineEditor::mapAddressString(std::string& usercommand) const {
	if (usercommand == ".") {
		return current;
	}
	else if (usercommand == "$") {
		return numlines;
	}
	else {
		return std::stoi(usercommand);
	}
}

// Checks the range of start and end to be within legal 1<x<$
bool LineEditor::checkRange(int& start, int& end) const {
	if (start < 1 || start > numlines || end < 1 || end > numlines) {
		std::cout << "Invalid range. Defaulting to all lines." << std::endl;
		start = 1;
		end = numlines;
		return false;
	}
	return true;
}

// Append function, current is set to last line
void LineEditor::append(int& lineNumber) {
	
	auto it = buffer.begin();
	// Need to go past by 1
	++it;
	auto i = 1;
	while (i<lineNumber){
		++it;
		++i;
	}

	std::string line;
	//std::cin.ignore();
	std::getline(std::cin,line);

	while(line != "."){ 
		buffer.insert(it,line);
		++numlines;
		std::getline(std::cin,line);
		++i;
	}
	current = i;
}

// Insert method, current is set to line before insert
void LineEditor::insert(int& lineNumber) {
	auto it = buffer.begin();
	auto i =1;
	while (i < lineNumber){
		++it;
		++i;
	}

	std::string line;
	//std::cin.ignore();
	std::getline(std::cin,line);

	while(line != "."){ 
		buffer.insert(it,line);
		++numlines;
		std::getline(std::cin,line);
		++i;
	}
	current = i-1;
}

// Remove method, current is set to last line in range or prior if none
void LineEditor::removel(int& start, int& end) {
	if (start < 1 || start > numlines || end < 1 || end > numlines) {
		std::cout << "Invalid range. Not removing any lines." << std::endl;
		start = 1;
		end = numlines;
		return;
	}

	auto it1 = buffer.begin();
	auto it2 = it1;

	std::advance(it1,start-1);
	std::advance(it2,end);
	
	buffer.erase(it1,it2);
	numlines -= end-start+1;
	
	if ( start < numlines ){
		current = start;
	}else{
		current = start-1;
	}
	
}

// Print method, prints a line range, current set to end of range
void LineEditor::print(int& start, int& end) {

	checkRange(start, end);
	int i{ 1 };
	for (auto it = buffer.begin(); it != buffer.end(); ++it) {
		//std::cout << i <<std::endl;
		if (i >= start && i<=end){
			std::cout << *it << std::endl;
		}
		++i;
	}
	current = end;
}

// Print line method, prints the line with numbers, current set to end of range
void LineEditor::nprint(int& start, int& end) {
	checkRange(start, end);
	int i{ 1 };
	for (auto it = buffer.begin(); it != buffer.end(); ++it) {
		if (i >= start && i<=end){
			std::cout << i << "  " << *it << std::endl;
		}
		++i;
	}

	current = end;

}

// Find and replace method, current is set to end of range
void LineEditor::change(int& start, int& end) {
	if (start < 1 || start > numlines || end < 1 || end > numlines) {
		std::cout << "Invalid range. Not changing any lines." << std::endl;
		start = 1;
		end = numlines;
		return;
	}

	std::string text,replace;

	std::cout << "Find: ";
	std::getline(std::cin,text);
	std::cout << "Replace: ";

	std::getline(std::cin,replace);
	int i {1};
	std::size_t found;
	for(auto &line : buffer){
		if (i >= start && i <= end){
			found = line.find(text);
			if (found!=std::string::npos){
				line.replace(found,text.length(),replace);
			}
		}
		++i;
	}
	current = end;

}

// MOves current up by nLines
void LineEditor::up(const int& nLines) {
	if (current - nLines < 1 ){
		std::cout << "BOF reached. Setting current to line 1" << std::endl;
		current = 1;
	} else {
		current -= nLines;
	}
}

// Moves current down by nLines
void LineEditor::down(const int& nLines) {
	if (current + nLines > numlines ){
		std::cout << "EOF reached. Setting current to line " << numlines <<std::endl;
		current = numlines;
	} else {
		current += nLines;
	}
}

// Writes buffer to file
void LineEditor::write() {
	std::cout << filename << std::endl;
	std::ofstream fs{filename};

	if (fs.is_open()){
		for(auto& line : buffer){
			fs << line << std::endl;
		}
	}

	if (!fs.fail()){
		std::cout << "Written to file : " << filename << std::endl;
	} else {
		std::cout << "There was a problem writing to file." << std::endl;
	}

	fs.close();
	isSaved = true;
}

// Quits the program and saves to file if not written.
void LineEditor::quit(){
	if (!isSaved){
		std::cout << "Save changes to " << filename << " (y/n)? ";
		std::string response;
		std::cin>>response;
		while(!(response == "y" || response == "n")){
			std::cout << "Invalid response. Please enter (y/n). "<< std::endl;
			std::cout << "Save changes to " << filename << "(y/n)? ";
			std::cin>>response;
		}

		if(response == "y"){
			write();
		}

	}
}
