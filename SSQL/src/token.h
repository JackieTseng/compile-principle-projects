#ifndef TOKEN_H
#define TOKEN_H
#include "basic.h"

class Token {
    public:
        Token();
        Token(int, string);
        int getIndex();
        int getIntValue();
        string getValue();
        void setIndex(int);
        void setValue(string);
    
    private:
        int index; //Token ID
        string value; //Token content
};

#endif // TOKEN_H
