#include "Command.h"
#include <string>
#include <assert.h>
#include <iostream>
#include <exception>

Command::Command(std::string command) : command(command) {}

void Command::parse()
{
	// Check  for empty string
	if (command.empty()) { return; }

	// Trim the command
	std::string newCommand{ trim(command) };

	// determine if only valid symbols are present
	std::size_t found_invalid = newCommand.find_first_not_of("airpncudwq.$,0123456789=");
	if (found_invalid != std::string::npos) {
		std::cout << "Invalid Command"<< std::endl;
		valid = false;
		return;
	}

	// Look for first command characters
	std::size_t found_symbol = newCommand.find_first_of("airpncudwq=");
	std::size_t found_other = newCommand.find_last_of("airpncudwq=");
	
	if (found_symbol != found_other){
		std::cout << "Invalid Command"<< std::endl;
		valid = false;
		return;
	}
	else if (found_symbol != std::string::npos) {
		csymbol = command[found_symbol];
	}

	//look for occurrence of comma if none 
	std::size_t found_comma = newCommand.find_first_of(",");
	std::size_t found_comma_other = newCommand.find_last_of(",");
	std::size_t lone_number = newCommand.find_first_of("0123456789");
	if (found_comma != found_comma_other){
		std::cout << "Invalid Command"<< std::endl;
		valid = false;
		return;
	}
	else if (found_comma != std::string::npos) {
		address1 = newCommand.substr(0, found_comma);
		address2 = newCommand.substr(found_comma + 1, newCommand.length() - 2 - found_comma);
	}
	else if (found_symbol != std::string::npos) {
		address1 = newCommand.substr(0, found_symbol);
		address2 = address1;
	}
	else if (lone_number != std::string::npos){
		address1 = newCommand.front();
		address2 = address1;
	}
	
	// store default address if empty
	if (address1.empty() && address2.empty()) {
		address1 = ".";
		address2 = ".";
	}
	else if (address1.empty()) {
		if (address2 =="$"){
			address1 = "$";
		}else{
			address1 = ".";
		}
	}
	else if (address2.empty()) {
		if (address1 =="$"){
			address2 = "$";
		}else{
			address2 = ".";
		}
	}


	// Set address validity to true
	valid = true;

}

std::string Command::getCommand() const
{
	return command;
}

void Command::setCommand(std::string& command)
{
	this->command = command;
	address1 = ".";
	address2 = address1;
	csymbol = 'p';
	valid = false;
}

std::string Command::getAddress1() const
{
	return address1;
}

std::string Command::getAddress2() const
{
	return address2;
}

char Command::getCsymbol() const
{
	return csymbol;
}
bool Command::isValid() const
{
	return valid;
}

std::string Command::trim(std::string& str)
{
	size_t first = str.find_first_not_of(' ');
	size_t last = str.find_last_not_of(' ');
	return str.substr(first, (last - first + 1));
}