#include "interpreter.h"

Interpreter::Interpreter() {
}

Interpreter::Interpreter(vector<Token> s) {
    sequence = s;
}

/* interpret the input statements separately */
void Interpreter::interpret() {
    int ip = 0;
    while(ip != sequence.size()) {
        vector<Token> temp;
        /* one statement each time */
        while(ip != sequence.size() && sequence[ip].getIndex() != semicolon) {
            temp.push_back(sequence[ip++]);
        }
        if(sequence[ip].getIndex() == semicolon) {
            temp.push_back(sequence[ip++]);
        }
        parser.inputSequence(temp);
        /* if the statement match the grammar then execute it */
        if(parser.parsing() == true) {
            execute(temp);
        }
    }
}

int Interpreter::execute(vector<Token> temp) {
    int ip = 0;
    /* if it is a create statement */
    if(temp[ip].getIndex() == create) {
        string tableName = temp[ip + 2].getValue();
        /* check if there is already table with the same name */
        for(int i = 0; i < database.size(); ++i)
        if(database[i].getTableName() == tableName) {
            cout << "Create table failed: there is already a table with the same name" << endl;
            return -1;
        }
        /* initialize the table with given name */
        Table newTable(tableName);
        ip += 4;
        bool flag = false; // to check if already set primary key
        int cols = 0, keys = 0;
        vector<string> tempPrimaryKey;
        while(temp[ip].getIndex() != bracket_r) {
            /* if it is defining a new column */
            if(temp[ip].getIndex() == _id) {
                /* columns can't have same name */
                if(newTable.addCol(temp[ip].getValue()) == -1) {
                    cout << "Create table failed: there are already some columns with the same name" << endl;
                    return -1;
                }
                ++cols; // count how many columns we have
                /* if there is no default value */
                if(temp[ip + 2].getIndex() == comma || temp[ip + 2].getIndex() == bracket_r) {
                    newTable.setDefaultValue(0);
                    ip += 2;
                }
                else {
                    int pos = ip + 4;
                    int left_ = 0;
                    vector<Token> tempExpr;
                    /* achieve the expression of the default value */
                    while(temp[pos].getIndex() != comma) {
                        if(temp[pos].getIndex() == bracket_l)
                            ++left_;
                        else if(temp[pos].getIndex() == bracket_r) {
                            --left_;
                            if(left_ < 0) {
                                break;
                            }
                        }
                        tempExpr.push_back(temp[pos++]);
                    }
                    Token result = exprEvaluate(tempExpr);
                    if(result.getIndex() == dollar) {
                        cout << "Error arithmetic expression: divide by zero" << endl;
                        return -1;
                    }
                    /* set the default value with the calculate result */
                    newTable.setDefaultValue(result.getIntValue());
                    ip = pos;
                }
            }
            else if(temp[ip].getIndex() == primary){
                if(flag == true) {
                    cout << "Create table failed: the table already has primary key" << endl;
                    return -1;
                }
                ip += 3;
                flag = true;
                while(temp[ip].getIndex() != bracket_r) {
                    tempPrimaryKey.push_back(temp[ip].getValue());
                    ++keys; // count how many primary key we have
                    if(temp[ip + 1].getIndex() == comma) {
                        ip += 2;
                    }
                    else {
                        ++ip;
                    }
                }
                ++ip;
            }
            else {
                ++ip;
            }
        }
        /* check if all the primary keys are valid */
        for(int i = 0; i < tempPrimaryKey.size(); ++i)
            if(newTable.addPrimaryKey(tempPrimaryKey[i]) == -1) {
                return -1;
            }
        if(cols > 100 || keys > 100) {
            cout << "Create table failed: too much cols or too much keys" << endl;
            return -1;
        }
        database.push_back(newTable);
        cout << "Table " << tableName << " created" << endl;
    }
    else if(temp[ip].getIndex() == _insert) {
        /* achieve the desired table's name */
        string tableName = temp[ip + 2].getValue();
        ip += 4;
        vector<string> colToBeInserted;
        /* achieve the columns which we provide values */
        while(temp[ip].getIndex() != bracket_r) {
            colToBeInserted.push_back(temp[ip].getValue());
            if(temp[ip + 1].getIndex() == comma) {
                ip += 2;
            }
            else {
                ++ip;
            }
        }
        vector<int> valuesToBeInserted;
        ip += 3;
        int left_;
        /* achieve the values we provide, probably in the form of expressions */
        while(temp[ip].getIndex() != bracket_r) {
            int pos = ip;
            vector<Token> tempExpr;
            /* cut the expression */
            left_ = 0;
            while(temp[pos].getIndex() != comma) {
                if(temp[pos].getIndex() == bracket_l)
                    ++left_;
                else if(temp[pos].getIndex() == bracket_r) {
                    --left_;
                    if(left_ < 0) {
                        break;
                    }
                }
                tempExpr.push_back(temp[pos++]);
            }
            Token result = exprEvaluate(tempExpr);
            if(result.getIndex() == dollar) {
                cout << "Error arithmetic expression: divide by zero" << endl;
                return -1;
            }
            valuesToBeInserted.push_back(result.getIntValue());
            ip = pos;
            if(temp[ip].getIndex() == comma) {
                ++ip;
            }
        }
        /* if the size of the columns and given values don't match */
        if(colToBeInserted.size() < valuesToBeInserted.size()) {
            cout << "Insert failed: arguments don't match" << endl;
            return -1;
        }
        set<string> cnt;
        for(int i = 0; i < colToBeInserted.size(); ++i)
            cnt.insert(colToBeInserted[i]);
        if(cnt.size() != colToBeInserted.size()) {
            cout << "Insert failed: there are some columns with same name" << endl;
            return -1;
        }
        int which;
        for(which = database.size() - 1; which >= 0; --which) {
            if(database[which].getTableName() == tableName) {
                if(database[which].insert_(colToBeInserted, valuesToBeInserted) != 1) {
                    return -1;
                }
                break;
            }
        }
        if(which < 0) {
            cout << "Insert failed: no such a table" << endl;
            return -1;
        }
        cout << "Insert success!" << endl;
    }
    else if(temp[ip].getIndex() == _select) {
        vector<string> colToBeQueryed;
        vector<Token> boolExpr;
        string tableName;
        /* select all columns */
        if(temp[ip + 1].getIndex() == star) {
            ip += 2;
        }
        else {
            ++ip;
            /* achieve the columns we want to query */
            while(temp[ip].getIndex() == _id) {
                colToBeQueryed.push_back(temp[ip++].getValue());
                if(temp[ip].getIndex() == comma) {
                    ++ip;
                }
                else if(temp[ip].getIndex() == from) {
                    break;
                }
            }
        }
        /* the name of the table we want to query */
        tableName = temp[ip + 1].getValue();
        /* if there are given conditions */
        if(temp[ip + 2].getIndex() == where) {
            ip += 3;
            int pos = ip;
            while(temp[pos].getIndex() != semicolon) {
                boolExpr.push_back(temp[pos++]);
            }
        }
        /* if there is no where clause */
        else {
            boolExpr.push_back(Token(_bool, "1"));
        }
        int which;
        for(which = database.size() - 1; which >= 0; --which) {
            if(database[which].getTableName() == tableName) {
                if(database[which].query_(colToBeQueryed, boolExpr) != 1) {
                    return -1;
                }
                break;
            }
        }
        if(which < 0) {
            cout << "Query failed: no such a table" << endl;
            return -1;
        }
    }
    else if(temp[ip].getIndex() == _delete) {
        string tableName;
        vector<Token> boolExpr;
        /* the name of the table we want to delete record from it */
        tableName = temp[ip + 2].getValue();
        ip += 3;
        /* if there are given conditions */
        if(temp[ip].getIndex() == where) {
            int pos = ip + 1;
            while(temp[pos].getIndex() != semicolon) {
                boolExpr.push_back(temp[pos++]);
            }
        }
        /* if there is no where clause */
        else {
            boolExpr.push_back(Token(_bool, "1"));
        }
        int which;
        for(which = database.size() - 1; which >= 0; --which) {
            if(database[which].getTableName() == tableName) {
                if(database[which].delete_(boolExpr) != 1) {
                    return -1;
                }
                break;
            }
        }
        if(which < 0) {
            cout << "Delete failed: no such a table" << endl;
            return -1;
        }
    }
    return 1;
}
