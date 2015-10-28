#ifndef TABLE_H
#define TABLE_H
#include "basic.h"

class Table {
    public:
        Table();
        Table(string);
        int addCol(string);
        int addPrimaryKey(string);
        string getTableName();
        int getColByName(string);
        void setDefaultValue(int);
        int insert_(vector<string>, vector<int>);
        int query_(vector<string>, vector<Token>);
        int delete_(vector<Token>);

    private:
        string tableName;
        vector<string> colName;
        vector<int> primaryKey;
        vector<int> defaultValue;
        vector<vector<int> > table_; // the column stored table
        map<string, int> nameToCol; // map the column name to the corresponding column
        vector<bool> notDelete;  // items not been deleted yet
};

#endif
