#include "LineEditor.h"
#include <iostream>

int main(int argc, char *argv[]) {
	
	std::string filename;
	switch (argc){
		case 1:
			break;
		case 2:
			filename = argv[1];
			break;
		default:
			std::cout<< ("too many arguments - all discarded") << std::endl;
			filename.clear();
			break;
	}
	LineEditor le{ filename };
	le.run();
	//system("PAUSE");
	return 0;
}