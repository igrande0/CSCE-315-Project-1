#include Database.h

class Parser{

public:
	enum Token {
		// symbols
		LARROW, RARROW, LPAREN, RPAREN, PLUS, MINUS, MULT, COMMA,
		EQ, NEQ, LESS, GREATER, LEQ, GEQ, OR, AND,
		
		// keywords
		SELECT, PROJECT, RENAME, OPEN, CLOSE, WRITE, EXIT, SHOW,
		CREATE_TABLE, PRIMARY_KEY, UPDATE, SET, WHERE, INSERT_INTO,
		VALUES_FROM, VALUES_FROM_RELATION, DELETE_FROM, VARCHAR, INTEGER_SYM,

		// others
		INTEGER, IDENTIFIER, LITERAL
	};
	void execute(string input);

private:
	void lex(string input);
	void parse();

	vector<string> tokens;
	vector<string> raw_data;

	int current_index;
}
