#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "Parser.h"

#include <vector>
#include <iostream>

using namespace std;

class Application{
	private:
		Parser parser;

		vector<string> tables {"bank", "account", "client", "enrollment", "authorization", "host"};

		vector<string> bank_attributes {"name", "number", "address"};
		vector<string> account_attributes {"number", "type", "balance"};
		vector<string> client_attributes {"name", "address", "birthdate", "phone number"};
		vector<string> bank_enrollment {"client name", "bank name"};
		vector<string> account_authorization{"client name", "account number"};
		vector<string> account_host{"bank name", "account number"};

		vector<string> keys {
			"name, routing number",
			"number, type",
			"name, birthdate",
			"client name, bank name",
			"client name, account number",
			"bank name, account number"};	

		vector<vector<string> > attributes {bank_attributes, account_attributes, client_attributes,
			bank_enrollment, account_authorization, account_host};

		vector<vector<string> > attribute_types {
			{"VARCHAR(255)", "INTEGER", "VARCHAR(255)"},
			{"INTEGER", "VARCHAR(255)", "INTEGER"},
			{"VARCHAR(255)", "VARCHAR(255)", "VARCHAR(255)", "INTEGER"},
			{"VARCHAR(255)", "VARCHAR(255)"},
			{"VARCHAR(255)", "INTEGER"},
			{"VARCHAR(255)", "INTEGER"}};

		void display_main_menu();
		void display_table_menu(string table);
		void table_menu(string table);
		
		/*int main_menu();
		int second_menu(int type);
		void add_bank();
		void remove_bank();
		void update_bank();
		void add_account();
		void remove_account();
		void add_client();
		void remove_client();*/

		void add(string table, vector<string> attributes, vector<string> attribute_types);
		void remove(string table, vector<string> attributes, vector<string> attribute_types);
		void update(string table, vector<string> attributes, vector<string> attribute_types);

	public:
		Application();
		~Application();

		void main_menu();
};

#endif
