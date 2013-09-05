#include "Database.h"
//Function Definitions

void select(string view_name, string in_table_name, string left_arg, string right_arg, string comparison){

}
//------------------------------------------------------------------------------------------------------------------------------------------------------
void project(string view_name, string in_table_name, string attr, ...){

}
//------------------------------------------------------------------------------------------------------------------------------------------------------
void rename(string relation_name, string attr, ...){                      //need more arguments? - JM
	int VECTOR_INDEX;

	for(int i=0; i<RELATION_LIST.size(); i++)
		if(relation_name == RELATION_LIST[i][0][0])
			VECTOR_INDEX=i;
	//need original attribute names as well as new ones - JM
	//possibly pass an int to tell how many attributes are beign changed

}
//------------------------------------------------------------------------------------------------------------------------------------------------------
void set_union(string view_name, string relation1_name, string relation2_name){

}
//------------------------------------------------------------------------------------------------------------------------------------------------------
void set_difference(string view_name, string relation1_name, string relation2_name){

}
//------------------------------------------------------------------------------------------------------------------------------------------------------
void cross_product(string view_name, string relation1_name, string relation2_name){

}
//------------------------------------------------------------------------------------------------------------------------------------------------------
void close(string table_name){

}
//------------------------------------------------------------------------------------------------------------------------------------------------------
void write(string table_name){

}
//------------------------------------------------------------------------------------------------------------------------------------------------------
void show(string table_name){

}
//------------------------------------------------------------------------------------------------------------------------------------------------------
void create(string table_name, int num_attr, string data, ...){
	vector<vector<string>> TEMP_TABLE;				//initialize temporary table that will be pushed into RELATION_TABLE vector  - JM
	TEMP_TABLE[0][0] = table_name;					//initialize table name
	va_list ATTRIBUTE_LIST;
	va_start(ATTRIBUTE_LIST, data);
	
	for(int i=0; i<num_attr; i++){
		TEMP_TABLE[1][i]=data;
		va_arg(ATTRIBUTE_LIST, data);
	}

	va_end(ATTRIBUTE_LIST);
	RELATION_TABLE.push_back(TEMP_TABLE);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
void update(string relation_name, string left_arg, string right_arg, string condition, string data, ...){       //need to rethink arguments - JM
	int VECTOR_INDEX, COLUMN_INDEX;

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
					//update information
			break;
        case "!=":
			for(int i=0; i<RELATION_LIST[VECTOR_INDEX].size(); i++)
				if(right_arg == RELATION_LIST[VECTOR_INDEX][i][COLUMN_INDEX])
					//update information
            break;
        case "<":
			for(int i=0; i<RELATION_LIST[VECTOR_INDEX].size(); i++)
				if(right_arg == RELATION_LIST[VECTOR_INDEX][i][COLUMN_INDEX])
					//update information
            break;
		case ">":
			for(int i=0; i<RELATION_LIST[VECTOR_INDEX].size(); i++)
				if(right_arg == RELATION_LIST[VECTOR_INDEX][i][COLUMN_INDEX])
					//update information
            break;
		case "<=":
			for(int i=0; i<RELATION_LIST[VECTOR_INDEX].size(); i++)
				if(right_arg == RELATION_LIST[VECTOR_INDEX][i][COLUMN_INDEX])
					//update information
            break;
		case ">=":
			for(int i=0; i<RELATION_LIST[VECTOR_INDEX].size(); i++)
				if(right_arg == RELATION_LIST[VECTOR_INDEX][i][COLUMN_INDEX])
					//update information
            break;
      }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
void insert_tuple(string data, ...){
	va_list ARGUMENT_LIST;
	va_start(ARGUMENT_LIST, data);
	int VECTOR_INDEX;

	for(int i=0; i<RELATION_LIST.size(); i++)
		if(data == RELATION_LIST[i][0][0])
			VECTOR_INDEX=i;

	va_arg(ARGUMENT_LIST, string);

	for(int i=0; data != NULL; i++){
		RELATION_LIST[VECTOR_INDEX][][] = data;			//need to find insert correct indicies - JM
		va_arg(ARGUMENT_LIST, string);
	}
	va_end(ARGUMENT_LIST);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
void insert_view(string table_name){                 //need to pass a vector of vectors or an index to a view table - JM
	int VECTOR_INDEX;

	for(int i=0; i<RELATION_LIST.size(); i++)
		if(relation_name == RELATION_LIST[i][0][0])
			VECTOR_INDEX=i;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
void remove(string table_name, string left_arg, string right_arg, string comparison){
	
}
