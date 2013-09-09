#include "Database.h"
#include <string>
#include <iostream>

int main() {
	Database db;

	string title = "Table";
	vector<string> keys;
	vector<string> attributes;
	vector<string> attribute_types;
	vector<string> tuple1;
	vector<string> tuple2;
	vector<string> tuple3;
	vector<string> tuple4;

	attributes.push_back("Attribute 1");
	attributes.push_back("Attribute 2");
	attributes.push_back("Attribute 3");
	attributes.push_back("Attribute 4");

	attribute_types.push_back("CHAR(10)");
	attribute_types.push_back("INT");
	attribute_types.push_back("DATE");
	attribute_types.push_back("FLOAT");

	keys.push_back("Attribute 1");
	keys.push_back("Attribute 3");

	tuple1.push_back("Allison");
	tuple1.push_back("1");
	tuple1.push_back("2013-10-10");
	tuple1.push_back("12.38");

	tuple2.push_back("Bill");
	tuple2.push_back("2");
	tuple2.push_back("2010-08-01");
	tuple2.push_back("48.2");

	tuple3.push_back("Charlotte");
	tuple3.push_back("3");
	tuple3.push_back("2033-08-08");
	tuple3.push_back("65.54");

	tuple4.push_back("Doug");
	tuple4.push_back("4");
	tuple4.push_back("2004-06-13");
	tuple4.push_back("89.38");

	cout << "TEST CREATE(), INSERT_TUPLE(), SHOW(): create and show a table\n";
	db.create(title, attributes, attribute_types, keys);
	db.insert_tuple(title, tuple1);
	db.insert_tuple(title, tuple2);
	db.insert_tuple(title, tuple3);
	db.insert_tuple(title, tuple4);
	db.show(title);
	cout << "\n\n";
	
	vector<string> tuple5;
	vector<string> tuple6;
	vector<string> tuple7;
	vector<string> tuple8;
	
	tuple5.push_back("Jake");
	tuple5.push_back("57");
	tuple5.push_back("1991-10-18");
	tuple5.push_back("54.32");

	tuple6.push_back("Joe");
	tuple6.push_back("10");
	tuple6.push_back("2015-01-05");
	tuple6.push_back("85.3");

	tuple7.push_back("Charlotte");
	tuple7.push_back("3");
	tuple7.push_back("2033-08-08");
	tuple7.push_back("65.54");

	tuple8.push_back("Doug");
	tuple8.push_back("4");
	tuple8.push_back("2004-06-13");
	tuple8.push_back("89.38");
	db.create("table2", attributes, attribute_types, keys);
	db.insert_tuple("table2", tuple5);
	db.insert_tuple("table2", tuple6);
	db.insert_tuple("table2", tuple7);
	db.insert_tuple("table2", tuple8);
	cout<<"\n\n TABLE 2:\n";
	db.show("table2");
	
	cout << "TEST REMOVE(): remove row 4\n";
	db.remove(title, 5);
	db.show(title);
	cout << "\n\n";
	
	cout << "TEST UPDATE(): change Attribute 2 on row 3 to '55'\n";
	vector<string> attribute;
	attribute.push_back("Attribute 2");
	vector<string> data;
	data.push_back("55");
	db.update(title, attribute, data, 3);
	db.show(title);
	cout << "\n\n";
	
	cout << "TEST SELECT(): select row 3 and row 5\n";
	db.select("view1", title, 3);
	db.select("view1", title, 5);
	db.show("view1");
	cout << "\n\n";
		
	cout << "TEST INSERT_VIEW(): insert view1 into Table\n";
	db.insert_view(title, "view1");
	db.show(title);
	cout << "\n\n";
	
	cout << "TEST PROJECT(): project the Attribute 2 column\n";
	db.project("view2", title, attribute);
	db.show("view2");
	
	cout<< "Test RENAME(): \n";
	attributes[0] = "Attribute 5";
	attributes[1] = "Attribute 6";
	attributes[2] = "Attribute 7";
	attributes[3] = "Attribute 8";
	db.rename("view3", title, attributes);
	db.show("view3");
	cout << "\n\n";
	
	cout << "TEST SET_UNION()\n";
	db.set_union("view4", title, "table2");
	db.show("view4");
	cout << "\n\n";


	cout << "TEST SET_DIFFERENCE()\n";
	db.set_difference("view5", title, "table2");
	db.show("view5");
	cout<<"\n\n";
	
}