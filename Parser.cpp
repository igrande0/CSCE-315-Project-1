#include "Parser.h"

#include <sstream>
#include <stdexcept>
#include <string>
#include <iostream>

using namespace std;

void Parser::execute(string input) {
	lex(input);
	cout << "\n\nTOKEN ENUMS:\n";
	for(unsigned int i = 0; i < tokens.size(); ++i)
		cout << '[' << token_strings[tokens[i]] << ']';
	cout << "\n\nPARSER:\n";
	parse();
	cout << "input successfully  parsed";
}

/*------------------------------------------------------------------------------------*/
/* LEXER FUNCTIONS */
/*------------------------------------------------------------------------------------*/

void Parser::lex(string INPUT_STRING) {
	raw_data.clear();
	tokens.clear();
	for(unsigned int INDEX = 0; INDEX<INPUT_STRING.size();){
		unsigned int START_INDEX = INDEX;
		bool check = false;
		while(isalpha(INPUT_STRING[INDEX]) || isdigit(INPUT_STRING[INDEX]) || INPUT_STRING[INDEX] == '_'){
			if(INDEX < INPUT_STRING.size())
				INDEX++;
			check = true;
		}
		unsigned int END_INDEX = INDEX - 1;
		string TEMP_STRING;
		if(check == true){
			for(unsigned int i=START_INDEX; i<=END_INDEX; i++)
				TEMP_STRING.push_back(INPUT_STRING[i]);
			if(get_second_word(TEMP_STRING)){
				TEMP_STRING.push_back(' ');
				INDEX++;
				while(isalpha(INPUT_STRING[INDEX])){
					TEMP_STRING.push_back(INPUT_STRING[INDEX]);
					if(INDEX < INPUT_STRING.size())
						INDEX++;
				}
				if(INDEX+1 < INPUT_STRING.size() && INPUT_STRING[INDEX+1] == 'R'){
					TEMP_STRING.push_back(' ');
					INDEX++;
					while(isalpha(INPUT_STRING[INDEX])){
						TEMP_STRING.push_back(INPUT_STRING[INDEX]);
						if(INDEX < INPUT_STRING.size())
							INDEX++;
					}
					
				}
					
			}
			add_token(get_token(TEMP_STRING), TEMP_STRING);
			cout << '[' << TEMP_STRING << ']';
		}
		else{	
			string SWITCH_STRING;
			switch(INPUT_STRING[INDEX]){
				case '<':
					SWITCH_STRING.push_back(INPUT_STRING[INDEX]);
					if(INDEX+1 < INPUT_STRING.size() && INPUT_STRING[INDEX+1] == '='){
						SWITCH_STRING.push_back(INPUT_STRING[INDEX+1]);
						add_token(LEQ, SWITCH_STRING);
						INDEX++;
					}
					else if(INDEX+1 < INPUT_STRING.size() && INPUT_STRING[INDEX+1] == '-'){
						SWITCH_STRING.push_back(INPUT_STRING[INDEX+1]);
						add_token(LARROW, SWITCH_STRING);
						INDEX++;
					}
					else
						add_token(LESS, SWITCH_STRING);
					cout << '[' << SWITCH_STRING << ']';
					break;
				case '(':
					SWITCH_STRING.push_back(INPUT_STRING[INDEX]);
					add_token(LPAREN, SWITCH_STRING);
					cout << '[' << SWITCH_STRING << ']';
					break;
				case ')':
					SWITCH_STRING.push_back(INPUT_STRING[INDEX]);
					add_token(RPAREN, SWITCH_STRING);
					cout << '[' << SWITCH_STRING << ']';
					break;
				case '+':
					SWITCH_STRING.push_back(INPUT_STRING[INDEX]);
					add_token(PLUS, SWITCH_STRING);
					cout << '[' << SWITCH_STRING << ']';
					break;
				case '-':
					SWITCH_STRING.push_back(INPUT_STRING[INDEX]);
					add_token(MINUS, SWITCH_STRING);
					cout << '[' << SWITCH_STRING << ']';
					break;
				case '*':
					SWITCH_STRING.push_back(INPUT_STRING[INDEX]);
					add_token(TIMES, SWITCH_STRING);
					cout << '[' << SWITCH_STRING << ']';
					break;
				case ',':
					SWITCH_STRING.push_back(INPUT_STRING[INDEX]);
					add_token(COMMA, SWITCH_STRING);
					cout << '[' << SWITCH_STRING << ']';
					break;
				case ';':
					SWITCH_STRING.push_back(INPUT_STRING[INDEX]);
					add_token(SEMICOLON, SWITCH_STRING);
					cout << '[' << SWITCH_STRING << ']';
					break;
				case '=':
					SWITCH_STRING.push_back(INPUT_STRING[INDEX]);
					if(INDEX+1 < INPUT_STRING.size() && INPUT_STRING[INDEX+1] == '='){
						SWITCH_STRING.push_back(INPUT_STRING[INDEX+1]);
						add_token(EQ,SWITCH_STRING);
						INDEX++;
					}
					else
						add_token(EQUALS, SWITCH_STRING);
					cout << '[' << SWITCH_STRING << ']';
					break;
				case '!':
					SWITCH_STRING.push_back(INPUT_STRING[INDEX]);
					if(INDEX+1 < INPUT_STRING.size() && INPUT_STRING[INDEX +1] == '='){
						SWITCH_STRING.push_back(INPUT_STRING[INDEX+1]);
						add_token(NEQ, SWITCH_STRING);
						INDEX++;
						cout << '[' << SWITCH_STRING << ']';
					}
					else
						throw runtime_error("Invalid char after '!'");
					break;
				case '>':
					SWITCH_STRING.push_back(INPUT_STRING[INDEX]);
					if(INDEX+1 < INPUT_STRING.size() && INPUT_STRING[INDEX+1] == '='){
						SWITCH_STRING.push_back(INPUT_STRING[INDEX+1]);
						add_token(GEQ,SWITCH_STRING);
						INDEX++;
					}
					else
						add_token(GREATER, SWITCH_STRING);
					cout << '[' << SWITCH_STRING << ']';
					break;
				case '|':
					SWITCH_STRING.push_back(INPUT_STRING[INDEX]);
					if(INDEX+1 < INPUT_STRING.size() && INPUT_STRING[INDEX+1] == '|'){
						SWITCH_STRING.push_back(INPUT_STRING[INDEX+1]);
						add_token(OR,SWITCH_STRING);
						INDEX++;
						cout << '[' << SWITCH_STRING << ']';
					}
					else
						throw runtime_error("Invalid char after '|'");
					break;
				case '&':
					SWITCH_STRING.push_back(INPUT_STRING[INDEX]);
					if(INDEX+1 < INPUT_STRING.size() && INPUT_STRING[INDEX+1] == '&'){
						SWITCH_STRING.push_back(INPUT_STRING[INDEX+1]);
						add_token(AND,SWITCH_STRING);
						INDEX++;
						cout << '[' << SWITCH_STRING << ']';
					}
					else
						throw runtime_error("Invalid char after '&'");
					break;
				case '"':
					INDEX++;
					while(INPUT_STRING[INDEX] != '"' && INDEX < INPUT_STRING.size()){
						SWITCH_STRING.push_back(INPUT_STRING[INDEX]);
						INDEX++;
					}
					if(INPUT_STRING[INDEX] != '"')
						throw runtime_error("Expected end of quotations");
					else {
						add_token(LITERAL, SWITCH_STRING);
						cout << "[\"" << SWITCH_STRING << "\"]";
					}

					break;
				case ' ': case '\n': case '\r':
					break;
				default:
					//cout<<INPUT_STRING[INDEX]<<endl;
					throw runtime_error("Invalid token");
					break;
			}
			INDEX++;
			//cout << raw_data[raw_data.size()-1] << "   |   ";
		}
	}	
}

void Parser::add_token(Token token, string s){
	tokens.push_back(token);
	raw_data.push_back(s);
}

bool Parser::get_second_word(string FIRST_WORD){
	if(FIRST_WORD == "CREATE" || FIRST_WORD == "PRIMARY" || FIRST_WORD == "INSERT" || FIRST_WORD == "VALUES" || FIRST_WORD == "DELETE")
		return(true);
	else
		return(false);
}

Parser::Token Parser::get_token(string s){
	if(s == "select")
		return SELECT;
	else if(s == "project")
		return PROJECT;
	else if(s== "rename")
		return RENAME;
	else if(s == "OPEN")
		return OPEN;
	else if(s == "CLOSE")
		return CLOSE;
	else if(s == "WRITE")
		return WRITE;
	else if(s == "EXIT")
		return EXIT;
	else if(s == "SHOW")
		return SHOW;
	else if(s == "UPDATE")
		return UPDATE;
	else if(s == "SET")
		return SET;
	else if(s == "WHERE")
		return WHERE;
	else if (s == "CREATE TABLE")
		return CREATE_TABLE;
	else if (s == "PRIMARY KEY")
		return PRIMARY_KEY;
	else if (s == "INSERT INTO")
		return INSERT_INTO;
	else if (s == "VALUES FROM")
		return VALUES_FROM;
	else if (s == "VALUES FROM RELATION")
		return VALUES_FROM_RELATION;
	else if (s == "DELETE FROM")
		return DELETE_FROM;
	else if(s == "VARCHAR")
		return VARCHAR;
	else if(s == "INTEGER")
		return INTEGER_SYM;
	else if(isdigit(s[0]))			///to_string(stoi(s)) == s
		return INTEGER;
	else if(isalpha(s[0]))
		return IDENTIFIER;
	else
		throw runtime_error("Invalid token");
}




/*------------------------------------------------------------------------------------*/
/* RECURSIVE DESCENT PARSER */
/*------------------------------------------------------------------------------------*/

int Parser::accept(Token t) {
	if(tokens[current_index] == t) {
		++current_index;
		return 1;
	}
	return 0;
}

int Parser::expect(Token t, string error) {
	if(accept(t)) 
		return 1;
	throw runtime_error(error);
	return 0;
}

void Parser::parse() {
	current_index = 0;
	if(!query() && !command())
		throw runtime_error("parser: unexpected symbol");
}


bool Parser::query() {
	if(!accept(IDENTIFIER))
		return false; // not a query
	expect(LARROW, "query: expected left arrow");
	expression();
	expect(SEMICOLON, "query: expected semicolon");
	return true;
}

bool Parser::command() {
	if(accept(OPEN)) {
		expect(IDENTIFIER, "open: expected identifier");
		string file_name = get_previous_data();
		ifstream INPUT_FILE;
		INPUT_FILE.open(file_name + ".db");
		if(!INPUT_FILE.is_open())
			throw runtime_error("Open: file failed to open");
		string NEW_LINE;
		while(getline(INPUT_FILE, NEW_LINE))
			execute(NEW_LINE);
		INPUT_FILE.close();
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
		expect(PRIMARY_KEY, "create table: expected 'PRIMARY KEY'");
		expect(LPAREN, "create table: expected '('");
		attribute_list();
		expect(RPAREN, "create table: expected ')'");
	}
	else if(accept(UPDATE)) {
		expect(IDENTIFIER, "update: expected identifier");
		expect(SET, "update: expected 'SET'");
		do {
			expect(IDENTIFIER, "update: expected identifier");
			expect(EQUALS, "update: expected '='");
			if(accept(LITERAL)) {

			}
			else if(accept(INTEGER)) {

			}
			else
				throw runtime_error("update: expected literal or integer");
		} while (accept(COMMA));
		expect(WHERE, "update: expected 'WHERE'");
		condition();
	}
	else if(accept(INSERT_INTO)) {
		expect(IDENTIFIER, "insert into: expected identifier");
		if(accept(VALUES_FROM)) {
			expect(LPAREN, "insert into: expected '('");
			do {
				if(accept(LITERAL)) {
				
				}
				else if(accept(INTEGER)) {
				
				}
				else if(accept(MINUS)) {
					expect(INTEGER, "insert into: expected integer");
				}
				else
					throw runtime_error("insert into: unexpected symbol");
			} while(accept(COMMA));
			expect(RPAREN, "insert into: expected ')'");
		}
		else if(accept(VALUES_FROM_RELATION)) {
			expression();
		}
		else
			throw runtime_error("insert into: unexpected symbol");
	}
	else if(accept(DELETE_FROM)) {
		expect(IDENTIFIER, "delete from: expected identifier");
		expect(WHERE, "delete from: expected 'WHERE'");
		condition();
	}
	else 
		return false;
	expect(SEMICOLON, "command: expected semicolon");
	return true;
}

void Parser::expression() {
	if(accept(SELECT)) 
		selection();
	else if(accept(PROJECT))
		projection();
	else if(accept(RENAME))
		renaming();
	else {
		atomic_expr();
		if(accept(PLUS)) 
			atomic_expr();
		else if(accept(MINUS)) 
			atomic_expr();
		else if(accept(TIMES)) 
			atomic_expr();
	}
}

void Parser::atomic_expr() {
	if(accept(IDENTIFIER)) {

	}
	else if(accept(LPAREN)) {
		expression();
		expect(RPAREN, "atomic expression: expected ')'");
	}
	else
		throw runtime_error("atomic expression: unexpected symbol");
}

void Parser::selection() {
	expect(LPAREN, "selection: expected '('");
	condition();
	expect(RPAREN, "selection: expected ')'");
	atomic_expr();
}

void Parser::projection() {
	expect(LPAREN, "projection: expected '('");
	attribute_list();
	expect(RPAREN, "projection: expected ')'");
	atomic_expr();
}

void Parser::renaming() {
	expect(LPAREN, "rename: expected '('");
	attribute_list();
	expect(RPAREN, "rename: expected ')'");
	atomic_expr();
}

void Parser::condition() {
	do {
		conjunction();
	} while(accept(OR));
}

void Parser::conjunction() {
	do {
		comparison();
	} while(accept(AND));
}

void Parser::comparison() {
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

void Parser::operand() {
	if(accept(IDENTIFIER)) {

	}
	else if(accept(LITERAL)) {

	}
	else if(accept(INTEGER)) {

	}
	else
		throw runtime_error("operand: unexpected operand");
}

void Parser::op() {
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

vector<string> Parser::attribute_list() {
	vector<string> attributes;

	do {
		expect(IDENTIFIER, "attribute list: expected identifier");
		attributes.push_back(raw_data[current_index-1]);
	} while(accept(COMMA));

	return attributes;
}

void Parser::typed_attribute_list() {
	vector<string> attributes;
	vector<string> types;

	do {
		expect(IDENTIFIER, "typed attribute list: expected identifier");
		type();
	} while(accept(COMMA));
}

string Parser::type() {
	string type = "";
	if(accept(VARCHAR)) {
		expect(LPAREN, "VARCHAR: expected '('");
		type += raw_data[current_index - 1];

		expect(INTEGER, "VARCHAR: expected integer");
		type += get_previous_data();;

		expect(RPAREN, "VARCHAR: expected ')'");
		type += raw_data[current_index - 1];
	}
	else if(accept(INTEGER_SYM)) {
		type += raw_data[current_index - 1];
	}
	else
		throw runtime_error("type: unexpected symbol");
}
