#include <cstdarg>
#include <string>
#include <vector>

typedef vector<vector<string>> Table;

class Database{
	Private:
		vector<Table> RELATION_LIST;
		vector<Table> VIEW_LIST;

	Public:
		//Query Functions             Just defining base functions return types and arguments will most likely not be correct -JM 
		void select(string view_name, string in_table_name, string left_arg, string right_arg, string comparison);
		void project(string view_name, string in_table_name, string attr, ...);
		void rename(string relation_name, string attr, ...);
		void set_union(string view_name, string relation1_name, string relation2_name);
		void set_difference(string view_name, string relation1_name, string relation2_name);
		void cross_product(string view_name, string relation1_name, string relation2_name);
    
		//Command Functions
		void close(string table_name);
		void write(string table_name);
		void show(string table_name);
		void create(string table_name, int num_attr, string data, ...);
		void update(string relation_name, string left_arg, string right_arg, string condition, string data, ...);
		void insert_tuple(string data, ...);
		void insert_view(string table_name);
		void remove(string table_name, string left_arg, string right_arg, string comparison);
};
