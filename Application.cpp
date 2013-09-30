#include "Application.h"

Application::Application() {
	/*vector<string> keys {
		"bank_name, bank routing_number",
		"account_number, account_type",
		"name, birthdate",
		"client_name, bank_name",
		"client_name, account_number",
		"bank_name, account_number"};	*/

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

/*int Application::main_menu(){
	cout<<"Welcome to the Bank Databasing System!"<<endl;
	cout<<"Please enter the integer corresponding to the entity you would like to add, change, ect.:"<<endl;
	cout<<"Bank:\t1"<<endl;
	cout<<"Account:\t2"<<endl;
	cout<<"client:\t3"<<endl;
	int MENU;
	cin>>MENU;
}

void Application::second_menu(int type){
	cout<<"Enter integer of desired option"<<endl;
	cout<<"Add:\t1"<<endl;
	cout<<"Remove:\t2"<<endl;
	cout<<"Update:\t3"<<endl;
	int MENU;
	cin>>MENU;
	switch(MENU){
	case 1:
		switch(type){
			case BANK:
				add_bank();
				break;
			case ACCOUNT:
				add_account();
				break;
			case client:
				add_client();
				break;
		}
		break;
	case 2:
		switch(type){
			case BANK:
				remove_bank();
				break;
			case ACCOUNT:
				remove_account();
				break;
			case client:
				remove_client();
				break;
		}
		break;
	case 3:
		switch(type){
			case BANK:
				update_bank();
				break;
			case ACCOUNT:
				update_account();
				break;
			case client:
				update_client();
				break;
		}
		break;
	}
}*/

void Application::main_menu() {
	while(true) {
		char choice;
		const char ASCII_offset = '0';
		display_main_menu();
		cin >> choice;

		switch(choice) {
		case '1': case '2': case '3':
		case '4': case '5': case '6':
			table_menu(tables[choice-ASCII_offset-1], choice-ASCII_offset-1);
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

void Application::table_menu(string table, int attribute_index) {
	while(true) {
		char choice;
		const char ASCII_offset = '0';
		display_table_menu(table);
		cin >> choice;

		switch(choice) {
		case '1':
			cout << choice-ASCII_offset-1;
			add(table, attributes[attribute_index], attribute_types[attribute_index]);
			break;
		case '2':
			cout << choice-ASCII_offset-1;
			remove(table, attributes[attribute_index], attribute_types[attribute_index]);
			break;
		case '3':
			cout << choice-ASCII_offset-1;
			update(table, attributes[attribute_index], attribute_types[attribute_index]);
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
	cout << "\n\nWhat would you like to do? Enter the number of your choice.\n";
	cout << "Enter 'q' at any time to exit this menu.\n";
	cout << "(1) modify the bank entity\n";
	cout << "(2) modify the account entity\n";
	cout << "(3) modify the client entity\n";
	cout << "(4) modify the bank enrollment relation\n";
	cout << "(5) modify the account authorization relation\n";
	cout << "(6) modify the account host relation\n";
	// ADD queries somehow?
}

void Application::display_table_menu(string table) {
	cout << "\n\nHow would you like to modify the " << table << " list? Enter the number of your choice.\n";
	cout << "Enter 'q' at any time to exit this menu.\n";
	cout << "(1) add a " << table << "\n";
	cout << "(2) remove a " << table << "\n";
	cout << "(3) update a " << table << "\n";
}

void Application::add(string table, vector<string> table_attributes, vector<string> attribute_types){
	vector<string> user_attributes(table_attributes.size());
	
	// ignore any trailing characters
	cin.ignore(1000,'\n');

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
	// EXAMPLE: INSERT INTO baseball_players VALUES FROM ("Alexander", "Smith", "Pirates", 2, 150000);
	// ask for every attribute
	// construct/call INSERT INTO
	// WRITE
}

void Application::remove(string table, vector<string> table_attributes, vector<string> attribute_types){
	// EXAMPLE: DELETE FROM dots WHERE (y1 <= 0);
	// loop to ask what conditions
	// construct/call DELETE FROM
	// WRITE
	vector<string> user_attributes(table_attributes.size());

	for(unsigned int i = 0; i < table_attributes.size(); ++i) {
		cout << "Enter the " << table << " " << table_attributes[i] << " you want to remove: ";
		cin >> user_attributes[i];
	}

	// construct INSERT INTO command
	string command = "DELETE FROM " + table + " WHERE ";
	for(unsigned int i = 0; i < table_attributes.size(); ++i) {
		if(attribute_types[i] == "INTEGER")
			command += table_attributes[i] + "=" + user_attributes[i];
		else
			command += table_attributes[i] + "=\"" + user_attributes[i] + "\"";
		if(i != table_attributes.size()-1)
			command += " AND ";
	}
	command += ";";

	parser.execute(command);
	parser.execute("WRITE " + table + ";");
}

void Application::update(string table, vector<string> attributes, vector<string> attribute_types){
	// EXAMPLE: UPDATE dots SET x1 = 0 WHERE x1 < 0;
	// loop to ask what conditions
	// loop to ask what to change
	// construct/call UPDATE
	// WRITE
}

/*
void Application::add_bank(){
	string BANK_NAME;
	string ROUTING_NUMBER;
	string ADDRESS;
	cout<<"Enter the name of new bank: ";
	cin<<BANK_NAME;
	cout<<endl<<"Enter the banks routing number: ";
	cin>>ROUTING_NUMBER;
	cout<<endl<<"Enter the banks address: ";
	cin>>ADDRESS;
	parser.query("INSERT INTO banks VALUES (" + BANK_NAME + ", " + ROUTING_NUMBER + ", " + ADDRESS + ");");
	parser.query("WRITE banks;");
}

void Application::remove_bank(){
	string BANK_NAME;
	string ROUTING_NUMBER;
	string ADDRESS;
	
	cout<<"Enter the name of the bank you would like to remove: ";
	cin>>BANK_NAME;
	
	cout<<endl<<"Enter the routing number: ";
	cin>>ROUTING_NUMBER;
	cout<<endl;
	
	parser.execute("DELETE FROM bank WHERE bank_name=\"" + BANK_NAME + "\" AND routing_number=\"" + ROUTING_NUMBER + "\";");
	parser.execute("WRITE bank;");
}

void Application::update_bank(){
	
}

void Application::add_account(){
	string ACCOUNT_NUMBER;
	string TYPE;
	string BALANCE;
	cout<<"Enter new account number: ";
	cin>>ACCOUNT_NUMBER;
	cout<<endl<<"Enter the account type(savings, checking): ";
	cin>>TYPE;
	cout<<endl<<"Enter the accounts starting balance: ";
	cin>>BALANCE;
	cout<<endl;
	parser.execute("INSERT INTO account VALUES (" + ACCOUNT_NUMBER + ", " + TYPE + ", " + BALANCE + ")");
	parser.execute("WRITE account");
}

void Application::remove_account(){
	string ACCOUNT_NUMBER;
	string TYPE;
	cout<<"Enter new account number: ";
	cin>>ACCOUNT_NUMBER;
	cout<<endl<<"Enter the account type(savings, checking): ";
	cin>>TYPE;
	cout<<endl;
	parser.execute("DELETE FROM account WHERE account_number=" + ACCOUNT_NUMBER + " AND account_type=\"" + TYPE "\";");
	parser.execute("WRITE account;");
}

void Applcation::add_client(){
	string NAME;
	string ADDRESS;
	string BIRTH_DATE;
	string PHONE_NUMBER;
	cout<<"Enter new client's first and last name: ";
	cin>>NAME;
	cout<<endl<<"Enter the client's address: ";
	cin>>ADDRESS;
	cout<<endl<<"Enter the client's birth date: ";
	cin>>BIRTH_DATE;
	cout<<endl<<"Enter the client's phone number (without dashes and spaces): ";
	cin>>PHONE_NUMBER;
	cout<<endl;
	
	parser.execute("INSERT INTO client VALUES (\"" + NAME + "\", \"" + ADDRESS + "\", \"" + BIRTH_DATE + "\", " + PHONE_NUMBER + ");");
	parser.execute("WRITE client;");
}

void remove_client(){
	string NAME;
	string BIRTH_DATE;
	cout<<"Enter the client's name you would like to remove: "
	cin<<NAME;
	cout<<endl<<"Enter the client's birth date that you would like to remove: ";
	cin>>BIRTH_DATE;
	
	parser.execute("DELETE FROM client WHERE name=\"" + NAME + "\" AND birth_date=\"" + BIRTH_DATE + "\";");
	parser.execute("WRITE client;");
}*/

