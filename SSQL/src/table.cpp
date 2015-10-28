#include "table.h"

Table::Table() {
}

Table::Table(string n) {
    tableName = n;
}

string Table::getTableName() {
    return tableName;
}
    
int Table::getColByName(string n) {
    return nameToCol[n];
}

int Table::addCol(string name) {
    /* if already have the column with same name */
    if(nameToCol.find(name) != nameToCol.end()) {
        return -1;
    }
    /* add the new column and map its name to its column number */
    table_.push_back(vector<int>());
    nameToCol.insert(pair<string, int>(name, table_.size() - 1));
    colName.push_back(name);
    return 1;
}

/* add one more default attribute */
void Table::setDefaultValue(int value) {
    defaultValue.push_back(value);
}

/* add one more primary key */
int Table::addPrimaryKey(string name) {
    /* if there is no such a column in the table */
    if(nameToCol.find(name) == nameToCol.end()) {
        cout << "Create table failed: no such a column in the table" << endl;
        return -1;
    }
    int col = nameToCol[name];
    for(int i = 0; i < primaryKey.size(); ++i) {
        if(primaryKey[i] == col) {
            cout << "Create table failed: there is already same column in the primary key" << endl;
            return -1;
        }
    }
    primaryKey.push_back(nameToCol[name]);
    return 1;
}

/* insert a record to the table */
int Table::insert_(vector<string> cols, vector<int> values_) {
    for(int i = cols.size() - 1; i >= 0; --i) {
        /* if there is no such a column in the table */
        if(nameToCol.find(cols[i]) == nameToCol.end()) {
            cout << "Insert failed: no such a column" << endl;
            return -1;
        }
    }
    bool flag = true;
    vector<int> primaryValues;
    for(int i = 0; i < table_.size(); ++i)
        primaryValues.push_back(defaultValue[i]);
    for(int i = 0; i < cols.size(); ++i) {
        primaryValues[getColByName(cols[i])] = values_[i];
    }
    /* if there is primary key and the table is not empty, check whether the record is different */
    if(primaryKey.size() != 0 && table_[0].size() != 0) {
        for(int i = 0, cnt; i < table_[0].size(); ++i) if(notDelete[i]) {
            cnt = 0;
            for(int j = primaryKey.size() - 1; j >= 0; --j) {
                if(table_[primaryKey[j]][i] == primaryValues[primaryKey[j]]) {
                    ++cnt;
                }
            }
            if(cnt == (int)primaryKey.size()) {
                flag = false;
                break;
            }
        }
    }
    /* if it is ok to insert */
    if(flag) {
        for(int i = 0; i < table_.size(); ++i) {
            table_[i].push_back(primaryValues[i]);
        }
        notDelete.push_back(true);
        return 1;
    }
    else {
        cout << "Insert failed: primary keys conflict" << endl;
        return -1;
    }
}

/* output all the matched records */
int Table::query_(vector<string> colsName, vector<Token> temp) {
    if(table_[0].size() == 0) {
        cout << "Query failed: the table is empty!" << endl;
        return 1;
    }
    vector<int> cols;/* the attributes we want */
    if(colsName.size() > 0) {
        /* check if there are some columns not exist */
        for(int i = 0; i < colsName.size(); ++i) {
            if(nameToCol.find(colsName[i]) == nameToCol.end()) {
                cout << "Query failed: no such a column " << colsName[i] << endl;
                return -1;
            }
        }
        for(int i = 0; i < colsName.size(); ++i) {
            cols.push_back(getColByName(colsName[i]));
        }
    }
    else {
        /* correspond to select *  */
        for(int i = 0; i < table_.size(); ++i)
            cols.push_back(i);
    }
    for(int i = 0; i < temp.size(); ++i) {
        if(temp[i].getIndex() == _id && nameToCol.find(temp[i].getValue()) == nameToCol.end()) {
            cout << "Query failed: no such a column " << temp[i].getValue() << endl;
            return -1;
        }
    }
    int maxLen = -1, dist, cnt = 0;
    for(int i = 0; i < cols.size(); ++i)
        maxLen = max(maxLen, (int)colName[cols[i]].size());
    maxLen += 10;
    cout << '|';
    for(int i = 0; i < cols.size(); ++i) {
        for(int j = 0; j < maxLen; ++j)
            cout << '-';
        cout << '|';
    }
    cout << endl << '|';
    for(int i = 0; i < cols.size(); ++i) {
        dist = maxLen - 5 - colName[cols[i]].size();
        cout << "     ";
        cout << colName[cols[i]];
        for(int j = 0; j < dist; ++j)
            cout << ' ';
        cout << '|';
    }
    cout << endl << '|';
    for(int i = 0; i < cols.size(); ++i) {
        for(int j = 0; j < maxLen; ++j)
            cout << '-';
        cout << '|';
    }
    cout << endl;
    for(int i = 0; i < table_[0].size(); ++i) if(notDelete[i] == true) {//if the record has not been deleted
        vector<Token> boolExpr = temp;
        /* replace all _id with corresponding number in the table */
        for(int j = 0; j < boolExpr.size(); ++j) {
            if(boolExpr[j].getIndex() == _id) {
                boolExpr[j] = Token(num, getString(table_[getColByName(boolExpr[j].getValue())][i]));
            }
        }
        /* if the boolean expression results true then output the record */
        if(exprEvaluate(boolExpr).getIntValue()) {
            ++cnt;
            cout << '|';
            for(int j = 0; j < cols.size(); ++j) {
                cout << "     ";
                cout << table_[cols[j]][i];
                dist = (maxLen - 5 - getString(table_[cols[j]][i]).size());
                for(int k = 0; k < dist; ++k)
                    cout << ' ';
                cout << '|';
            }
            cout << endl << '|';
            for(int p = 0; p < cols.size(); ++p) {
                for(int q = 0; q < maxLen; ++q)
                    cout << '-';
                cout << '|';
            }
            cout << endl;
        }
    }
    cout << cnt << " rows affected" << endl;
    return 1;
}

/* delete all the matched records */
int Table::delete_(vector<Token> temp) {
    if(table_[0].size() == 0) {
        cout << "Delete failed: the table is empty!" << endl;
        return 1;
    }
    /* check if there are some columns not exist */
    for(int j = 0; j < temp.size(); ++j) {
        if(temp[j].getIndex() == _id) {
            if(nameToCol.find(temp[j].getValue()) == nameToCol.end()) {
                cout << "Delete failed: no such a column " << temp[j].getValue() << endl;
                return -1;
            }
        }
    }
    int cnt = 0;
    for(int i = 0; i < table_[0].size(); ++i) if(notDelete[i] == true) {//if the record has not been deleted
        vector<Token> boolExpr = temp;
        /* replace all _id with corresponding number in the table */
        for(int j = 0; j < boolExpr.size(); ++j) {
            if(boolExpr[j].getIndex() == _id) {
                boolExpr[j] = Token(num, getString(table_[getColByName(boolExpr[j].getValue())][i]));
            }
        }
        /* if the boolean expression results true then sets the record deleted */
        if(exprEvaluate(boolExpr).getIntValue()) {
            /* truly delete it only when write to the file so as to make the program run faster */
            notDelete[i] = false;
            ++cnt;
        }
    }
    cout << "Delete success: " << cnt << " rows affected" << endl;
    return 1;
}
