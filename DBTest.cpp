#include "Database.h"
#include <string>

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

	db.create(title, attributes, attribute_types, keys);
	db.insert_tuple(title, tuple1);
	db.insert_tuple(title, tuple2);
	db.insert_tuple(title, tuple3);
	db.insert_tuple(title, tuple4);

	db.show(title);
	db.remove(title, 4);
	db.show(title);
	vector<string> temp_at;
	temp_at.push_back("Attribute 2");
	vector<string> temp_new;
	temp_new.push_back("55");
	db.update(title, temp_at, temp_new, 3);
	db.show(title);
	db.select("view1", title, 3);
	db.select("view1", title, 5);
	db.show("view1");              //dumps core? couldnt figure out gbd - JM
	



}