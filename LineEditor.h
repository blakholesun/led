#ifndef LINEEDITOR_H
#define LINEEDITOR_H

#include "Command.h"
#include <string>
#include <list>

class LineEditor {
public:
	LineEditor()=delete;
	LineEditor(const std::string& filename);
	~LineEditor() = default;
	void run();

private:
	std::string filename;
	std::list<std::string> buffer;
	int current{ 1 };
	int numlines{ 0 };
	Command command;
	bool isSaved{true};

	void newlineLoop();
	void loadToBuffer(const std::string& filename);
	bool fileExists(const std::string& filename);
	int mapAddressString(std::string&) const;
	bool checkRange(int& start, int& end) const;
	void append(int& lineNumber);
	void insert(int& lineNumber);
	void removel(int& start, int& end);
	void print(int& start, int& end);
	void nprint(int& start, int& end);
	void change(int& start, int& end);
	void up(const int& nLines);
	void down(const int& nLines);
	void write();
	void quit();
};

#endif // !LINEEDITOR_H
