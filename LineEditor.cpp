#include "LineEditor.h"
#include <fstream>
#include <iostream>

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

void LineEditor::loadToBuffer(const std::string& filename) {
	std::fstream fs;
	fs.open(filename, std::fstream::in);

	std::string line;
	while (std::getline(fs, line)) {
		buffer.push_back(line);
		++numlines;
	}
	current = numlines-1;
}

bool LineEditor::fileExists(const std::string& filename) {
	std::ifstream f(filename);
	return f.good();
}

void LineEditor::run() {
	std::cout << "Entering command mode.\n:";
	//Keep running until q received

	std::string usercommand;
	std::cin >> usercommand;
	while (usercommand!="q") {
		
		command.setCommand(usercommand);
		command.parse();
		std::string a1 = command.getAddress1();
		std::string a2 = command.getAddress2();
		
		char csymbol = command.getCsymbol(); 
		int ad1 = mapAddressString(a1);
		int ad2 = mapAddressString(a2);

		//std::cout << csymbol << ad1 << ad2 << std::endl;

		if (command.isValid()){

			if (csymbol == 'p'){
				//std::cout << "IN" << std::endl;
				print(ad1, ad2);
			}
			else if (csymbol == 'a'){
				//std::cout << "IN" << std::endl;
				append(ad1);
			}
			else if (csymbol == 'n'){
				//std::cout << "IN" << std::endl;
				nprint(ad1,ad2);
			}
			else if (csymbol == 'i'){
				insert(ad1);
			}
			else if (csymbol == 'r'){
				removel(ad1,ad2);
			}
			else if (csymbol == 'c'){
				change(ad1,ad2);
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
			else if (csymbol == '='){
				std::cout << current << std::endl;
			}
		}
		std::cout << ":";
		std::cin >> usercommand;
	}

}

int LineEditor::mapAddressString(std::string& usercommand) const {
	if (usercommand == ".") {
		return current;
	}
	else if (usercommand == "$") {
		return numlines - 1;
	}
	else {
		return std::stoi(usercommand);
	}
}

bool LineEditor::checkRange(int& start, int& end) const {
	if (start < 0 || start > numlines - 1 || end < 0 || end > numlines - 1) {
		std::cout << "Invalid range. Defaulting to all lines." << std::endl;
		start = 0;
		end = numlines - 1;
		return false;
	}
	return true;
}

void LineEditor::append(int& lineNumber) {
	
	auto it = buffer.begin();
	// Need to go past by 1
	++it;
	auto i = 0;
	while (i<lineNumber){
		++it;
		++i;
	}

	std::string line;
	std::cin.ignore();
	std::getline(std::cin,line);

	while(line != "."){ 
		buffer.insert(it,line);
		++numlines;
		std::getline(std::cin,line);
	}
	current = numlines-1;
}

void LineEditor::insert(int& lineNumber) {
	auto it = buffer.begin();
	auto i = 0;
	while (i<lineNumber){
		++it;
		++i;
	}

	std::string line;
	std::cin.ignore();
	std::getline(std::cin,line);

	while(line != "."){ 
		buffer.insert(it,line);
		++numlines;
		std::getline(std::cin,line);
	}
	current = i;
}

void LineEditor::removel(int& start, int& end) {
	if (start < 0 || start > numlines - 1 || end < 0 || end > numlines - 1) {
		std::cout << "Invalid range. Not removing any lines." << std::endl;
		start = 0;
		end = numlines - 1;
		return;
	}

	auto it1 = buffer.begin();
	auto it2 = it1;

	std::advance(it1,start);
	std::advance(it2,end+1);
	
	buffer.erase(it1,it2);
	numlines -= end-start+1;
	
	if ( end != numlines-1 ){
		current = start-1;
	}else{
		current = start;
	}
	
}

void LineEditor::print(int& start, int& end) {

	checkRange(start, end);
	int i{ 0 };
	for (auto it = buffer.begin(); it != buffer.end(); ++it) {
		//std::cout << i <<std::endl;
		if (i >= start && i<=end){
			std::cout << *it << std::endl;
		}
		++i;
	}
	current = end;
}

void LineEditor::nprint(int& start, int& end) {
	checkRange(start, end);
	int i{ 0 };
	for (auto it = buffer.begin(); it != buffer.end(); ++it) {
		if (i >= start && i<=end){
			std::cout << i << ") " << *it << std::endl;
		}
		++i;
	}

	current = end;

}

void LineEditor::change(int& start, int& end) {
	if (start < 0 || start > numlines - 1 || end < 0 || end > numlines - 1) {
		std::cout << "Invalid range. Not changing any lines." << std::endl;
		start = 0;
		end = numlines - 1;
		return;
	}

	std::string text,replace;
	std::cout << "Find: ";
	std::cin.ignore();
	std::getline(std::cin,text);
	std::cout << "Replace: ";
	//std::cin.ignore();
	std::getline(std::cin,replace);
	
	int i {0};
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

}

void LineEditor::up(int& nLines) {
	if (current - nLines < 0 ){
		std::cout << "BOF reached. Setting current to line 0" << std::endl;
		current = 0;
	} else {
		current -= nLines;
	}
}

void LineEditor::down(int& nLines) {
	if (current + nLines > numlines-1 ){
		std::cout << "EOF reached. Setting current to line " << numlines-1 <<std::endl;
		current = numlines-1;
	} else {
		current += nLines;
	}
}

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
	
}

