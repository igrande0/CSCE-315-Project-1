#ifndef PARSER_H_
#define PARSER_H_

#include <string>
#include <vector>

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
	string get_error();
	bool get_second_word(string FIRST_WORD);
	void add_token(Token token, string s);
	Token get_token(string s);

private:
	void lex(string input);
	void parse();

	int accept(Token);
	int expect(Token, string error);

	bool query();
	bool command();
	void expression();
	void atomic_expr();

	void selection();
	void projection();
	void renaming();

	void condition();
	void conjunction();
	void comparison();
	void op();
	void operand();

	void attribute_list();
	void typed_attribute_list();
	void type();

	vector<Token> tokens;
	vector<string> raw_data;

	int current_index = 0;
};

#endif
