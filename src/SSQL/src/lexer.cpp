#include "lexer.h"

Lexer::Lexer(string file) {
    filename = file;
    fin.open(filename.c_str(), fstream::in);
    //Use the first buffer first
    flag = 0;
    initKeyword();
}

vector<Token> Lexer::getTokenList() {
    splitTokens();
    return tokenList;
}

void Lexer::initKeyword() {
    keyword.push_back(string("create"));
    keyword.push_back(string("default"));
    keyword.push_back(string("delete"));
    keyword.push_back(string("from"));
    keyword.push_back(string("insert"));
    keyword.push_back(string("int"));
    keyword.push_back(string("into"));
    keyword.push_back(string("key"));
    keyword.push_back(string("primary"));
    keyword.push_back(string("select"));
    keyword.push_back(string("table"));
    keyword.push_back(string("values"));
    keyword.push_back(string("where"));
    keywordMap.insert(pair<string, int>("create", create));
    keywordMap.insert(pair<string, int>("default", _default));
    keywordMap.insert(pair<string, int>("delete", _delete));
    keywordMap.insert(pair<string, int>("from", from));
    keywordMap.insert(pair<string, int>("insert", _insert));
    keywordMap.insert(pair<string, int>("int", _int));
    keywordMap.insert(pair<string, int>("into", into));
    keywordMap.insert(pair<string, int>("key", key));
    keywordMap.insert(pair<string, int>("primary", primary));
    keywordMap.insert(pair<string, int>("select", _select));
    keywordMap.insert(pair<string, int>("table", table));
    keywordMap.insert(pair<string, int>("values", values));
    keywordMap.insert(pair<string, int>("where", where));
}

bool Lexer::isKeyword(string name) {
    return binary_search(keyword.begin(), keyword.end(), name);
}

int Lexer::readOnePage() {
    if(!fin)
        return -1;
    //Read only terminate when meet with EOF
    fin.get(buffer[flag], BUFFER_SIZE, EOF);
    // Replace the last char \n with EOF
    buffer[flag][strlen(buffer[flag])] = EOF;
    //Change the flag indicate the next buffer to be use
    flag = (flag) ? 0 : 1;
    return 1;
}

/***
    move the pointer forward for one position
    the boolean read indicate that whether the Lexer object should read a new page
    If the moved pointer is the "forward" pointer, read = true, else, read = false
***/
void Lexer::moveForward(pointer& forward, bool read) {
    //Look ahead, if EOF, go to another buffer's begin
    if (buffer[forward.flag][forward.index + 1] == EOF) {
        if (read)
            readOnePage();
        forward.index = 0;
        forward.flag = (forward.flag) ? 0 : 1;
    } else {
        ++forward.index;
    }
}

//Calculate the distance between two pointers
int Lexer::pointerDist(pointer left, pointer right) {
    if (left.flag == right.flag)
        return right.index - left.index;
    else {
        int k = 0;
        while (buffer[left.flag][k] != EOF) {
            k++;
        }
        return k - left.index + right.index + 1;
    }
}

//Get the substring between the two pointers
string Lexer::cut(pointer left, pointer right) {
    string temp = "";
    //If the pointers are in the same buffer
    if (left.flag == right.flag) {
        for(int i = left.index; i < right.index; ++i) {
            temp += buffer[left.flag][i];
        }
    }
    //If they are in different buffer
    else {
        int i = left.index;
        while (buffer[left.flag][i] != EOF) {
            temp += buffer[left.flag][i++];
        }
        for (i = 0; i < right.index; ++i)
            temp += buffer[right.flag][i];
    }
    return temp;
}

int Lexer::splitTokens() {
    if (readOnePage() == -1)
        return -1;
    int state = 0;
    pointer lexemeBegin, forward;
    while (1) {
        char beginChar = buffer[lexemeBegin.flag][lexemeBegin.index];
        if (beginChar == EOF)
            break;
        if(state == 0) {
            if(isCharacter(buffer[forward.flag][forward.index])) {
                state = 1;
            }
            else if(isNumber(buffer[forward.flag][forward.index])) {
                state = 2;
            }
            else if(isComparison(buffer[forward.flag][forward.index])) {
                state = 3;
            }
            else if(isLogic(buffer[forward.flag][forward.index])) {
                state = 4;
            }
            else if(isArithmetic(buffer[forward.flag][forward.index])) {
                state = 5;
            }
            else if(other(buffer[forward.flag][forward.index])) {
                state = 6;
            }
            else if(isSpace(buffer[forward.flag][forward.index])) {
                moveForward(lexemeBegin, false);
            }
            else {
                state = 7;
            }
            moveForward(forward, true);
        }
        //Reserved word / identifier
        else if (state == 1) {
            //look forward token end, return a id or reserved word
            if (!isCharacter(buffer[forward.flag][forward.index]) && !isUnderline(buffer[forward.flag][forward.index]) && !isNumber(buffer[forward.flag][forward.index])) {
                string temp = cut(lexemeBegin, forward);
                if (pointerDist(lexemeBegin, forward) > 64) {
                    temp = "Identifier Length limitation exceeds: " + temp;
                    Token token(dollar, temp);
                    tokenList.push_back(token);
                    state = 0;
                    lexemeBegin.index = forward.index;
                    lexemeBegin.flag = forward.flag;
                    continue;
                    // cout << "Length limitation exceeds: " << temp << endl;
                    // return -1;
                }
                string tempLower = temp;
                int len = tempLower.size();
                for (int i = 0; i < len; ++i)
                    tempLower[i] = tolower(tempLower[i]);
                if (isKeyword(tempLower)) {
                    int keywordID = keywordMap[tempLower];
                    Token token(keywordID, temp);
                    tokenList.push_back(token);
                }
                else {
                    Token token(_id, temp);
                    tokenList.push_back(token);
                }
                //reset
                lexemeBegin.index = forward.index;
                lexemeBegin.flag = forward.flag;
                state = 0;
            } else {
                //token not end, continue
                moveForward(forward, true);
            }
        }
        //number
        else if(state == 2) {
            if(!isNumber(buffer[forward.flag][forward.index]) && !isCharacter(buffer[forward.flag][forward.index])) {
                string temp = cut(lexemeBegin, forward);
                // If it is a substring start with number and include character, it's a illegal identifier
                int len = temp.size();
                bool valid = true;
                for (int i = 0; i < len; ++i) {
                    if (isCharacter(temp[i])) {
                        temp = "Invalid identifier:" + temp;
                        Token token(dollar, temp);
                        tokenList.push_back(token);
                        valid = false;
                        state = 0;
                        lexemeBegin.index = forward.index;
                        lexemeBegin.flag = forward.flag;
                        break;
                        // cout << "Invalid identifier:" << temp << endl;
                        // return -1;
                    }
                }
                if (!valid)
                    continue;
                if(!lessThanNumberLimit(temp)) {
                    temp = "Number limitation exceeds:" + temp;
                    Token token(dollar, temp);
                    tokenList.push_back(token);
                    state = 0;
                    lexemeBegin.index = forward.index;
                    lexemeBegin.flag = forward.flag;
                    continue;
                    // cout << "Number limitation exceeds: " << temp << endl;
                    // return -1;
                }
                Token token(num, temp);
                tokenList.push_back(token);
                lexemeBegin.index = forward.index;
                lexemeBegin.flag = forward.flag;
                state = 0;
            }
            else {
                moveForward(forward, true);
            }
        }
        //comparion / assignment
        else if(state == 3) {
            //dual char comparison symbol
            if(isComparison(buffer[forward.flag][forward.index]) && (buffer[forward.flag][forward.index] == '=' || buffer[forward.flag][forward.index] == '>')) {
                pointer forwardPeek = forward;
                moveForward(forwardPeek, false);
                string temp = cut(lexemeBegin, forwardPeek);
                if(buffer[forward.flag][forward.index] == '=') {
                    if(beginChar == '>') {
                        Token token(ge, temp);
                        tokenList.push_back(token);
                    }
                    else if(beginChar == '<') {
                        Token token(le, temp);
                        tokenList.push_back(token);
                    }
                    else if(beginChar == '=') {
                        Token token(eq_2, temp);
                        tokenList.push_back(token);
                    }
                    else {
                        temp = "Invalid comparison:" + temp;
                        Token token(dollar, temp);
                        tokenList.push_back(token);
                        state = 0;
                        lexemeBegin.index = forward.index;
                        lexemeBegin.flag = forward.flag;
                        continue;
                        // cout << "Invalid comparison: " << temp << endl;
                        // return -1;
                    }
                }
                else if(buffer[forward.flag][forward.index] == '>') {
                    if(beginChar == '<') {
                        Token token(ne, temp);
                        tokenList.push_back(token);
                    }
                    else {
                        temp = "Invalid comparison:" + temp;
                        Token token(dollar, temp);
                        tokenList.push_back(token);
                        state = 0;
                        lexemeBegin.index = forward.index;
                        lexemeBegin.flag = forward.flag;
                        continue;
                        // cout << "Invalid comparison: " << temp << endl;
                        // return -1;
                    }
                }
                else {
                    temp = "Invalid comparison: " + temp;
                    Token token(dollar, temp);
                    tokenList.push_back(token);
                    state = 0;
                    lexemeBegin.index = forward.index;
                    lexemeBegin.flag = forward.flag;
                    continue;
                    // cout << "Invalid comparison: " << temp << endl;
                    // return -1;
                }
                moveForward(forward, true);
                lexemeBegin.index = forward.index;
                lexemeBegin.flag = forward.flag;
                state = 0;
            }
            // single char comparison symbol
            else {
                string temp = cut(lexemeBegin, forward);
                // cout << temp << endl;
                if(beginChar == '>') {
                    Token token(gt, temp);
                    tokenList.push_back(token);
                }
                else if(beginChar == '<') {
                    Token token(lt, temp);
                    tokenList.push_back(token);
                }
                else if(beginChar == '=') {
                    Token token(eq_1, temp);
                    tokenList.push_back(token);
                }
                lexemeBegin.index = forward.index;
                lexemeBegin.flag = forward.flag;
                state = 0;
            }
        }
        //logic operator
        else if(state == 4) {
            if(isLogic(buffer[forward.flag][forward.index]) && beginChar != '!') {
                pointer forwardPeek = forward;
                moveForward(forwardPeek, false);
                string temp = cut(lexemeBegin, forwardPeek);
                if(buffer[forward.flag][forward.index] == '&' && beginChar == '&') {
                    Token token(_and, temp);
                    tokenList.push_back(token);
                }
                else if(buffer[forward.flag][forward.index] == '|' && beginChar == '|') {
                    Token token(_or, temp);
                    tokenList.push_back(token);
                }
                else {
                    temp = "Invalid logical operator:" + temp;
                    Token token(dollar, temp);
                    tokenList.push_back(token);
                    state = 0;
                    lexemeBegin.index = forward.index;
                    lexemeBegin.flag = forward.flag;
                    continue;
                    // cout << "Invalid logical operator: " << temp << endl;
                    // return -1;
                }
                moveForward(forward, true);
                lexemeBegin.index = forward.index;
                lexemeBegin.flag = forward.flag;
                state = 0;
            } else {
                string temp = cut(lexemeBegin, forward);
                if(beginChar == '!') {
                    Token token(_not, temp);
                    tokenList.push_back(token);
                }
                else {
                    temp = "Invalid logical operator:" + temp;
                    Token token(dollar, temp);
                    tokenList.push_back(token);
                    state = 0;
                    lexemeBegin.index = forward.index;
                    lexemeBegin.flag = forward.flag;
                    continue;
                    // cout << "Invalid logical operator: " << temp << endl;
                    // return -1;
                }
                lexemeBegin.index = forward.index;
                lexemeBegin.flag = forward.flag;
                state = 0;
            }
        }
        //arithmetic operator
        else if(state == 5) {
            string temp = cut(lexemeBegin, forward);
            if(beginChar == '+') {
                Token token(add, temp);
                tokenList.push_back(token);
            }
            else if(beginChar == '-') {
                Token token(sub, temp);
                tokenList.push_back(token);
            }
            else if(beginChar == '*') {
                Token token(star, temp);
                tokenList.push_back(token);
            }
            else if(beginChar == '/') {
                Token token(_div, temp);
                tokenList.push_back(token);
            }
            state = 0;
            lexemeBegin.index = forward.index;
            lexemeBegin.flag = forward.flag;
        }
        else if(state == 6) {
            string temp = cut(lexemeBegin, forward);
            if(beginChar == ',') {
                Token token(comma, temp);
                tokenList.push_back(token);
            }
            else if(beginChar == ';') {
                Token token(semicolon, temp);
                tokenList.push_back(token);
            }
            else if(beginChar == '(') {
                Token token(bracket_l, temp);
                tokenList.push_back(token);
            }
            else if(beginChar == ')') {
                Token token(bracket_r, temp);
                tokenList.push_back(token);
            }
            state = 0;
            lexemeBegin.index = forward.index;
            lexemeBegin.flag = forward.flag;
        }
        else if(state == 7) {
            string temp = cut(lexemeBegin, forward);
            Token token(dollar, "Illegal Character:" + temp);
            tokenList.push_back(token);
            state = 0;
            lexemeBegin.index = forward.index;
            lexemeBegin.flag = forward.flag;
        }
    }
}
