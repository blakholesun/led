#ifndef COMMAND_H
#define COMMAND_H
#include <string>

class Command
{

public:

	Command() = default;
	Command(std::string);
	~Command() = default;
	void parse();
	void setCommand(std::string&);
	std::string getCommand() const;
	std::string getAddress1() const;
	std::string getAddress2() const;
	char getCsymbol() const;
	bool isValid() const;
	std::string trim(std::string&);

private:

	std::string command{ "" };
	std::string address1{ "." };
	std::string address2{ "." };
	char csymbol{ 'p' }; //default is p
	bool valid{ false };
};

#endif