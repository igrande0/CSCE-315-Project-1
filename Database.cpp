#include "Database.h"

#include <cctype>

#define RELATION 0
#define VIEW 1

/*------------------------------------------------------------------------------------*/
/* QUERY FUNCTIONS */
/*------------------------------------------------------------------------------------*/

void Database::select(string view_name, string in_table_name, string left_arg, string right_arg, string comparison){

}

void Database::project(string view_name, string in_table_name, vector<string> attributes){

}

/* rename all attributes in an existing table (can be a relation or view)
 * add resulting table into the view list
 */
void Database::rename(string out_view_name, string in_table_name, vector<string> attributes){
	int in_table_type;
	int in_table_index = -1;
	int out_view_index;

	// look for table in relation list
	for(int i = 0; i < RELATION_LIST.size(); ++i) 
		if(in_table_name == RELATION_LIST[i][0][0]) {
			in_table_index = i;
			in_table_type = RELATION;
			break;
		}
	
	// if table isn't found, look in view list
	if(in_table_index == -1)
		for(int i = 0; i < VIEW_LIST.size(); ++i) 
			if(in_table_name == VIEW_LIST[i][0][0]) {
				in_table_index = i;
				in_table_type = VIEW;
				break;
			}

	//*********BEGIN MISSING ERROR HANDLING***********
	// ERROR - no such table 
	if(in_table_index == -1);
		
	// ERROR - incorrect number of attributes 
	if(in_table_type == RELATION && RELATION_LIST[in_table_index][1].size() != attributes.size());
	if(in_table_type == VIEW && VIEW_LIST[in_table_index][1].size() != attributes.size());
	//*********END MISSING ERROR HANDLING***********
		
	// create new view table with 2 rows (title and attribute rows)
	VIEW_LIST.push_back(vector<vector<string> >(2));
	out_view_index = VIEW_LIST.size() - 1;

	// set title
	VIEW_LIST[out_view_index][0].push_back(out_view_name);
	
	// set attributes
	VIEW_LIST[out_view_index][1] = attributes;

	// copy remaining data
	if(in_table_type == RELATION)
		for(int i = 2; i < RELATION_LIST[in_table_index].size(); ++i)
			VIEW_LIST[out_view_index].push_back(RELATION_LIST[in_table_index][i]);
	else
		for(int i = 2; i < VIEW_LIST[in_table_index].size(); ++i)
			VIEW_LIST[out_view_index].push_back(VIEW_LIST[in_table_index][i]);
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
	
	for(int i=0; i<num_attr; i++){
		TEMP_TABLE[1][i]=data;
		va_arg(ATTRIBUTE_LIST, data);
	}

	va_end(ATTRIBUTE_LIST);
	RELATION_TABLE.push_back(TEMP_TABLE);*/
}

void Database::update(string relation_name, string left_arg, string right_arg, string condition, vector<string> attributes){       //need to rethink arguments - JM
	/*int VECTOR_INDEX, COLUMN_INDEX;
	vector<int> ROW_INDECIES;
	for(int i=0; i<RELATION_LIST.size(); i++)
		if(relation_name == RELATION_LIST[i][0][0])
			VECTOR_INDEX=i;

	for(int i=0; i<RELATION_LIST[VECTOR_INDEX][1].size(); i++)
		if(left_arg == RELATION_LIST[VECTOR_INDEX][1][i])
			COLUMN_INDEX=i;
	
	switch (condition){
		case "==":
			for(int i=0; i<RELATION_LIST[VECTOR_INDEX].size(); i++)
				if(right_arg == RELATION_LIST[VECTOR_INDEX][i][COLUMN_INDEX])
					ROW_INDICIES.push_back(i);	
			break;
        	case "!=":
			for(int i=0; i<RELATION_LIST[VECTOR_INDEX].size(); i++)
				if(right_arg != RELATION_LIST[VECTOR_INDEX][i][COLUMN_INDEX])
					ROW_INDICIES.push_back(i);
            		break;
        	case "<":
			for(int i=0; i<RELATION_LIST[VECTOR_INDEX].size(); i++)
				if(right_arg < RELATION_LIST[VECTOR_INDEX][i][COLUMN_INDEX])
					ROW_INDICIES.push_back(i);
            		break;
		case ">":
			for(int i=0; i<RELATION_LIST[VECTOR_INDEX].size(); i++)
				if(right_arg > RELATION_LIST[VECTOR_INDEX][i][COLUMN_INDEX])
					ROW_INDICIES.push_back(i);
            		break;
		case "<=":
			for(int i=0; i<RELATION_LIST[VECTOR_INDEX].size(); i++)
				if(right_arg <= RELATION_LIST[VECTOR_INDEX][i][COLUMN_INDEX])
					ROW_INDICIES.push_back(i);
            		break;
		case ">=":
			for(int i=0; i<RELATION_LIST[VECTOR_INDEX].size(); i++)
				if(right_arg >= RELATION_LIST[VECTOR_INDEX][i][COLUMN_INDEX])
					ROW_INDICIES.push_back(i);
            		break;
      }
      for(int i=0; i< ROW_INDICIES.size(); i++){
      	      RELATION_LIST[VECTOR_INDEX][ROW_INDICIES[i]][COLUMN_INDEX] == attributes[0];	
      }*/
}

void Database::insert_tuple(string relation_name, vector<string> tuple){
	int table_index = -1;

	// find location of relation table
	for(int i = 0; i < RELATION_LIST.size(); i++)
		if(relation_name == RELATION_LIST[i][0][0]) {
			table_index = i;
			break;
		}

	//*********BEGIN MISSING ERROR HANDLING***********
	// ERROR - no such table 
	if(table_index == -1);
	//*********END MISSING ERROR HANDLING***********

	// add new tuple to the end of the table
	RELATION_LIST[table_index].push_back(tuple);
}

void Database::insert_view(string relation_name, string view_name){
	int view_index = -1;
	int relation_index = -1;

	// find location of view table
	for(int i = 0; i < VIEW_LIST.size(); i++)
		if(view_name == VIEW_LIST[i][0][0]) {
			view_index = i;
			break;
		}

	// find location of relation table
	for(int i = 0; i < RELATION_LIST.size(); i++)
		if(relation_name == RELATION_LIST[i][0][0]) {
			relation_index = i;
			break;
		}

	//*********BEGIN MISSING ERROR HANDLING***********
	// ERROR - no such table 
	if(view_index == -1);
	if(relation_index == -1);
	//*********END MISSING ERROR HANDLING***********

	// copy values from view table to relation table
	for(int i = 2; i < VIEW_LIST[relation_index].size(); ++i)
		RELATION_LIST[i].push_back(VIEW_LIST[relation_index][i]);
}

void Database::remove(string table_name, string attribute_name, string right_arg, string comparison_op){
	int relation_index = -1;
	int COLUMN_INDEX = -1;
	string attribute_type;

	for(int i = 0; i < RELATION_LIST.size(); i++)
		if(relation_name == RELATION_LIST[i][0][0])
			relation_index=i;

	for(int i = 0; i < RELATION_LIST[relation_index][1].size(); i++)
		if(attribute_name == RELATION_LIST[relation_index][1][i])
			column_index=i;

	//*********BEGIN MISSING ERROR HANDLING***********
	// ERROR - no such table 
	if(relation_index == -1);
	// ERROR - no such attribute
	if(column_index == -1);
	//*********END MISSING ERROR HANDLING***********

	attribute_type = RELATION_LIST[relation_index][2][column_index];
	
	for(int i=0; i<RELATION_LIST[relation_index].size(); i++)
		if(compare(attribute_type, RELATION_LIST[relation_index][i][column_index], right_arg, comparison_op))
			RELATION_LIST[relation_index].erase(RELATION_LIST[relation_index].begin()+i);	
}

bool Database::compare(string attribute_type, string left_arg, string right_arg, string comparison_op) {
	switch(attribute_type[0]) {
		// INT or INTEGER
		case 'I':
			return compare<int>(stoi(left_arg), stoi(right_arg), comparison_op);

		// FLOAT  or REAL
		case 'F': case 'R':
			return compare<float>(stof(left_arg), stof(right_arg), comparison_op); 

		// CHAR or VARCHAR
		case 'C': case 'V':
			return compare<string>(left_arg, right_arg, comparison_op);

		// DATE or TIME 
		case 'D': case 'T': {
			return compare<int>(extract_digits(left_arg), extract_digits(right_arg), comparison_op);
		}
	}
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
	else if(comparison_op == ">=")
		return left_arg >= right_arg;
}

int Database::extract_digits(string input) {
	for(int i = input.size(); i > 0; --i)
		if(!isdigit(input[i-1]))
			input.erase(input.begin() + i);

	return stoi(input);
}
