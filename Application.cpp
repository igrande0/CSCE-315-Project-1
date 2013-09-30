#include "Application.h"
//possible querys
//show people not enrolled in a bank?
//show people that are enrolled in a bank
//
Application::Application() {
	vector<string> keys {
		"name, routing_number",
		"number, type",
		"full_name, birthdate",
		"client_name, bank_name",
		"client_name, account_number",
		"bank_name, account_number"};	

	// either open all tables or create all tables and save to disk
	for(unsigned int i = 0; i < tables.size(); ++i)
		if(!parser.execute("OPEN " + tables[i] + ";")) {
			// construct CREATE TABLE command
			string create = "CREATE TABLE " + tables[i] + " (";
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

#define BANK 1
#define ACCOUNT 2
#define client 3

void Application::main_menu() {
	while(true) {
		char choice;
		const char ASCII_offset = 'a';

		display_main_menu();
		cin >> choice;
		// ignore any trailing characters
		cin.ignore(1000,'\n');

		switch(choice) {
		case 'a': case 'b': case 'c':
		case 'd': case 'e': case 'f':
			table_menu(choice-ASCII_offset);
			break;
		case 'g':
			enroll_all_clients();
			break;
		case 'h':
			enrolled_people();
			break;
		case 'q':
			return;
			break;
		default:
			cout << "Invalid option. Please enter another option.\n";
			break;
		}
		// add QUERIES somehow
	}
}

void Application::table_menu(int t_index) {
	while(true) {
		char choice;

		display_table_menu(tables[t_index]);
		cin >> choice;
		// ignore any trailing characters
		cin.ignore(1000,'\n');

		switch(choice) {
		case 'a':
			cout << '\n';
			parser.execute("SHOW " + tables[t_index] + ";");
			break;
		case 'b':
			add(tables[t_index], attributes[t_index], attribute_types[t_index]);
			break;
		case 'c':
			remove(tables[t_index], attributes[t_index], attribute_types[t_index], table_keys[t_index]);
			break;
		case 'd':
			update(tables[t_index], attributes[t_index], attribute_types[t_index], table_keys[t_index]);
			break;
		case 'q':
			return;
			break;
		default:
			cout << "Invalid option. Please enter another option.\n";
			break;
		}
	}
}

void Application::display_main_menu() {
	// CHANGE THESE -  maybe? I think they sound stupid - Isaac
	cout << "\n\nWhat would you like to do? Enter the letter of your choice.\n";
	cout << "Enter 'q' at any time to exit this menu.\n";
	cout << "(a) modify or show the bank entity\n";
	cout << "(b) modify or show the account entity\n";
	cout << "(c) modify or show the client entity\n";
	cout << "(d) modify or show the bank enrollment relation\n";
	cout << "(e) modify or show the account authorization relation\n";
	cout << "(f) modify or show the account host relation\n";
	cout << "(g) enroll all existing clients into an existing bank\n";
	cout << "(h) show all enrolled people\n";
	// ADD queries somehow?
}

void Application::display_table_menu(string table) {
	cout << "\n\nHow would you like to modify the " << table << " list? Enter the letter of your choice.\n";
	cout << "Enter 'q' at any time to exit this menu.\n";
	cout << "(a) show all " << table << "s\n";
	if(table == "account" || table == "enrollment" || table == "authorization") {
		cout << "(b) add an " << table << "\n";
		cout << "(c) remove an " << table << "\n";
		cout << "(d) update an " << table << "\n";
	}
	else {
		cout << "(b) add a " << table << "\n";
		cout << "(c) remove a " << table << "\n";
		cout << "(d) update a " << table << "\n";
	}
}

void Application::add(string table, vector<string> table_attributes, vector<string> attribute_types){
	// EXAMPLE: INSERT INTO baseball_players VALUES FROM ("Alexander", "Smith", "Pirates", 2, 150000);
	// ask for every attribute
	// construct/call INSERT INTO
	// WRITE

	vector<string> user_attributes(table_attributes.size());

	// collect attributes from user
	for(unsigned int i = 0; i < table_attributes.size(); ++i) {
		cout << "Enter the " << table << " " << table_attributes[i] << ": ";
		getline(cin, user_attributes[i]);
	}

	// construct INSERT INTO command
	string command = "INSERT INTO " + table + " VALUES FROM (";
	for(unsigned int i = 0; i < table_attributes.size(); ++i) {
		if(attribute_types[i] == "INTEGER")
			command += user_attributes[i];
		else
			command += "\"" + user_attributes[i] + "\"";
		if(i != table_attributes.size()-1)
			command += ",";
	}
	command += ");";

	parser.execute(command);
	parser.execute("WRITE " + table + ";");

}

void Application::remove(string table, vector<string> table_attributes,
						 vector<string> attribute_types, vector<string> keys){
	// EXAMPLE: DELETE FROM dots WHERE (y1 <= 0);
	// loop to ask what conditions
	// construct/call DELETE FROM
	// WRITE

	vector<string> user_attributes(keys.size());

	// show existing table
	cout << "\n\nExisting " << table << "s:\n";
	parser.execute("SHOW " + table + ";");
	cout << "\n";

	// gather tuple info
	for(unsigned int i = 0; i < keys.size(); ++i) {
		cout << "Enter the " << keys[i] 
				<< " of the " << table << " you want to remove: ";
		getline(cin, user_attributes[i]);
	}

	// construct DELETE FROM command
	string command = "DELETE FROM " + table + " WHERE (";
	for(unsigned int i = 0; i < keys.size(); ++i) {
		for(unsigned int j = 0; j < table_attributes.size(); ++j)
			if(table_attributes[j] == keys[i]) {
				if(attribute_types[j] == "INTEGER")
					command += table_attributes[j] + " == " + user_attributes[i] + " ";
				else
					command += table_attributes[j] + " == \"" + user_attributes[i] + "\" ";
				if(i != keys.size()-1)
					command += " && ";
			}
	}
	command += ");";

	parser.execute(command);
	parser.execute("WRITE " + table + ";");
}

void Application::update(string table, vector<string> attributes,
						 vector<string> attribute_types, vector<string> keys){
	// EXAMPLE: UPDATE dots SET x1 = 0 WHERE x1 < 0;
	// loop to ask what conditions
	// loop to ask what to change
	// construct/call UPDATE
	// WRITE
	vector<string> user_attributes(keys.size());

	// show existing table
	cout << "\n\nExisting " << table << "s:\n";
	parser.execute("SHOW " + table + ";");
	cout << "\n";

	// gather tuple info
	for(unsigned int i = 0; i < keys.size(); ++i) {
		cout << "Enter the " << keys[i] 
				<< " of the " << table << " you want to update: ";
		getline(cin, user_attributes[i]);
	}
	cout<<"Enter the type of attribute you want to change: ";
	string ATT;
	getline(cin, ATT);
	cout<<endl<<"Enter the new value of this attribute: ";
	string NEW_VALUE;
	getline(cin, NEW_VALUE);
	cout<<endl;

	// construct UPDATE command
	string command = "UPDATE " + table + " SET " + ATT + "=\"" + NEW_VALUE + "\" WHERE ";
	for(unsigned int i=0; i<keys.size(); i++){
		if(attribute_types[i] == "INTEGER")
			command += keys[i] + "==" + user_attributes[i];
		else
			command += keys[i] + "==\"" + user_attributes[i] + "\"";
		if(i != keys.size()-1)
			command += " && ";
	}
	command += ";";
	cout<<command<<endl;
	parser.execute(command);
	parser.execute("WRITE " + table + ";");
}

void Application::enroll_all_clients() {
	// cross product of bank and clients
	// selection of a certain bank
	// projection of enrollment relation's fields
	// insert view
	string bank_name;
	string routing_number;

	// show existing table
	cout << "\n\nExisting banks:\n";
	parser.execute("SHOW bank;");
	cout << "\n";

	cout << "Enter the name of the bank you want to enroll clients in: ";
	getline(cin, bank_name);
	cout << "Enter the routing number of the bank you want to enroll clients in: ";
	getline(cin, routing_number);

	parser.execute("all_banks_clients <- client * bank;");
	parser.execute("single_bank_clients <- select (name == \"" + 
					bank_name + "\" && routing_number == " + routing_number + ") all_banks_clients;");
	parser.execute("two_columns <- project (full_name, name) single_bank_clients;");
	parser.execute("INSERT INTO enrollment VALUES FROM RELATION two_columns;");
	parser.execute("WRITE enrollment;");
	parser.execute("CLOSE all_banks_clients;");
	parser.execute("CLOSE single_bank_clients;");
	parser.execute("CLOSE two_columns;");
}

void Application::enrolled_people(){
	parser.execute("proj_people <- project (full_name, name) client;");
	parser.execute("enrolled_people <- project (client_name, name) bank_enrollment;");
	//set union of proj_people and enrolled_people

	parser.execute("SHOW proj_people;"); //display result table
	parser.execute("CLOSE proj_people;");
	parser.execute("CLOSE enrolled_people;");
}


