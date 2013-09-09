#include "Database.h"

#include <cctype>

#define RELATION 0
#define VIEW 1

/*------------------------------------------------------------------------------------*/
/* QUERY FUNCTIONS */
/*------------------------------------------------------------------------------------*/

void Database::select(string view_name, string in_table_name, string left_arg, string right_arg, string comparison){
	bool VIEW_CHECK = true;
	unsigned int VIEW_INDEX = -1;
	for(unsigned int i=0; i<VIEW_LIST.size(); i++)
		if(VIEW_LIST[i][0][0] == view_name){
			VIEW_CHECK = false;
			VIEW_INDEX = i;
		}

	int RELATION_INDEX, RELATION_COLUMN = -1;
	vector<vector<string>> TEMP_VIEW_TABLE;

	for(unsigned int i =0; i<RELATION_LIST.size(); i++)
		if(RELATION_LIST[i][0][0] == in_table_name)
			RELATION_INDEX = i;
	for(unsigned int i=0; i<RELATION_LIST[RELATION_INDEX][1].size(); i++)
		if(RELATION_LIST[RELATION_INDEX][1][i] == left_arg)
			RELATION_COLUMN = i;
	
	
	
	if(VIEW_CHECK){
		vector<string> temp_vec;
		temp_vec.push_back(view_name);
		TEMP_VIEW_TABLE.push_back(temp_vec);					//inserts view table name into temp vector
		//need to initialize view table headings
		//TEMP_VIEW_TABLE.push_back(table name);
		//push_back attributes and types
		for(unsigned int i=0; i<RELATION_LIST[RELATION_INDEX].size(); i++)
			if(compare(RELATION_LIST[RELATION_INDEX][2][RELATION_COLUMN], RELATION_LIST[RELATION_INDEX][i][RELATION_COLUMN], right_arg, comparison))
				TEMP_VIEW_TABLE.push_back(RELATION_LIST[RELATION_INDEX][i]);

		VIEW_LIST.push_back(TEMP_VIEW_TABLE);
	}
	else{
		for(unsigned int i=0; i<RELATION_LIST[RELATION_INDEX].size(); i++)
			if(compare(RELATION_LIST[RELATION_INDEX][2][RELATION_COLUMN], RELATION_LIST[RELATION_INDEX][i][RELATION_COLUMN], right_arg, comparison))
				VIEW_LIST[VIEW_INDEX].push_back(RELATION_LIST[RELATION_INDEX][i]);
	}
	
}

void Database::project(string view_name, string in_table_name, vector<string> attributes){

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
		; // RETURN AN ERROR
		
	// ERROR - incorrect number of attributes 
	if(existing_table_type == RELATION && RELATION_LIST[existing_table_index][1].size() != attributes.size());
	if(existing_table_type == VIEW && VIEW_LIST[existing_table_index][1].size() != attributes.size());
		
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

}

void Database::set_difference(string view_name, string relation1_name, string relation2_name){

}

void Database::cross_product(string view_name, string relation1_name, string relation2_name){

}

/*------------------------------------------------------------------------------------*/
/* COMMAND FUNCTIONS */
/*------------------------------------------------------------------------------------*/

void Database::close(string table_name){
	//dont need for monday Sept. 9th submission
}

void Database::write(string table_name){
	//dont need for monday Sept. 9th submission
}

void Database::show(string table_name){

}

void Database::create(string table_name, vector<string> attributes, vector<string> keys){
	//vector<vector<string>> TEMP_TABLE;				//initialize temporary table that will be pushed into RELATION_TABLE vector  - JM
	/*TEMP_TABLE[0][0] = table_name;					//initialize table name
	va_list ATTRIBUTE_LIST;
	va_start(ATTRIBUTE_LIST, data);
	
	for(unsigned int i=0; i<num_attr; i++){
		TEMP_TABLE[1][i]=data;
		va_arg(ATTRIBUTE_LIST, data);
	}

	va_end(ATTRIBUTE_LIST);
	RELATION_TABLE.push_back(TEMP_TABLE);*/
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
	for(unsigned int i = 3; i < VIEW_LIST[relation_index].size(); ++i)
		RELATION_LIST[i].push_back(VIEW_LIST[relation_index][i]);
}

void Database::remove(string relation_name, int row_index){
	int relation_index = -1;
	relation_index = get_relation_index(relation_name);

	// ERROR - no such relation
	if(relation_index == -1);

	RELATION_LIST[relation_index].erase(RELATION_LIST[relation_index].begin() + row_index);
}

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
