class Database{
  Private:
    List<vector<vector<string>>> TABLE_LIST;
  Public:
    //Query Functions             Just defining base functions return types and arguments will most likely not be correct -JM 
    void select();
    void project();
    void rename();
    void set_union();
    void set_difference();
    void cross_product();
    
    //Command Functions
    void insert();
    void close();
    void write();
    void show();
    void create();
    void update();
    void insert();
    void delete();
}
