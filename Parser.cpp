#include Parser.h
#include <sstream>

void Parser::query(string INPUT_STRING){  //not sure about stringstream, i dont know tokens well enough so this is what i started with - JM
  std::stringstream STRING_STREAM;
  string FIRST_KEYWORD;
  STRING_STREAM << INPUT_STRING;
  STRING_STREAM >> FIRST_KEYWORD;
  string RELATION_TITLE;
  
  /* struct Token { //I'm pretty sure that tokens are what the parser reads in, and then converts it to data compatible with the program. The last time I wrote a parser, the token was similar to this, but feel free to change 
					//if you think it should be done differently
  string name;
  string attribute1;
  string attribute2;
  string attribute3;
  string attribute4;
  
  };*/
  switch(FIRST_KEYWORD){
    case "CREATE":
      STRING_STREAM >> RELATION_TITLE;
      break;
    case "INSERT":
    
      break;
    case "UPDATE":
    
      break;
    case "DELETE":
    
      break;
    case "SHOW":
    
      break;
    case "OPEN":
    
      break;
    case "CLOSE":
    
      break;
    case "WRITE"
    
      break;
    case "EXIT"
    
      break;
  }
}
