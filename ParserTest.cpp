#include "Parser.h"
#include "Database.h"

#include <iostream>

int main() {
	string line;
	Parser p;
	
	while(getline(cin,line))
		p.execute(line);
}
