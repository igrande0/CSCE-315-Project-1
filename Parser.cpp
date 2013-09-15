#include Parser.h
#include <sstream>
#include <stdexcept>

void Parser::execute(string input) {
	lex(input);
	parse();
}

/*------------------------------------------------------------------------------------*/
/* LEXER FUNCTIONS */
/*------------------------------------------------------------------------------------*/

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

/*------------------------------------------------------------------------------------*/
/* RECURSIVE DESCENT PARSER */
/*------------------------------------------------------------------------------------*/

int accept(Token t) {
	if(tokens[current_index] == t) {
		++current_index;
		return 1;
	}
	return 0;
}

int expect(Token t, string error) {
	if(accept(t)) 
		return 1;
	throw runtime_error(error);
	return 0;
}

void Parser::parse() {
	query();
	command();
}


void Parser::query() {
	if(!accept(IDENTIFIER))
		return; // not a query
	expect(LARROW, "query: expected left arrow");
	expression();
	expect(SEMICOLON, "query: expected semicolon");
}

void Parser::command() {
	if(accept(OPEN)) {
		expect(IDENTIFIER, "open: expected identifier");
	}
	else if(accept(CLOSE)) {
		expect(IDENTIFIER, "close: expected identifier");
	}
	else if(accept(WRITE)) {
		expect(IDENTIFIER, "write: expected identifier");
	}
	else if(accept(EXIT)) {

	}
	else if(accept(SHOW)) {
		atomic_expr();
	}
	else if(accept(CREATE_TABLE)) {
		expect(IDENTIFIER, "create table: expected identifier");
		expect(LPAREN, "create table: expected '('");
		typed_attribute_list();
		expect(RPAREN, "create table: expected ')'");
		expect(WHERE, "create table: expected 'WHERE'");
		condition();
	}
	else if(accept(UPDATE)) {
		expect(IDENTIFIER, "update: expected identifier");
		expect(SET, "update: expected 'SET'");
		do {
			expect(IDENTIFIER, "update: expected identifier");
			expect(EQUALS, "update: expected '='");
			expect(LITERAL, "update: expected literal");
		} while (accept(COMMA));
		expect(WHERE, "update: expected 'WHERE'");
		condition();
	}
	else if(accept(INSERT_INTO)) {
		expect(IDENTIFIER, "accept: expected identifier");
		if(accept(VALUES_FROM)) {
			expect(LPAREN, "accept: expected '('");
			attribute_list();
			expect(RPAREN, "accept: expected ')'");
		}
		else(accept(VALUES_FROM_RELATION)) {
			expression();
		}
	}
	else if(accept(DELETE_FROM)) {
		expect(IDENTIFIER, "delete from: expected identifier");
		expect(WHERE, "delete from: expected 'WHERE'");
		condition();
	}
	else 
		throw runtime_error("parser: unexpected symbol");
	expect(SEMICOLON, "query: expected semicolon");
}

void expression() {
	if(accept(SELECTION)) 
		selection();
	else if(accept(PROJECTION))
		projection();
	else if(accept(RENAMING))
		renaming();
	else if(accept(UNION))
		unions();
	else if(accept(DIFFERENCE))
		difference();
	else if(accept(PRODUCT))
		product();
	atomic_expr();
}

void atomic_expr() {
	if(accept(IDENTIFIER)) {

	}
	else if(accept(LPAREN)) {
		expression();
		expect(RPAREN, "atomic expression: expected ')'");
	}
	else
		throw runtime_error("atomic expression: unexpected symbol");
}

void selection() {
	expect(LPAREN, "selection: expected '('");
	condition();
	expect(RPAREN, "selection: expected ')'");
	atomic_expr();
}

void projection() {
	expect(LPAREN, "projection: expected '('");
	attribute_list();
	expect(RPAREN, "projection: expected ')'");
	atomic_expr();
}

void renaming() {
	expect(LPAREN, "projection: expected '('");
	attribute_list();
	expect(RPAREN, "projection: expected ')'");
	atomic_expr();
}

void unions() {
	atomic_expr();
	expect(PLUS, "union: expected '+'");
	atomic_expr();
}

void difference() {
	atomic_expr();
	expect(MINUS, "difference: expected '-'");
	atomic_expr();
}

void product() {
	atomic_expr();
	expect(TIMES, "difference: expected '*'");
	atomic_expr();
}

void condition() {
	do {
		conjunction();
	} while(accept(OR));
}

void conjunction() {
	do {
		comparison();
	} while(accept(AND));
}

void comparison() {
	if(accept(LPAREN)) {
		condition();
		expect(RPAREN, "comparison: expected ')'");
	}
	else {
		operand();
		op();
		operand();
	}
}

void operand() {
	if(accept(IDENTIFIER)) {

	}
	else if(accept(LITERAL)) {

	}
	else
		throw runtime_error("operand: unexpected operand");
}

void op() {
	if(accept(EQ)) {

	}
	else if(accept(NEQ)) {

	}
	else if(accept(GREATER)) {

	}
	else if(accept(LESS)) {

	}
	else if(accept(LEQ)) {

	}
	else if(accept(GEQ)) {

	}
	else
		throw runtime_error("op: unexpected operator");
}

void attribute_list() {
	do {
		expect(IDENTIFIER, "attribute list: expected identifier");
	} while(accept(COMMA));
}

void typed_attribute_list() {
	do {
		expect(IDENTIFIER, "typed attribute list: expected identifier");
		type();
	} while(accept(COMMA));
}

void type() {
	if(accept(VARCHAR)) {
		expect(LPAREN, "VARCHAR: expected '('");
		expect(INTEGER, "VARCHAR: expected integer");
		expect(RPAREN, "VARCHAR: expected ')'");
	}
	else if(accept(INTEGER)) {

	}
	else
		throw runtime_error("type: unexpected symbol");
}
