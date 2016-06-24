#include "LineEditor.h"
#include <iostream>

int main() {
	
	std::string filename;
	std::cout << "Enter filename: " << filename << std::endl;
	std::cin >> filename;

	LineEditor le{ filename };
	le.run();
	//system("PAUSE");
	return 0;
}