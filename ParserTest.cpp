#include "Parser.h"
#include <iostream>
int main() {
	string line;
	Parser p;
	while(getline(cin,line)){
		try{
			
			p.execute(line);
		}
	
		catch(exception& e){
			cerr << e.what() <<endl;
		}

	}

}