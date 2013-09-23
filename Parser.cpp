#include "Parser.h"
#include "Database.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <iostream>
#include <utility>

using namespace std;

bool Parser::execute(string input) {
	bool ret_val = true;;
	cout << "---------------------------------------------------------------\n";
	cout << "\n\nINPUT: " << input << "\n\n";
	try{
		lex(input);
		parse();
	}
	catch(exception& e) {
		cout  << "error: " << e.what() << endl;
		ret_val = false;
	}
	cout << "\n\nTOKENS:\n";
	for(unsigned int i = 0; i < raw_data.size(); ++i)
		cout << '[' << raw_data[i] << ']';
	cout << "\n\nTOKEN ENUMS:\n";
	for(unsigned int i = 0; i < tokens.size(); ++i)
		cout << '[' << token_strings[tokens[i]] << ']';
	cout << "\n\n\n---------------------------------------------------------------\n";
	return ret_val;
}

/*------------------------------------------------------------------------------------*/
/* LEXER */
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
					break;
				case '(':
					SWITCH_STRING.push_back(INPUT_STRING[INDEX]);
					add_token(LPAREN, SWITCH_STRING);
					break;
				case ')':
					SWITCH_STRING.push_back(INPUT_STRING[INDEX]);
					add_token(RPAREN, SWITCH_STRING);
					break;
				case '+':
					SWITCH_STRING.push_back(INPUT_STRING[INDEX]);
					add_token(PLUS, SWITCH_STRING);
					break;
				case '-':
					SWITCH_STRING.push_back(INPUT_STRING[INDEX]);
					add_token(MINUS, SWITCH_STRING);
					break;
				case '*':
					SWITCH_STRING.push_back(INPUT_STRING[INDEX]);
					add_token(TIMES, SWITCH_STRING);
					break;
				case ',':
					SWITCH_STRING.push_back(INPUT_STRING[INDEX]);
					add_token(COMMA, SWITCH_STRING);
					break;
				case ';':
					SWITCH_STRING.push_back(INPUT_STRING[INDEX]);
					add_token(SEMICOLON, SWITCH_STRING);
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
					break;
				case '!':
					SWITCH_STRING.push_back(INPUT_STRING[INDEX]);
					if(INDEX+1 < INPUT_STRING.size() && INPUT_STRING[INDEX +1] == '='){
						SWITCH_STRING.push_back(INPUT_STRING[INDEX+1]);
						add_token(NEQ, SWITCH_STRING);
						INDEX++;
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
					break;
				case '|':
					SWITCH_STRING.push_back(INPUT_STRING[INDEX]);
					if(INDEX+1 < INPUT_STRING.size() && INPUT_STRING[INDEX+1] == '|'){
						SWITCH_STRING.push_back(INPUT_STRING[INDEX+1]);
						add_token(OR,SWITCH_STRING);
						INDEX++;
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
					}

					break;
				case ' ': case '\n': case '\r':
					break;
				default:
					throw runtime_error("Invalid token");
					break;
			}
			INDEX++;
		}
	}	
}

/*------------------------------------------------------------------------------------*/
/* LEXER UTILITIES */
/*------------------------------------------------------------------------------------*/

void Parser::add_token(Token token, string s){
	tokens.push_back(token);
	raw_data.push_back(s);
}

bool Parser::get_second_word(string FIRST_WORD){
	if(FIRST_WORD == "CREATE" || FIRST_WORD == "PRIMARY"
		|| FIRST_WORD == "INSERT" || FIRST_WORD == "VALUES" || FIRST_WORD == "DELETE")
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
/* PARSER RECURSIVE FUNCTIONS */
/*------------------------------------------------------------------------------------*/

int Parser::accept(Token t) {
	if(current_index >= tokens.size())
		throw runtime_error("parser: invalid input");
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
	string expected_name;
	string anon_name;

	if(!accept(IDENTIFIER))
		return false; // not a query
	expected_name = get_previous_data();

	expect(LARROW, "query: expected left arrow");
	anon_name = expression();
	expect(SEMICOLON, "query: expected semicolon");

	db.update_view_name(expected_name, anon_name);
	return true;
}

bool Parser::command() {
	if(accept(OPEN))
		open();
	else if(accept(CLOSE)) 
		close();
	else if(accept(WRITE)) 
		write();
	else if(accept(EXIT))
		db.exit();
	else if(accept(SHOW))
		show();
	else if(accept(CREATE_TABLE))
		create_table();
	else if(accept(UPDATE))
		update();
	else if(accept(INSERT_INTO)) 
		insert_into();
	else if(accept(DELETE_FROM))
		delete_from();
	else 
		return false;
	return true;
}

/* ----------------------------Command Functions---------------------------*/

void Parser::open() {
	expect(IDENTIFIER, "open: expected identifier");
	string file_name = get_previous_data();
	expect(SEMICOLON, "command: expected semicolon");

	ifstream INPUT_FILE;
	INPUT_FILE.open(file_name + ".db");
	if(!INPUT_FILE.is_open())
		throw runtime_error("Open: file failed to open");

	vector<Token> old_tokens = tokens;
	vector<string> old_raw_data = raw_data;

	cout << "===============================================================\n";
	string NEW_LINE;
	getline(INPUT_FILE, NEW_LINE);
	if(execute(NEW_LINE) == false) {
		INPUT_FILE.close();
		tokens = old_tokens;
		raw_data = old_raw_data;
		throw runtime_error("open: relation is already open");
	}
	
	while(!INPUT_FILE.eof()) {
		getline(INPUT_FILE, NEW_LINE);
		execute(NEW_LINE);
	}
	cout << "===============================================================\n";
	INPUT_FILE.close();

	tokens = old_tokens;
	raw_data = old_raw_data;
}

void Parser::close() {
	expect(IDENTIFIER, "close: expected identifier");
	string table_name = get_previous_data();
	expect(SEMICOLON, "command: expected semicolon");
	db.close(table_name);
}

void Parser::write() {
	expect(IDENTIFIER, "write: expected identifier");
	string table_name = get_previous_data();
	expect(SEMICOLON, "command: expected semicolon");
	db.write(table_name);
}

void Parser::show() {
	string table_name = atomic_expr();
	expect(SEMICOLON, "command: expected semicolon");
	db.show(table_name);
}

void Parser::create_table() {
	pair<vector<string>, vector<string> > attributes_types;
	vector<string> keys;
	string table_name;

	expect(IDENTIFIER, "create table: expected identifier");
	table_name = get_previous_data();

	expect(LPAREN, "create table: expected '('");
	attributes_types = typed_attribute_list();
	expect(RPAREN, "create table: expected ')'");


	expect(PRIMARY_KEY, "create table: expected 'PRIMARY KEY'");
	expect(LPAREN, "create table: expected '('");
	keys = attribute_list();
	expect(RPAREN, "create table: expected ')'");
	
	expect(SEMICOLON, "command: expected semicolon");
	db.create(table_name, attributes_types.first, attributes_types.second, keys);
}

void Parser::update() {
	string table_name;
	vector<string> attribute_names;
	vector<string> new_values;
	vector<Token> type;
	vector<vector<string> > table;
	vector<bool> truth_values;

	expect(IDENTIFIER, "update: expected identifier");
	table_name = get_previous_data();

	expect(SET, "update: expected 'SET'");
	do {
		expect(IDENTIFIER, "update: expected identifier");
		attribute_names.push_back(get_previous_data());

		expect(EQUALS, "update: expected '='");
		if(accept(LITERAL)) {
			type.push_back(LITERAL);
			new_values.push_back(get_previous_data());
		}
		else if(accept(INTEGER)) {
			type.push_back(INTEGER);
			new_values.push_back(get_previous_data());
		}
		else
			throw runtime_error("update: expected literal or integer");
	} while (accept(COMMA));

	table = db.get_table(table_name);
	expect(WHERE, "update: expected 'WHERE'");
	truth_values = condition(table);

	expect(SEMICOLON, "command: expected semicolon");

	for(unsigned int i = 0; i < truth_values.size(); ++i) 
		if(truth_values[i] == true)
			db.update(table_name, attribute_names, new_values, i + 3);
}

void Parser::insert_into() {
	string relation_name;

	expect(IDENTIFIER, "insert into: expected identifier");
	relation_name = get_previous_data();

	if(accept(VALUES_FROM)) {
		vector<string> tuple;

		expect(LPAREN, "insert into: expected '('");
		do {
			if(accept(LITERAL))
				tuple.push_back(get_previous_data());
			else if(accept(INTEGER))
				tuple.push_back(get_previous_data());
			else if(accept(MINUS)) {
				expect(INTEGER, "insert into: expected integer");
				string value = "-";
				value += get_previous_data();
				tuple.push_back(value);
			}
			else
				throw runtime_error("insert into: unexpected symbol");
		} while(accept(COMMA));
		expect(RPAREN, "insert into: expected ')'");
		expect(SEMICOLON, "command: expected semicolon");
		db.insert_tuple(relation_name, tuple);
	}
	else if(accept(VALUES_FROM_RELATION)) {
		string view_name = expression();
		expect(SEMICOLON, "command: expected semicolon");
		db.insert_view(relation_name, view_name);
	}
	else
		throw runtime_error("insert into: unexpected symbol");
}

void Parser::delete_from() {
	string relation_name;
	vector<bool> truth_values;
	unsigned int vector_offset = 3;

	expect(IDENTIFIER, "delete from: expected identifier");
	relation_name = get_previous_data();
	const vector<vector<string> >& table = db.get_table(relation_name);

	expect(WHERE, "delete from: expected 'WHERE'");
	truth_values = condition(table);

	for(unsigned int i = 0; i < truth_values.size(); ++i)
		if(truth_values[i] == true) {
			db.remove(relation_name, i + vector_offset);
			--vector_offset;
		}
}

/* -------------------------------Expressions------------------------------*/

string Parser::expression() {
	string view_name;

	if(accept(SELECT)) 
		view_name = selection();
	else if(accept(PROJECT))
		view_name = projection();
	else if(accept(RENAME))
		view_name = renaming();
	else {
		view_name = atomic_expr();
		if(accept(PLUS))
			view_name = set_union(view_name);
		else if(accept(MINUS))
			view_name = set_difference(view_name);
		else if(accept(TIMES))
			view_name = cross_product(view_name);
	}

	return view_name;
}

string Parser::atomic_expr() {
	string view_name;

	if(accept(IDENTIFIER)) {
		view_name = get_previous_data();
	}
	else if(accept(LPAREN)) {
		view_name = expression();
		expect(RPAREN, "atomic expression: expected ')'");
	}
	else
		throw runtime_error("atomic expression: unexpected symbol");

	return view_name;
}

/* -----------------------------Query Functions----------------------------*/

string Parser::selection() {
	string view_name = get_anonymous_view();
	string in_table_name;
	int begin_condition;
	int after_atomic;
	vector<bool> truth_values;

	// first parse the condition without executing any comparisons
	expect(LPAREN, "selection: expected '('");
	begin_condition = current_index;
	condition();
	expect(RPAREN, "selection: expected ')'");
	in_table_name = atomic_expr();
	after_atomic = current_index;

	// now re-parse the condition while doing all comparisons
	current_index = begin_condition;
	const vector<vector<string> >& table = db.get_table(in_table_name);
	truth_values = condition(table);
	
	// call select on every index
	for(unsigned int i = 0; i < truth_values.size(); ++i)
		if(truth_values[i] == true)
			db.select(view_name, in_table_name, i+3);

	current_index = after_atomic;
	return view_name;
}

string Parser::projection() {
	string view_name = get_anonymous_view();
	string in_table_name;
	vector<string> attributes;

	expect(LPAREN, "projection: expected '('");
	attributes = attribute_list();
	expect(RPAREN, "projection: expected ')'");
	in_table_name = atomic_expr();

	db.project(view_name, in_table_name, attributes);

	return view_name;
}

string Parser::renaming() {
	string view_name = get_anonymous_view();
	string in_table_name;
	vector<string> attributes;

	expect(LPAREN, "rename: expected '('");
	attributes = attribute_list();
	expect(RPAREN, "rename: expected ')'");
	in_table_name = atomic_expr();

	db.rename(view_name, in_table_name, attributes);

	return view_name;
}

string Parser::set_union(string left_argument) {
	string view_name = get_anonymous_view();
	string right_argument = atomic_expr(); 

	db.set_union(view_name, left_argument, right_argument);

	return view_name;
}

string Parser::set_difference(string left_argument) {
	string view_name = get_anonymous_view();
	string right_argument = atomic_expr(); 

	db.set_difference(view_name, left_argument, right_argument);

	return view_name;
}

string Parser::cross_product(string left_argument) {
	string view_name = get_anonymous_view();
	string right_argument = atomic_expr(); 

	db.cross_product(view_name, left_argument, right_argument);

	return view_name;
}

/* -------------------------Conditional Statements------------------------*/

vector<bool> Parser::condition(const vector<vector<string> >& table) {
	vector<bool> values;
	if(table.size() > 2) {
		vector<bool> initializer(table.size()-3, false);
		values = initializer;
	}
	
	do {
		vector<bool> new_values = conjunction(table);
		for(unsigned int i = 0; i < values.size() && i < new_values.size(); ++i) 
			values[i] = values[i] || new_values[i];
	} while(accept(OR));
	
	return values;
}

vector<bool> Parser::conjunction(const vector<vector<string> >& table) {
	vector<bool> values;
	if(table.size() > 2) {
		vector<bool> initializer(table.size()-3, true);
		values = initializer;
	}
	
	do {
		vector<bool> new_values = comparison(table);
		for(unsigned int i = 0; i < values.size() && i < new_values.size(); ++i) 
			values[i] = values[i] && new_values[i];
	} while(accept(AND));

	return values;
}

vector<bool> Parser::comparison(const vector<vector<string> >& table) {
	vector<bool> values;
	if(table.size() > 2) {
		vector<bool> initializer(table.size()-3);
		values = initializer;
	}

	if(accept(LPAREN)) {
		values = condition(table);
		expect(RPAREN, "comparison: expected ')'");
	}
	else {
		expect(IDENTIFIER, "comparison: left operand: expected identifier");
		string left_arg = get_previous_data();

		Token op = get_current_token();
		if(!(accept(EQ) || accept(NEQ) || accept(GREATER) || accept(LESS) || accept(LEQ) || accept(GEQ)))
			throw runtime_error("op: unexpected operator");

		Token right_arg_type = get_current_token();
		if(!(accept(IDENTIFIER) || accept(LITERAL) || accept(INTEGER)))
			throw runtime_error("comparison: right operand: unexpected operand");
		string right_arg = get_previous_data();
		
		if(table.size() > 2) {
			// get first attribute index
			int attribute_index1 = -1;
			Token attribute_index1_type;
			for(unsigned int i = 0; i < table[1].size(); ++i)
				if(table[1][i] == left_arg) {
					attribute_index1 = i;
					string type = table[2][i];
					switch(type[0]) {
						case 'I':
							attribute_index1_type = INTEGER;
							break;
						case 'V':
							attribute_index1_type = VARCHAR;
							break;
					}
					break;
				}
			if(attribute_index1 == -1)
				throw runtime_error("comparison: left operand: no such attribute");

			switch(right_arg_type) {
				case IDENTIFIER: {
					// get second attribute index
					int attribute_index2 = -1;
					Token attribute_index2_type;
					for(unsigned int i = 0; i < table[1].size(); ++i)
						if(table[1][i] == right_arg) {
							attribute_index2 = i;
							string type = table[2][i];
							switch(type[0]) {
								case 'I':
									attribute_index2_type = INTEGER;
									break;
								case 'V':
									attribute_index2_type = VARCHAR;
									break;
							}
							break;
						}
					if(attribute_index2 == -1)
						throw runtime_error("comparison: right operand: no such attribute");

					if(attribute_index1_type != attribute_index2_type)
						throw runtime_error("comparison: incompatible attribute types");
				
					// do comparisons on all tuples
					if(attribute_index1_type == INTEGER)
						for(unsigned int i = 3; i < table.size(); ++i) {
							int left = stoi(table[i][attribute_index1]);
							int right = stoi(table[i][attribute_index2]);
							values[i-3] = compare<int>(left, right, op);
						}
					else if(attribute_index1_type == VARCHAR)
						for(unsigned int i = 3; i < table.size(); ++i) {
							string left = table[i][attribute_index1];
							string right = table[i][attribute_index2];
							values[i-3] = compare<string>(left, right, op);
						}
					else
						throw runtime_error("comparison: argument types: unexpected error");

					break;
				}
				case LITERAL: {
					if(attribute_index1_type != VARCHAR)
						throw runtime_error("comparison: incompatible arguments");

					for(unsigned int i = 3; i < table.size(); ++i) 
						values[i-3] = compare<string>(table[i][attribute_index1], right_arg, op);
					break;
				}
				case INTEGER: {
					if(attribute_index1_type != INTEGER)
						throw runtime_error("comparison: incompatible arguments");

					for(unsigned int i = 3; i < table.size(); ++i) {
						int left = stoi(table[i][attribute_index1]);
						int right = stoi(right_arg);
						values[i-3] = compare<int>(left, right, op);
					}
					break;
				}
				default:
					throw runtime_error("comparison: unexpected error");
					break;
			}
		}
	}

	return values;
}

/* ----------------------------   Lists   ---------------------------*/

vector<string> Parser::attribute_list() {
	vector<string> attributes;

	do {
		expect(IDENTIFIER, "attribute list: expected identifier");
		attributes.push_back(get_previous_data());
	} while(accept(COMMA));

	return attributes;
}

pair<vector<string>, vector<string> > Parser::typed_attribute_list() {
	vector<string> attributes;
	vector<string> types;

	do {
		expect(IDENTIFIER, "typed attribute list: expected identifier");
		attributes.push_back(get_previous_data());

		types.push_back(type());
	} while(accept(COMMA));

	pair<vector<string>, vector<string> > return_pair (attributes, types);
	return return_pair;
}

string Parser::type() {
	string type;
	if(accept(VARCHAR)) {
		type += get_previous_data();

		expect(LPAREN, "VARCHAR: expected '('");
		type += get_previous_data();

		expect(INTEGER, "VARCHAR: expected integer");
		type += get_previous_data();

		expect(RPAREN, "VARCHAR: expected ')'");
		type += get_previous_data();
	}
	else if(accept(INTEGER_SYM)) {
		type += get_previous_data();
	}
	else
		throw runtime_error("type: unexpected symbol");
	return type;
}

/*------------------------------------------------------------------------------------*/
/* PARSER UTILITIES */
/*------------------------------------------------------------------------------------*/

template <class T> bool Parser::compare(T left_arg, T right_arg, Token op) {
	bool ret_val;
	
	switch(op) {
		case EQ:
			ret_val = left_arg == right_arg;
			break;
		case NEQ:
			ret_val = left_arg != right_arg;
			break;
		case LESS:
			ret_val = left_arg < right_arg;
			break;
		case GREATER:
			ret_val = left_arg > right_arg;
			break;
		case GEQ:
			ret_val = left_arg >= right_arg;
			break;
		case LEQ:
			ret_val = left_arg <= right_arg;
			break;
		default:
			throw runtime_error("compare: unexpected error");
			break;
	}
	
	return ret_val;
}

Parser::Token Parser::get_current_token() {
	return tokens[current_index];
}

string Parser::get_previous_data() {
	return raw_data[current_index - 1];
}

string Parser::get_anonymous_view() {
	string view_name = to_string(current_anon_view);
	++current_anon_view;
	return view_name;
}
