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

		vector<string> bank_attributes {"name", "routing_number", "address"};
		vector<string> account_attributes {"number", "type", "balance"};
		vector<string> client_attributes {"full_name", "home_address", "birthdate", "phone_number"};
		vector<string> bank_enrollment {"client_name", "bank_name"};
		vector<string> account_authorization{"client_name", "account_number"};
		vector<string> account_host{"bank_name", "account_number"};

		vector<vector<string> > table_keys {
			{"name", "routing_number"},
			{"number", "type"},
			{"full_name", "birthdate"},
			{"client_name", "bank_name"},
			{"client_name", "account_number"},
			{"bank_name", "account_number"}};	

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
		void table_menu(int table_index);
		
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
		void remove(string table, vector<string> attributes,
			vector<string> attribute_types, vector<string> keys);
		void update(string table, vector<string> attributes,
			vector<string> attribute_types, vector<string> keys);

		void enroll_all_clients();

	public:
		Application();
		~Application();

		void main_menu();
};

#endif
