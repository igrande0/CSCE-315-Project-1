#include "Parser.h"
#include <iostream>
int main() {
	string line;
	Parser p;
	cout << "---------------------------------------------------------------\n";
	while(getline(cin,line)){
		cout << "INPUT: " << line << "\n\n";
		cout << "TOKENS: \n";
		try{
			p.execute(line);
		}
		catch(exception& e){
			cerr << "error: " << e.what() <<endl;
		}
		cout << "\n\n\n---------------------------------------------------------------\n";
	}
}