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
	bool execute(string input);
	

private:
	/*------------------- LEXER FUNCTIONS -------------------*/
	void lex(string input);

	// Lexer Utilities
	bool get_second_word(string FIRST_WORD);
	void add_token(Token token, string s);
	Token get_token(string s);

	/*-------------- PARSER RECURSIVE FUNCTIONS --------------*/
	void parse();

	int accept(Token);
	int expect(Token, string error);

	bool query();
	bool command();

	// Command Functions
	void open();
	void close();
	void write();
	void show();
	void create_table();
	void update();
	void insert_into();
	void delete_from();

	// Expressions
	string expression();
	string atomic_expr();

	// Query Functions
	string selection();
	string projection();
	string renaming();
	string set_union(string left_argument);
	string set_difference(string left_argument);
	string cross_product(string left_argument);

	// Conditional Statements
	vector<bool> condition(const vector<vector<string> >& table = vector<vector<string> >());
	vector<bool> conjunction(const vector<vector<string> >& table = vector<vector<string> >());
	vector<bool> comparison(const vector<vector<string> >& table = vector<vector<string> >());

	// Lists
	vector<string> attribute_list();
	pair<vector<string>, vector<string> > typed_attribute_list();
	string type();

	/*----------------- PARSER UTILITIES -----------------*/
	template <class T> bool compare(T left_arg, T right_arg, Token op);
	string get_previous_data();
	string get_anonymous_view();
	Token get_current_token();

	/*-------------------- PARSER DATA -------------------*/
	vector<Token> tokens;
	vector<string> raw_data;

	unsigned int current_index = 0;
	int current_anon_view = 0;

	Database db;
};

#endif
