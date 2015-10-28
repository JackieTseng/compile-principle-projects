#ifndef INTERPRETER_H
#define INTERPRETER_H
#include "table.cpp"

class Interpreter {
    public:
        Interpreter();// the default constructor
        Interpreter(vector<Token>);// initialize the interpreter with given tokens
        void interpret();
        int execute(vector<Token>);
    
    private:
	    /* the list of table */
	    vector<Table> database;
        vector<Token> sequence;
        Parser parser;
};

#endif
