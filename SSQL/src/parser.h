#ifndef PARSER_H
#define PARSER_H
#include "token.cpp"

class Parser {
    public:
        Parser();
        Parser(vector<Token>); // initialize the parser with input token sequence
        void inputSequence(vector<Token>);
        bool parsing(); // check if an expression match the grammar

    private:
        int ip;
        vector<Token> sequence;
        vector<int> pdt[where_clause + 1][dollar + 1];
        void initPdt();  // initialize the predict analysis table
};

#endif
