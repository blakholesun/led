#include "Command.h"
#include <iostream>
using namespace std;

int main(){
	std::string input;
	while (cin.get() != '.'){
	cout << ":";
	cin >> input;
    Command c{input};
	c.parse();

    cout << "Command is :" << c.getCommand() << endl;
	cout << "A1 is :" << c.getAddress1() << endl;
	cout << "A2 is :" << c.getAddress2() << endl;
	cout << "Csymbol is :" << c.getCsymbol() << endl;
	cout << "Is Valid? :" << c.isValid() << endl;
	//system("PAUSE");
	}
    return 0;
}