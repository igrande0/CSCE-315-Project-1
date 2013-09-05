#include "Database.h"
//Function Definitions

void select(string view_name, string in_table_name, string left_arg, string right_arg, string comparison){

}
//------------------------------------------------------------------------------------------------------------------------------------------------------
void project(string view_name, string in_table_name, string attr, ...){

}
//------------------------------------------------------------------------------------------------------------------------------------------------------
void rename(string relation_name, string attr, ...){

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

}
//------------------------------------------------------------------------------------------------------------------------------------------------------
void update(string relation_name, string left_arg, string right_arg, string condition, string data, ...){

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
void insert_view(string table_name){

}
//------------------------------------------------------------------------------------------------------------------------------------------------------
void remove(string table_name, string left_arg, string right_arg, string comparison){

}