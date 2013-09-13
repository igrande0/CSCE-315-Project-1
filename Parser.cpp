#include Parser.h
#include <sstream>

void Parser::execute(string input) {
	lex(input);
	parse();
}

void Parser::lex(string INPUT_STRING){  //not sure about stringstream, i dont know tokens well enough so this is what i started with - JM
  std::stringstream STRING_STREAM;
  string FIRST_KEYWORD;
  STRING_STREAM << INPUT_STRING;
  STRING_STREAM >> FIRST_KEYWORD;
  string RELATION_TITLE;
  int RELATION_INDEX;
  

  switch(FIRST_KEYWORD){
    case "CREATE":
      STRING_STREAM >> RELATION_TITLE;
	  RELATION_INDEX = DB.get_realtion_index(RELATION_TITLE);
      break;
    case "INSERT":
		string SECOND_KEYWORD;
		STRING_STREAM >> SECOND_KEYWORD;
		if(SECOND_KEYWORD == "INTO"){
			STRING_STREAM >> RELATION_TITLE;
			RELATION_INDEX = DB.get_relation_index(RELATION_TITLE);
		}
    
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

void Parser::parse() {

}
