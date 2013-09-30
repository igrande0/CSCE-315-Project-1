#include "Database.h"
#include <fstream>

#include <cctype>
#include <iostream>
#include <iomanip>
#include <stdexcept>

/*------------------------------------------------------------------------------------*/
/* QUERY FUNCTIONS */
/*------------------------------------------------------------------------------------*/

void Database::select(string view_name, string in_table_name, int row_index){
	bool VIEW_CHECK = true;

	// check to see if view_name already exists
	for(unsigned int i=0; i<VIEW_LIST.size(); i++)
		if(VIEW_LIST[i][0][0] == view_name){
			VIEW_CHECK = false;
		}
	bool IN_VIEW = false;
	bool IN_RELATION = false;
	for(unsigned int i=0; i<VIEW_LIST.size(); i++)
		if(VIEW_LIST[i][0][0] == in_table_name){
			IN_VIEW = true;
		}
	for(unsigned int i=0; i<RELATION_LIST.size(); i++)
		if(RELATION_LIST[i][0][0] == in_table_name){
			IN_RELATION = true;
		}
	if(!IN_VIEW)
		if(!IN_RELATION)
			throw runtime_error("select: no such table");
	
	vector<vector<string> > TEMP_VIEW_TABLE;
	if(IN_RELATION){
		if(VIEW_CHECK){
			int RELATION_INDEX = get_relation_index(in_table_name);
			vector<string> temp_vec;
			temp_vec.push_back(view_name);
			for(unsigned int i=1; i<RELATION_LIST[RELATION_INDEX][0].size(); i++)
				temp_vec.push_back(RELATION_LIST[RELATION_INDEX][0][i]);

			TEMP_VIEW_TABLE.push_back(temp_vec);
			TEMP_VIEW_TABLE.push_back(RELATION_LIST[RELATION_INDEX][1]);
			TEMP_VIEW_TABLE.push_back(RELATION_LIST[RELATION_INDEX][2]);
			TEMP_VIEW_TABLE.push_back(RELATION_LIST[RELATION_INDEX][row_index]);

			VIEW_LIST.push_back(TEMP_VIEW_TABLE);
		}
		else{
			int VIEW_INDEX = get_view_index(view_name);
			int RELATION_INDEX = get_relation_index(in_table_name);
			VIEW_LIST[VIEW_INDEX].push_back(RELATION_LIST[RELATION_INDEX][row_index]);
		}
	}
	else if(IN_VIEW){
		if(VIEW_CHECK){
			int VIEW_INDEX = get_view_index(in_table_name);
			vector<string> temp_vec;
			temp_vec.push_back(view_name);
			for(unsigned int i=1; i<VIEW_LIST[VIEW_INDEX][0].size(); i++)
				temp_vec.push_back(VIEW_LIST[VIEW_INDEX][0][i]);

			TEMP_VIEW_TABLE.push_back(temp_vec);
			TEMP_VIEW_TABLE.push_back(VIEW_LIST[VIEW_INDEX][1]);
			TEMP_VIEW_TABLE.push_back(VIEW_LIST[VIEW_INDEX][2]);
			TEMP_VIEW_TABLE.push_back(VIEW_LIST[VIEW_INDEX][row_index]);

			VIEW_LIST.push_back(TEMP_VIEW_TABLE);
		}
		else{
			int VIEW_INDEX = get_view_index(view_name);
			int V_INDEX = get_view_index(in_table_name);
			VIEW_LIST[VIEW_INDEX].push_back(VIEW_LIST[V_INDEX][row_index]);
		}
	}
	else
		throw runtime_error("Select: no such table name exists");
}

void Database::project(string view_name, string in_table_name, vector<string> attributes){
	if(view_name == in_table_name)
		throw runtime_error("Project: both name equal");
	int INDEX = get_relation_index(in_table_name);
	vector<vector<string> > TEMP_VIEW_TABLE;
	vector<int> columns;

	if(INDEX == -1){
		INDEX = get_view_index(in_table_name);
		if(INDEX == -1)
			throw runtime_error("Project: no such view exists");
		else{
			bool ATTRIBUTE_CHECK = false;
			for(unsigned int i = 0; i<attributes.size(); i++){
				for(unsigned int j=0; j<VIEW_LIST[INDEX][1].size(); j++)
					if(attributes[i] == VIEW_LIST[INDEX][1][j]){
						columns.push_back(i);
						ATTRIBUTE_CHECK = true;
					}
				if(ATTRIBUTE_CHECK == false) 
					throw runtime_error("Project: no such attribute exists (" + attributes[i] + ")");
			}
			vector<string> t_vec;
			t_vec.push_back(view_name);
			TEMP_VIEW_TABLE.push_back(t_vec);
	 
			for(unsigned int i=1; i< VIEW_LIST[INDEX].size(); i++){
				vector<string> temp_vec;
				for(unsigned int j=0; j<columns.size(); j++)
					temp_vec.push_back(VIEW_LIST[INDEX][i][columns[j]]);
				TEMP_VIEW_TABLE.push_back(temp_vec);
			}
			VIEW_LIST.push_back(TEMP_VIEW_TABLE);
		}
	}
	else{
		bool ATTRIBUTE_CHECK = false;
		for(unsigned int i = 0; i<attributes.size(); i++){
			for(unsigned int j=0; j<RELATION_LIST[INDEX][1].size(); j++)
				if(attributes[i] == RELATION_LIST[INDEX][1][j]){
					columns.push_back(i);
					ATTRIBUTE_CHECK = true;
				}
			if(ATTRIBUTE_CHECK == false)
				throw runtime_error("Project: no such attribute exists (" + attributes[i] + ")");
		}
		vector<string> t_vec;
		t_vec.push_back(view_name);
		TEMP_VIEW_TABLE.push_back(t_vec);
		 
		for(unsigned int i=1; i< RELATION_LIST[INDEX].size(); i++){
			vector<string> temp_vec;
			for(unsigned int j=0; j<columns.size(); j++)
				temp_vec.push_back(RELATION_LIST[INDEX][i][columns[j]]);
			TEMP_VIEW_TABLE.push_back(temp_vec);
		}
		VIEW_LIST.push_back(TEMP_VIEW_TABLE);
	}
}

void Database::rename(string new_view, string existing_table, vector<string> attributes){
	vector<string> key_attribute_indices;
	const vector<vector<string> >& old_table = get_table(existing_table);
		
	// ERROR - incorrect number of attributes 
	if(old_table[1].size() != attributes.size())
		throw runtime_error("rename: incorrect number of attributes");
		
	// create new view table with 2 rows (title and attribute rows)
	VIEW_LIST.push_back(vector<vector<string> >(2));
	vector<vector<string> >& view = VIEW_LIST[VIEW_LIST.size() - 1];

	// set title
	view[0].push_back(new_view);
	
	// set attributes
	view[1] = attributes;

	// copy data
	for(unsigned int i = 2; i < old_table.size(); ++i)
		view.push_back(old_table[i]);

	// set new key names to reflect new attributes
	for(unsigned int i = 1; i < old_table[0].size(); ++i) {
		for(unsigned int j = 0; j < old_table[1].size(); ++j)
			if(old_table[0][i] == old_table[1][j])
				view[0].push_back(attributes[j]);
	}
}

void Database::set_union(string view_name, string table1_name, string table2_name){
	const vector<vector<string> >& table1 = get_table(table1_name);
	const vector<vector<string> >& table2 = get_table(table2_name);

	// ERROR - non-matching attributes
	if(table1[1] != table1[1] || table1[2] != table2[2])
		throw runtime_error("set union: non-matching attributes");

	// create new view table
	VIEW_LIST.push_back(vector<vector<string> >());
	vector<vector<string> >& view = VIEW_LIST[VIEW_LIST.size() - 1];

	// set title and copy keys
	view.push_back(vector<string> ());
	view[0].push_back(view_name);
	view[0].insert(view[0].end(), table1[0].begin() + 1, table1[0].end());

	// copy relation 1 to view table
	for(unsigned int i = 1; i < table1.size(); ++i)
		view.push_back(table1[i]);
	
	// copy tuples from relation 2 to view table
	for(unsigned int i = 3; i < table2.size(); ++ i)
		view.push_back(table2[i]);

	// remove duplicates
	for(unsigned int i = 3; i < view.size(); ++i)
		for(unsigned int j = i + 1; j < view.size(); ++j)
			if(view[i] == view[j])
				view.erase(view.begin() + j);
}

void Database::set_difference(string view_name, string table1_name, string table2_name){
	vector<vector<string> > temp;
	const vector<vector<string> >& table1 = get_table(table1_name);
	const vector<vector<string> >& table2 = get_table(table2_name);
	bool equal = false;
	vector<string> temp_vec;

	// ERROR - non-matching attributes
	if(table1[1] != table1[1] || table1[2] != table2[2])
		throw runtime_error("set difference: non-matching attributes");

	temp_vec.push_back(view_name);
	for(unsigned int i=1; i<table1[0].size(); i++)
		temp_vec.push_back(table1[0][i]);
	temp.push_back(temp_vec);
	temp.push_back(table1[1]);
	temp.push_back(table1[2]);

	for(unsigned int i=3; i<table1.size(); i++){
		for(unsigned int j=3; j<table2.size(); j++)
			if(table1[i] == table2[j])
				equal = true;
		if(equal == false){
			temp.push_back(table1[i]);
		}
		equal = false;
	}
	VIEW_LIST.push_back(temp);

}

void Database::cross_product(string view_name, string table1_name, string table2_name){
	vector<string> attributes;
	vector<string> types;
	const vector<vector<string> >& table1 = get_table(table1_name);
	const vector<vector<string> >& table2 = get_table(table2_name);

	// create new view table
	VIEW_LIST.push_back(vector<vector<string> >());
	vector<vector<string> >& view = VIEW_LIST[VIEW_LIST.size() - 1];

	// set title
	view.push_back(vector<string>());
	view[0].push_back(view_name);

	// merge attributes and attribute types
	attributes = table1[1];
	attributes.insert(attributes.end(), table2[1].begin(), table2[1].end());
	types = table1[2];
	types.insert(types.end(), table2[2].begin(), table2[2].end());

	view.push_back(attributes);
	view.push_back(types);

	// perform cross product
	for(unsigned int i=3; i<table1.size(); i++)
		for(unsigned int j=3; j<table2.size(); j++) {
			vector<string> tuple;
			tuple = table1[i];
			tuple.insert(tuple.end(), table2[j].begin(), table2[j].end());
			view.push_back(tuple);
		}
}

/*------------------------------------------------------------------------------------*/
/* COMMAND FUNCTIONS */
/*------------------------------------------------------------------------------------*/

void Database::close(string table_name){
	int index;
	if((index = get_relation_index(table_name)) != -1)
		RELATION_LIST.erase(RELATION_LIST.begin() + index);
	else if((index = get_view_index(table_name)) != -1)
		VIEW_LIST.erase(VIEW_LIST.begin() + index);
	else
		throw runtime_error("close: no such table");
}

void Database::exit(){
	RELATION_LIST.clear();						
	VIEW_LIST.clear();
}

void Database::write(string table_name){
	int index;

	if((index = get_relation_index(table_name)) != -1)
		write_table(table_name, RELATION_LIST[index]);
	else if((index = get_view_index(table_name)) != -1)
		write_table(table_name, VIEW_LIST[index]);
	else
		throw runtime_error("write: no such table");
}

void Database::write_table(string table_name, const vector<vector<string> >& TABLE){
	ofstream OUTPUT_FILE;
	OUTPUT_FILE.open(table_name + ".db");
	OUTPUT_FILE << "CREATE TABLE " << table_name << " (";
	for(unsigned int i=0; i<TABLE[1].size(); i++){
		if(i != (TABLE[1].size() - 1))
			OUTPUT_FILE << TABLE[1][i] << " " << TABLE[2][i] << ", ";
		else
			OUTPUT_FILE << TABLE[1][i] << " " << TABLE[2][i] << ") ";
	}
	OUTPUT_FILE << "PRIMARY KEY (";
	for(unsigned int i=1; i<TABLE[0].size(); i++){
		if(i != (TABLE[0].size() - 1))
			OUTPUT_FILE << TABLE[0][i] << ", ";
		else
			OUTPUT_FILE << TABLE[0][i] << ");" << endl;
	}
	for(unsigned int i=3; i<TABLE.size(); i++){
		OUTPUT_FILE << "INSERT INTO " << table_name << " VALUES FROM (";
		for(unsigned int j=0; j<TABLE[i].size(); j++){
			if(j != (TABLE[i].size() - 1)){
				if(TABLE[2][j] == "INTEGER")
					OUTPUT_FILE << TABLE[i][j] << ", ";
				else
					OUTPUT_FILE << "\"" << TABLE[i][j] << "\", ";
			}
			else{
				if(TABLE[2][j] == "INTEGER")
					OUTPUT_FILE << TABLE[i][j] << ");" << endl;
				else
					OUTPUT_FILE << "\"" << TABLE[i][j] << "\");" << endl;
			}
		}
	}
	OUTPUT_FILE.close();
}

void Database::show(string table_name){
	int index;

	if((index = get_relation_index(table_name)) != -1) 
		print_table(RELATION_LIST[index]);
	else if((index = get_view_index(table_name)) != -1)
		print_table(VIEW_LIST[index]);
	else
		throw runtime_error("show: no such table");
}

void Database::print_table(const vector<vector<string > >& table) {
	cout << table[0][0] << "\t\t keys(";
	for(unsigned int i = 1; i < table[0].size(); i++) {
		cout << table[0][i];
		if(i != (table[0].size()-1))
			cout << ",";
	}
	cout << ")" << endl;
	
	for(unsigned int i = 1; i < table.size(); i++) {
		for(unsigned int j = 0; j < table[i].size(); j++){
			cout << left << setw(25);
			cout << table[i][j];
		}
		cout << endl;
	}
}

void Database::create(string table_name, vector<string> attributes,
					  vector<string> attribute_types, vector<string> keys){
	// check for keys referring to non-existent attributes
	for(unsigned int i = 0; i < keys.size(); ++i) {
		unsigned int matching_count = 0;
		for(unsigned int j = 0; j < attributes.size(); ++j)
			if(keys[i] == attributes[j])
				++matching_count;
		if(matching_count == 0) {
			string error = "create: key (";
			error += keys[i];
			error += ") does not match any attributes";
			throw runtime_error(error);
		}
	}
	// check for duplicate attributes
	for(unsigned int i = 0; i < attributes.size(); ++i) 
		for(unsigned int j = i + 1; j < attributes.size(); ++j) 
			if(attributes[i] == attributes[j]) {
				string error = "create: duplicate attribute (";
				error += attributes[i];
				throw runtime_error(error);
			}

	// check for existing table
	if(get_relation_index(table_name) > -1) {
		string error = "create: table with name (";
		error += table_name;
		error += ") already exists";
		throw runtime_error(error);
	}

	// push back empty table to relation list and get reference to table
	RELATION_LIST.push_back(vector<vector<string> >());
	vector<vector<string> >& relation_table = RELATION_LIST[RELATION_LIST.size()-1];

	// push back empty row and intialize with title and keys
	relation_table.push_back(vector<string>());
	relation_table[0].push_back(table_name);
	relation_table[0].insert(relation_table[0].end(), keys.begin(), keys.end());

	// add attribute names and attribute types
	relation_table.push_back(attributes);
	relation_table.push_back(attribute_types);
}


void Database::update(string relation_name, vector<string>attribute, vector<string> data, int row_index){
	int relation_index = -1;
	vector<int> attribute_indices;

	relation_index = get_relation_index(relation_name);

	if(relation_index == -1)
		throw runtime_error("update: no such relation");

	// find attribute indices
	for(unsigned int i = 0; i < attribute.size(); ++i) {
		int attribute_index = -1;
		attribute_index = get_attribute_index(RELATION, relation_index, attribute[i]);

		if(relation_index == -1)
			throw runtime_error("update: no such attribute");

		attribute_indices.push_back(attribute_index);
	}

	// update attributes
	for(unsigned int i = 0; i < attribute_indices.size(); ++i)
		RELATION_LIST[relation_index][row_index][attribute_indices[i]] = data[i];
}

void Database::insert_tuple(string relation_name, vector<string> tuple){
	int table_index;
	vector<string> keys;
	vector<int> key_indices;

	if((table_index = get_relation_index(relation_name)) == -1)
		throw runtime_error("insert_tuple: relation does not exist");

	vector<vector<string> >& table =  RELATION_LIST[table_index];

	if(tuple.size() != table[2].size())
		throw runtime_error("insert_tuple: incorrect number of attributes");

	// get keys
	for(unsigned int i = 1; i < table[0].size(); ++i)
		keys.push_back(table[0][i]);

	// get indices of attribute keys
	for(unsigned int i = 0; i < keys.size(); ++i)
		key_indices.push_back(get_attribute_index(RELATION, table_index, keys[i]));

	// check for duplicates
	for(unsigned int i = 3; i < table.size(); ++i) {
		unsigned int dupe_count = 0;
		for(unsigned int j = 0; j < key_indices.size(); ++j)
			if(table[i][key_indices[j]] == tuple[key_indices[j]])
				++dupe_count;
		if(dupe_count == key_indices.size())
			throw runtime_error("insert_tuple: duplicate tuple");
	}

	table.push_back(tuple);
}

void Database::insert_view(string relation_name, string view_name){
	int view_index = -1;
	int relation_index = -1;
	vector<string> keys;
	vector<int> key_indices;

	if((view_index = get_view_index(view_name)) == -1)
		throw runtime_error("Insert View: view does not exist"); 
	if((relation_index = get_relation_index(relation_name)) == -1)
		throw runtime_error("Insert View: relation table does not exist");

	vector<vector<string> >& view_table = VIEW_LIST[view_index];
	vector<vector<string> >& relation_table = RELATION_LIST[relation_index];

	// check for compatibility
	if(view_table[0].size() != relation_table[0].size()
	|| view_table[1].size() != relation_table[1].size()
	|| view_table[2].size() != relation_table[2].size())
		throw runtime_error("insert view: incompatible tables");
	for(unsigned int i = 0; i < view_table[2].size(); ++i)
		if(view_table[2][i] != relation_table[2][i])
			throw runtime_error("insert_view: incompatible tables: different attribute types");

	// get keys
	for(unsigned int i = 1; i < relation_table[0].size(); ++i)
		keys.push_back(relation_table[0][i]);

	// get indices of attribute keys
	for(unsigned int i = 0; i < keys.size(); ++i)
		key_indices.push_back(get_attribute_index(RELATION, relation_index, keys[i]));

	// copy non-duplicate values from view table to relation table
	for(unsigned int i = 3; i < view_table.size(); ++i) {
		bool duplicate = false;
		for(unsigned int j = 3; j < relation_table.size(); ++j) {
			unsigned int dupe_count = 0;
			for(unsigned int k = 0; k < key_indices.size(); ++k)
				if(view_table[i][key_indices[k]] == relation_table[j][key_indices[k]])
					dupe_count++;
			if(dupe_count == key_indices.size())
				duplicate = true;
		}
		if(!duplicate)
			relation_table.push_back(view_table[i]);
	}
}

void Database::remove(string relation_name, int row_index){
	int relation_index;
	if((relation_index = get_relation_index(relation_name)) == -1)
		throw runtime_error("remove: no such relation");

	vector<vector<string> >& relation_table = RELATION_LIST[relation_index];

	if(unsigned(row_index) >= relation_table.size())
		throw runtime_error("remove: no such row index (" + to_string(row_index) + ")");

	relation_table.erase(relation_table.begin() + row_index);
}

/*------------------------------------------------------------------------------------*/
/* UTILITY FUNCTIONS */
/*------------------------------------------------------------------------------------*/

int Database::get_relation_index(string table_name){
	int INDEX = -1;

	for(unsigned int i=0; i<RELATION_LIST.size(); i++)
		if(RELATION_LIST[i][0][0] == table_name)
			INDEX = i;
	return INDEX;
}

int Database::get_view_index(string table_name){
	int INDEX = -1;

	for(unsigned int i=0; i<VIEW_LIST.size(); i++)
		if(VIEW_LIST[i][0][0] == table_name)
			INDEX = i;
	return INDEX;
}

int Database::get_attribute_index(TableType type, int table_index, string attribute_name){
	int COLUMN_INDEX = -1;
	if(type == RELATION){
		for(unsigned int i = 0; i<RELATION_LIST[table_index][1].size(); i++)
			if(attribute_name == RELATION_LIST[table_index][1][i])
				COLUMN_INDEX = i;
	}
	else{
		for(unsigned int i = 0; i<VIEW_LIST[table_index][1].size(); i++)
			if(attribute_name == VIEW_LIST[table_index][1][i])
				COLUMN_INDEX = i;
	}
	return COLUMN_INDEX;
}

void Database::update_view_name(string new_name, string old_name){
	int VIEW_INDEX = get_view_index(old_name);
	if(VIEW_INDEX == -1)
		throw runtime_error("Change View Name: No such table exists");
	else
		VIEW_LIST[VIEW_INDEX][0][0] = new_name;
}

const vector<vector<string> >& Database::get_table(string table_name){
	int index;

	if((index = get_relation_index(table_name)) != -1)
		return RELATION_LIST[index];
	else if((index = get_view_index(table_name)) != -1)
		return VIEW_LIST[index];
	else
		throw runtime_error("get_table: no such table");
}
