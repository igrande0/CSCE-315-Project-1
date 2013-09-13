#include Parser.h
#include <sstream>

void Parser::query(string INPUT_STRING){  //not sure about stringstream, i dont know tokens well enough so this is what i started with - JM
  std::stringstream STRING_STREAM;
  string FIRST_KEYWORD;
  STRING_STREAM << INPUT_STRING;
  STRING_STREAM >> FIRST_KEYWORD;
  string RELATION_TITLE;
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
