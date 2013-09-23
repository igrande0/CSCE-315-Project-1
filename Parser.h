#ifndef PARSER_H_
#define PARSER_H_

#include "Database.h"

#include <string>
#include <vector>
#include <utility>

using namespace std;

class Parser{

public:
	enum Token {
		// symbols
		// <-     (        )      +     -     *      ,        ;       =
		LARROW, LPAREN, RPAREN, PLUS, MINUS, TIMES, COMMA, SEMICOLON, EQUALS,
		//== !=    <      >      <=   >=  ||   &&
		EQ, NEQ, LESS, GREATER, LEQ, GEQ, OR, AND,
		
		// keywords
		SELECT, PROJECT, RENAME, OPEN, CLOSE, WRITE, EXIT, SHOW,
		CREATE_TABLE, PRIMARY_KEY, UPDATE, SET, WHERE, INSERT_INTO,
		VALUES_FROM, VALUES_FROM_RELATION, DELETE_FROM, VARCHAR, INTEGER_SYM,

		// others
		// INTEGER - any amount of digits
		// IDENTIFIER - starts with alpha, then any amount of alpha, digit, or "_"
		// LITERAL - "(any amount of characters)"
		INTEGER, IDENTIFIER, LITERAL
	};
	const char* token_strings [39] = {
		// symbols
		"LARROW", "LPAREN", "RPAREN", "PLUS", "MINUS", "TIMES", "COMMA", "SEMICOLON", "EQUALS",
		"EQ", "NEQ", "LESS", "GREATER", "LEQ", "GEQ", "OR", "AND",
		
		// keywords
		"SELECT", "PROJECT", "RENAME", "OPEN", "CLOSE", "WRITE", "EXIT", "SHOW",
		"CREATE_TABLE", "PRIMARY_KEY", "UPDATE", "SET", "WHERE", "INSERT_INTO",
		"VALUES_FROM", "VALUES_FROM_RELATION", "DELETE_FROM", "VARCHAR", "INTEGER_SYM",

		// others
		"INTEGER", "IDENTIFIER", "LITERAL"
	};
	void execute(string input);
	bool get_second_word(string FIRST_WORD);
	void add_token(Token token, string s);
	Token get_token(string s);

private:
	// Parser functions
	void lex(string input);
	void parse();

	int accept(Token);
	int expect(Token, string error);

	bool query();
	bool command();
	string expression();
	string atomic_expr();

	string selection();
	string projection();
	string renaming();

	vector<bool> condition(vector<vector<string> > tuples = vector<vector<string> >());
	vector<bool> conjunction(vector<vector<string> > tuples = vector<vector<string> >());
	vector<bool> comparison(vector<vector<string> > tuples = vector<vector<string> >());
	void op();
	void operand();

	vector<string> attribute_list();
	pair<vector<string>, vector<string> > typed_attribute_list();
	string type();

	// Uitilities
	string get_previous_data();
	string get_anonymous_view();

	// Data
	vector<Token> tokens;
	vector<string> raw_data;

	int current_index = 0;
	int current_anon_view = 0;

	Database db;
};

#endif
