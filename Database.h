#include <cstdarg>
#include <string>
#include <vector>


class Database{
private:
	/* Table format:
	 * v[0][0] <= title of table
	 * v[1][x] <= attribute list
	 * v[2][x] <= tuple
	 * v[3][x] <= tuple
	 * ...
	 */
	typedef vector<vector<string>> Table;

	vector<Table> RELATION_LIST;
	vector<Table> VIEW_LIST;

public:
	//Query Functions
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
