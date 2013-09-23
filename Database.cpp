#include "Database.h"
#include <fstream>

#include <cctype>
#include <iostream>
#include <iomanip>
#include <stdexcept>

#define RELATION 0
#define VIEW 1

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
		if(VIEW_LIST[i][0][0] == in_table_name){
			IN_RELATION = true;
		}
	if(!IN_VIEW)
		if(!IN_RELATION)
			throw runtime_error("select: no such table");
	
	vector<vector<string>> TEMP_VIEW_TABLE;
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
	vector<vector<string>> TEMP_VIEW_TABLE;
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
					throw runtime_error("Project: no such attribute exists " + attributes[i]);
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
				throw runtime_error("Project: no such attribute exists " + attributes[i]);
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

/* rename all attributes in an existing table (can be a relation or view)
 * add resulting table into the view list
 */
void Database::rename(string new_view, string existing_table, vector<string> attributes){
	int existing_table_type;
	int existing_table_index;
	int new_view_index;

	// look for table in relation list
	if((existing_table_index = get_relation_index(existing_table)) != -1)
		existing_table_type = RELATION;
	else if((existing_table_index = get_view_index(existing_table)) != -1)
		existing_table_type = VIEW;
	else
		throw runtime_error("Rename: table does not exist");
		
	// ERROR - incorrect number of attributes 
	if(existing_table_type == RELATION && RELATION_LIST[existing_table_index][1].size() != attributes.size())
		throw runtime_error("Rename: incorrect number of relation attributes");
	if(existing_table_type == VIEW && VIEW_LIST[existing_table_index][1].size() != attributes.size())
		throw runtime_error("Rename: incorrect number of view attributes");
		
	// create new view table with 2 rows (title and attribute rows)
	VIEW_LIST.push_back(vector<vector<string> >(2));
	new_view_index = VIEW_LIST.size() - 1;

	// set title
	VIEW_LIST[new_view_index][0].push_back(new_view);
	
	// set attributes
	VIEW_LIST[new_view_index][1] = attributes;

	// copy remaining data
	if(existing_table_type == RELATION)
		for(unsigned int i = 2; i < RELATION_LIST[existing_table_index].size(); ++i)
			VIEW_LIST[new_view_index].push_back(RELATION_LIST[existing_table_index][i]);
	else
		for(unsigned int i = 2; i < VIEW_LIST[existing_table_index].size(); ++i)
			VIEW_LIST[new_view_index].push_back(VIEW_LIST[existing_table_index][i]);
}

void Database::set_union(string view_name, string relation1_name, string relation2_name){
	int r1_index;
	int r2_index;

	// ERROR - no such table
	if((r1_index = get_relation_index(relation1_name)) == -1)
		;
	// ERROR - no such table
	if((r2_index = get_relation_index(relation2_name)) == -1)
		;

	// ERROR - non-matching attributes
	if(RELATION_LIST[r1_index][1] != RELATION_LIST[r2_index][1] 
	|| RELATION_LIST[r1_index][2] != RELATION_LIST[r2_index][2])
		;

	// create new view table
	VIEW_LIST.push_back(vector<vector<string> >());
	vector<vector<string> >& view = VIEW_LIST[VIEW_LIST.size() - 1];

	// set title and copy keys
	view.push_back(vector<string> ());
	view[0].push_back(view_name);
	view[0].insert(view[0].end(), RELATION_LIST[r1_index][0].begin() + 1, RELATION_LIST[r1_index][0].end());

	// copy relation 1 to view table
	for(unsigned int i = 1; i < RELATION_LIST[r1_index].size(); ++i)
		view.push_back(RELATION_LIST[r1_index][i]);
	
	// copy tuples from relation 2 to view table
	for(unsigned int i = 3; i < RELATION_LIST[r2_index].size(); ++ i)
		view.push_back(RELATION_LIST[r2_index][i]);

	// remove duplicates
	for(unsigned int i = 3; i < view.size(); ++i)
		for(unsigned int j = i + 1; j < view.size(); ++j)
			if(view[i] == view[j])
				view.erase(view.begin() + j);
}

void Database::set_difference(string view_name, string relation1_name, string relation2_name){
	vector<vector<string>> temp;
	int relation1 = get_relation_index(relation1_name);
	int relation2 = get_relation_index(relation2_name);
	bool equal = false;
	vector<string> temp_vec;
	temp_vec.push_back(view_name);
	for(unsigned int i=1; i<RELATION_LIST[relation1][0].size(); i++)
		temp_vec.push_back(RELATION_LIST[relation1][0][i]);
	temp.push_back(temp_vec);
	temp.push_back(RELATION_LIST[relation1][1]);
	temp.push_back(RELATION_LIST[relation1][2]);

	for(unsigned int i=3; i<RELATION_LIST[relation1].size(); i++){
		for(unsigned int j=3; j<RELATION_LIST[relation2].size(); j++)
			if(RELATION_LIST[relation1][i] == RELATION_LIST[relation2][j])
				equal = true;
		if(equal == false){
			temp.push_back(RELATION_LIST[relation1][i]);
		}
		equal = false;
	}
	VIEW_LIST.push_back(temp);

}

void Database::cross_product(string view_name, string relation1_name, string relation2_name){
	int r1_index;
	int r2_index;
	vector<string> attributes;
	vector<string> types;

	// ERROR - no such table
	if((r1_index = get_relation_index(relation1_name)) == -1)
		;
	// ERROR - no such table
	if((r2_index = get_relation_index(relation2_name)) == -1)
		;

	// create new view table
	VIEW_LIST.push_back(vector<vector<string> >());
	vector<vector<string> >& view = VIEW_LIST[VIEW_LIST.size() - 1];

	// set title
	view.push_back(vector<string>());
	view[0].push_back(view_name);

	// merge attributes and attribute types
	attributes = RELATION_LIST[r1_index][1];
	attributes.insert(attributes.end(), RELATION_LIST[r2_index][1].begin(), RELATION_LIST[r2_index][1].end());
	types = RELATION_LIST[r1_index][2];
	types.insert(types.end(), RELATION_LIST[r2_index][2].begin(), RELATION_LIST[r2_index][2].end());

	view.push_back(attributes);
	view.push_back(types);

	// perform cross product
	for(unsigned int i=3; i<RELATION_LIST[r1_index].size(); i++)
		for(unsigned int j=3; j<RELATION_LIST[r2_index].size(); j++) {
			vector<string> tuple;
			tuple = RELATION_LIST[r1_index][i];
			tuple.insert(tuple.end(), RELATION_LIST[r2_index][j].begin(), RELATION_LIST[r2_index][j].end());
			view.push_back(tuple);
		}
}

/*------------------------------------------------------------------------------------*/
/* COMMAND FUNCTIONS */
/*------------------------------------------------------------------------------------*/

void Database::close(string table_name){			//clears all vectors erasing all data that has not been written to a file
	RELATION_LIST.clear();						
	VIEW_LIST.clear();
}

void Database::write(string table_name){
	int INDEX = get_relation_index(table_name);
	if(INDEX == -1){
		INDEX = get_view_index(table_name);
		if(INDEX == -1)
			throw runtime_error("Write: no such table exists");
		else
			write_table(table_name, VIEW_LIST[INDEX]);
	}
	else
		write_table(table_name, RELATION_LIST[INDEX]);
}

void Database::write_table(string table_name, const vector<vector<string>>& TABLE){
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
	int INDEX = get_relation_index(table_name);
	int TABLE_TYPE = RELATION;
	if(INDEX == -1){
		INDEX = get_view_index(table_name);
		TABLE_TYPE = VIEW;
	}
	if(TABLE_TYPE == RELATION){
		print_table(RELATION_LIST[INDEX]);
	}
	else{
		print_table(VIEW_LIST[INDEX]);
	}
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
			cout << left << setw(15);
			cout << table[i][j];
		}
		cout << endl;
	}
}

void Database::create(string table_name, vector<string> attributes, vector<string> attribute_types, vector<string> keys){
	vector<vector<string>> TEMP_RELATION_TABLE;
	vector<string> temp_vec1, temp_vec2, temp_vec3;

	temp_vec1.push_back(table_name);

	for(unsigned int i=0; i<keys.size(); i++)
		temp_vec1.push_back(keys[i]);
	for(unsigned int i=0; i<attributes.size(); i++)
		temp_vec2.push_back(attributes[i]);
	for(unsigned int i=0; i<attribute_types.size(); i++)
		temp_vec3.push_back(attribute_types[i]);

	TEMP_RELATION_TABLE.push_back(temp_vec1);
	TEMP_RELATION_TABLE.push_back(temp_vec2);
	TEMP_RELATION_TABLE.push_back(temp_vec3);

	RELATION_LIST.push_back(TEMP_RELATION_TABLE);
}


void Database::update(string relation_name, vector<string>attribute, vector<string> data, int row_index){
	int relation_index = -1;
	vector<int> attribute_indices;

	relation_index = get_relation_index(relation_name);

	// ERROR - no such relation
	if(relation_index == -1);

	// find attribute indices
	for(unsigned int i = 0; i < attribute.size(); ++i) {
		int attribute_index = -1;
		attribute_index = get_attribute_index(RELATION, relation_index, attribute[i]);

		// ERROR - no such attribute
		if(relation_index == -1);

		attribute_indices.push_back(attribute_index);
	}

	// update attributes
	for(unsigned int i = 0; i < attribute_indices.size(); ++i)
		RELATION_LIST[relation_index][row_index][attribute_indices[i]] = data[i];
}

void Database::insert_tuple(string relation_name, vector<string> tuple){
	int table_index;

	if((table_index = get_relation_index(relation_name)) == -1)
		; // RETURN ERROR

	RELATION_LIST[table_index].push_back(tuple);
}

void Database::insert_view(string relation_name, string view_name){
	int view_index = -1;
	int relation_index = -1;

	if((view_index = get_view_index(view_name)) == -1)
		; // RETURN ERROR

	if((relation_index = get_relation_index(relation_name)) == -1)
		; // RETURN ERROR

	// copy values from view table to relation table
	for(unsigned int i = 3; i < VIEW_LIST[view_index].size(); ++i)
		RELATION_LIST[relation_index].push_back(VIEW_LIST[view_index][i]);
}

void Database::remove(string relation_name, int row_index){
	int relation_index = -1;
	relation_index = get_relation_index(relation_name);

	// ERROR - no such relation
	if(relation_index == -1);

	RELATION_LIST[relation_index].erase(RELATION_LIST[relation_index].begin() + row_index);
}

/*------------------------------------------------------------------------------------*/
/* UTILITY FUNCTIONS */
/*------------------------------------------------------------------------------------*/

bool Database::compare(string attribute_type, string left_arg, string right_arg, string comparison_op) {
	bool result;
	switch(attribute_type[0]) {
		// INT or INTEGER
		case 'I':
			result = compare<int>(stoi(left_arg), stoi(right_arg), comparison_op);

		// FLOAT  or REAL
		case 'F': case 'R':
			result = compare<float>(stof(left_arg), stof(right_arg), comparison_op); 

		// CHAR or VARCHAR
		case 'C': case 'V':
			result = compare<string>(left_arg, right_arg, comparison_op);

		// DATE or TIME 
		case 'D': case 'T':
			result = compare<int>(extract_digits(left_arg), extract_digits(right_arg), comparison_op);
	}
	return result;
}

template <class T> bool Database::compare(T left_arg, T right_arg, string comparison_op) {
	if(comparison_op == "==")
		return left_arg == right_arg;
	else if(comparison_op == "!=")
		return left_arg != right_arg;
	else if(comparison_op == "<")
		return left_arg < right_arg;
	else if(comparison_op == ">")
		return left_arg > right_arg;
	else if(comparison_op == "<=")
		return left_arg <= right_arg;
	else
		return left_arg >= right_arg;
}

int Database::extract_digits(string input) {
	for(unsigned int i = input.size(); i > 0; --i)
		if(!isdigit(input[i-1]))
			input.erase(input.begin() + i);

	return stoi(input);
}

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

int Database::get_attribute_index(int table_type, int table_index, string attribute_name){
	int COLUMN_INDEX = -1;
	if(table_type == RELATION){
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

void Database::erase_view(string view_name){
	int VIEW_INDEX = get_view_index(view_name);
	VIEW_LIST.erase(VIEW_LIST.begin() + VIEW_INDEX);
}

void Database::change_view_name(string new_name, string old_name){
	int VIEW_INDEX = get_view_index(old_name);
	if(VIEW_INDEX == -1)
		throw runtime_error("Change View Name: No such table exists");
	else
		VIEW_LIST[VIEW_INDEX][0][0] = new_name;
}
vector<vector<string>> Database::get_tuples(string table_name){
	int INDEX = get_relation_index(table_name);
	vector<vector<string>> TEMP_TABLE;
	if(INDEX == -1){
		INDEX = get_view_index(table_name);
		if(INDEX == -1)
			throw runtime_error("Get Tuples: no such table exists");
		else
			for(unsigned int i=3; i<VIEW_LIST[INDEX].size(); i++)
				TEMP_TABLE.push_back(VIEW_LIST[INDEX][i]);
	}
	else
		for(unsigned int i=3; i<RELATION_LIST[INDEX].size(); i++)
				TEMP_TABLE.push_back(RELATION_LIST[INDEX][i]);	
	return TEMP_TABLE;
}
