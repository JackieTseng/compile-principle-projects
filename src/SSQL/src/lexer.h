#ifndef LEXER_H
#define LEXER_H
#include "token.cpp"

class Lexer {
    public:
        Lexer(string);
        vector<Token> getTokenList();
        void initKeyword();
        bool isKeyword(string);
        int readOnePage();
        void moveForward(pointer&, bool);
        int pointerDist(pointer, pointer);
        string cut(pointer, pointer);
        int splitTokens();
    
    private:
        string filename;
        //Determine which buffer is currently in use
        int flag;
        fstream fin;
        //The two page buffer
        char buffer[2][BUFFER_SIZE];
        int len;
        vector<Token> tokenList;
        vector<string> keyword;
        map<string, int> keywordMap;
};

#endif
