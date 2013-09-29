#include "Application.h"

Application::Application() {
	vector<string> keys {
		"bank name, bank routing number",
		"account number, account type",
		"name, birthdate",
		"person's name, bank name",
		"person's name, account number",
		"bank name, account number"};	
	vector<vector<string> > attribute_types {
		{"VARCHAR(255)", "INT", "VARCHAR(255)"},
		{"INTEGER", "VARCHAR(255)", "INTEGER"},
		{"VARCHAR(255)", "VARCHAR(255)", "VARCHAR(255)", "VARCHAR(255)"},
		{"VARCHAR(255)", "VARCHAR(255)"},
		{"VARCHAR(255)", "INTEGER"},
		{"VARCHAR(255)", "INTEGER"}};

	// either open all tables or create all tables and save to disk
	for(unsigned int i = 0; i < tables.size(); ++i)
		if(!parser.execute("OPEN " + tables[i] + ";")) {
			// construct CREATE TABLE command
			string create = "CREATE TABLE " + tables[i] + "(";
			for(unsigned int j = 0; j < attributes[i].size(); ++j){
				create += attributes[i][j] + " " + attribute_types[i][j];
				if(j != attributes[i].size()-1)
					create += ", ";
			}
			create += ") PRIMARY KEY (" + keys[i] +");";

			parser.execute(create);
			parser.execute("WRITE " + tables[i] + ";");
		}
}

Application::~Application() {
	// save all tables to disk
	for(unsigned int i = 0; i < tables.size(); ++i) 
		parser.execute("WRITE " + tables[i] + ";");

	// clear all open tables
	parser.execute("EXIT;");
}

void Application::main_menu() {
	int input;
	display_main_menu();
	cin >> input;
	if (input == 1)
	{
		//call update
	}
	else if (input == 2)
	{
		//call update
	}
	else if (input == 3)
	{
		//call update
	}
	else if (input == 4)
	{
		//call update
	}
	else if (input == 5)
	{
		//call update
	}
	else if (input == 6)
	{
		//call update
	}
	// LOOP
	// display menu
	// take in valid option #
	// call table_menu(tables[i-1])
}

void Application::table_menu(string table) {
	
	// LOOP
	// display menu
	// take in valid option #
	// call add(), remove(), or update() with correct tables[i] and corresponding attributes[i]
}

void Application::display_main_menu() {
	// CHANGE THESE -  maybe? I think they sound stupid - Isaac
	cout << "What would you like to do? Enter the number of your choice.\n";
	cout << "(1) modify the bank list\n";
	cout << "(2) modify the person list\n";
	cout << "(3) modify the account list\n";
	cout << "(4) modify the enrollment list\n";
	cout << "(5) modify the authorization list\n";
	cout << "(6) modify the account host list\n";
	// ADD queries somehow?
}

void Application::display_table_menu(string table) {
	// CHANGE - these won't make sense and will need to be fixed somehow
	int choice; //not sure if this is the best way, but it's all I know
	string tablename;
	cout << "(1) Add a table\n";
	cout << "(2) Remove a table\n";
	cout << "(3) Update a table\n";
	cout << "Indicate what you would like to do:\n";
	cin >> choice;
	if (choice == 1)
	{
		cout << "Please input the table name:\n";
		cin >> tablename;
		add(tablename, attributes);
	}
	else if (choice == 2)
	{
		cout << "Please input the table name:\n";
		cin >> tablename;
		remove(tablename, attributes);
	}
	else if (choice == 3)
	{
		cout << "Please input the table name:\n";
		update(table, attributes);
	}
	else
	{
		throw runtime_error("Invalid Input");
	}
	
	/*cout << "How would you like to modify the" << table << " list? Enter the number of your choice.\n";
	cout << "(1) add a " << table << "\n";
	cout << "(2) remove a" << table << "\n";
	cout << "(3) update a" << table << "\n";*/
}

void Application::add(string table, vector<string> attributes){
	// EXAMPLE: INSERT INTO baseball_players VALUES FROM ("Alexander", "Smith", "Pirates", 2, 150000);
	// ask for every attribute
	// construct/call INSERT INTO
	// WRITE
}

void Application::remove(string table, vector<string> attributes){
	// EXAMPLE: DELETE FROM dots WHERE (y1 <= 0);
	// loop to ask what conditions
	// construct/call DELETE FROM
	// WRITE
}

void Application::update(string table, vector<string> attributes){
	// EXAMPLE: UPDATE dots SET x1 = 0 WHERE x1 < 0;
	// loop to ask what conditions
	// loop to ask what to change
	// construct/call UPDATE
	// WRITE
}
