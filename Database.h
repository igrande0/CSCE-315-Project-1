#ifndef DATABASE_H_
#define DATABASE_H_

#include <cstdarg>
#include <string>
#include <vector>

using namespace std;

class Database{
private:
	/* Table format:
	 * v[0][0] <= title of table
	 * v[1][x] <= attribute list
	 * v[2][x] <= tuple
	 * v[3][x] <= tuple
	 * ...
	 */

	vector<vector<vector<string> > > RELATION_LIST;
	vector<vector<vector<string> > > VIEW_LIST;

public:
	// Query Functions
	void select(string view_name, string in_table_name, string left_arg, string right_arg, string comparison);
	void project(string view_name, string in_table_name, vector<string> attributes);
	void rename(string out_view_name, string in_table_name, vector<string> attributes);
	void set_union(string view_name, string table1_name, string table2_name);
	void set_difference(string view_name, string table1_name, string table2_name);
	void cross_product(string view_name, string table1_name, string table2_name);
    
	// Command Functions
	void close(string table_name);
	void write(string table_name);
	void show(string table_name);
	void create(string table_name, vector<string> attributes, vector<string> keys);
	void update(string relation_name, string left_arg, string right_arg, string condition, vector<string> attributes);
	void insert_tuple(string relation_name, vector<string> tuple);
	void insert_view(string relation_name, string view_name);
	void remove(string table_name, string left_arg, string right_arg, string comparison);
};

#endif
