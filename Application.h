#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "Parser.h"

#include <vector>
#include <iostream>

using namespace std;

class Application{
	private:
		Parser parser;

		vector<string> tables {"bank", "account", "person", "enrollment", "authorization", "host"};

		vector<string> bank_attributes {"bank name", "bank routing number", "bank address"};
		vector<string> account_attributes {"account number", "account type", "account balance"};
		vector<string> person_attributes {"name", "address", "birthdate", "phone number"};
		vector<string> bank_enrollment {"person's name", "bank name"};
		vector<string> account_authorization{"person's name", "account number"};
		vector<string> account_host{"bank name", "account number"};

		vector<vector<string> > attributes {bank_attributes, account_attributes, person_attributes,
			bank_enrollment, account_authorization, account_host};

		void display_main_menu();
		void display_table_menu(string table);
		void table_menu(string table);

		void add(string table, vector<string> attributes);
		void remove(string table, vector<string> attributes);
		void update(string table, vector<string> attributes);


	public:
		Application();
		~Application();

		void main_menu();
};

#endif
