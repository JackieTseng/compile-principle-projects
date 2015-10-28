#ifndef TOKEN_CPP
#define TOKEN_CPP
#include "token.h"

Token::Token() {
}

Token::Token(int ind, string val) {
    index = ind;
    value = val;
}

int Token::getIndex() {
    return index;
}

int Token::getIntValue() {
    return atoi(value.c_str());
}

string Token::getValue() {
    return value;
}

void Token::setIndex(int ind) {
    index = ind;
}

void Token::setValue(string val) {
    value = val;
}

/* compare two numbers and return the result */
bool compNums(Token a, Token b, int op) {
    if(op == eq_2) {
        return a.getValue() == b.getValue();
    }
    else if(op == ne) {
        return !compNums(a, b, eq_2);
    }
    else if(op == lt) {
        return a.getIntValue() < b.getIntValue();
    }
    else if(op == le) {
        return compNums(a, b, eq_2) || compNums(a, b, lt);
    }
    else if(op == gt) {
        return !compNums(a, b, le);
    }
    else if(op == ge) {
        return !compNums(a, b, lt);
    }
}

/* evaluate the operator with two or one given tokens */
Token evaluate(Token a, Token b, int op) {
    if(op == add) {
        return Token(num, getString(b.getIntValue() + a.getIntValue()));
    }
    else if(op == sub) {
        return Token(num, getString(b.getIntValue() - a.getIntValue()));
    }
    else if(op == star) {
        return Token(num, getString(b.getIntValue() * a.getIntValue()));
    }
    else if(op == _div) {
        if(a.getIntValue() == 0) {
            return Token(dollar, "");
        }
        else {
            return Token(num, getString(b.getIntValue() / a.getIntValue()));
        }
    }
    else if(op == _not) {
        return Token(_bool, getString(!b.getIntValue()));
    }
    else if(op == _and) {
        return Token(_bool, getString(a.getIntValue() && b.getIntValue()));
    }
    else if(op == _or) {
        return Token(_bool, getString(a.getIntValue() || b.getIntValue()));
    }
    else if(op == unary_sub) {
        return Token(num, getString(-b.getIntValue()));
    }
    else if(op == unary_add) {
        return Token(num, getString(b.getIntValue()));
    }
    else {
        return compNums(b, a, op) ? Token(_bool, getString(1)) : Token(_bool, getString(0));
    }
}

Token exprEvaluate(vector<Token> tokens) {
    vector<Token> round1;
    /* round 1: check which tokens are unary add or unary sub */
    for(int i = 0; i < tokens.size(); ++i) {
        if(tokens[i].getIndex() == sub) {
            if(round1.size() == 0 || (round1.back().getIndex() != num && round1.back().getIndex() != bracket_r)) {
                round1.push_back(Token(unary_sub, "-"));
            }
            else {
                round1.push_back(tokens[i]);
            }
        }
        else if(tokens[i].getIndex() == add) {
            if(round1.size() == 0 || (round1.back().getIndex() != num && round1.back().getIndex() != bracket_r)) {
                round1.push_back(Token(unary_add, "+"));
            }
            else {
                round1.push_back(tokens[i]);
            }
        }
        else {
            round1.push_back(tokens[i]);
        }
    }
    vector<Token> postfixExpr;
    stack<Token> temp;
    /* transform the expression to postfix form */
    for(int i = 0; i < round1.size(); ++i) {
        if(round1[i].getIndex() == num) {
            postfixExpr.push_back(round1[i]);
        }
        else if(round1[i].getIndex() == bracket_l || temp.empty()) {
            temp.push(round1[i]);
        }
        else if(round1[i].getIndex() == bracket_r) {
            while(!temp.empty()) {
                if(temp.top().getIndex() == bracket_l) {
                    temp.pop();
                    break;
                }
                postfixExpr.push_back(temp.top());
                temp.pop();
            }
        }
        else {
            bool flag = false;
            while(!temp.empty()) {
                if(temp.top().getIndex() == bracket_l || getPriority(round1[i].getIndex()) > getPriority(temp.top().getIndex())) {
                    temp.push(round1[i]);
                    flag = true;
                    break;
                }
                else {
                    postfixExpr.push_back(temp.top());
                    temp.pop();
                }
            }
            if(!flag) {
                temp.push(round1[i]);
            }
        }
    }
    while(!temp.empty()) {
        postfixExpr.push_back(temp.top());
        temp.pop();
    }
    Token num1, num2, result;
    /* evaluate the postfix expression using a stack */
    for(int i = 0; i < postfixExpr.size(); ++i) {
        if(postfixExpr[i].getIndex() == num || postfixExpr[i].getIndex() == _bool) {
            temp.push(postfixExpr[i]);
        }
        else if(isLogicToken(postfixExpr[i].getIndex())) {
            num1 = temp.top();
            temp.pop();
            if(postfixExpr[i].getIndex() == _not) {
                result = evaluate(Token(dollar, ""), num1, _not);
            }
            else {
                num2 = temp.top();
                temp.pop();
                result = evaluate(num1, num2, postfixExpr[i].getIndex());
            }
            temp.push(result);
        }
        else if(postfixExpr[i].getIndex() == unary_add || postfixExpr[i].getIndex() == unary_sub) {
            num1 = temp.top();
            temp.pop();
            result = evaluate(Token(dollar, ""), num1, postfixExpr[i].getIndex());
            temp.push(result);
        }
        else {
            num1 = temp.top();
            temp.pop();
            num2 = temp.top();
            temp.pop();
            result = evaluate(num1, num2, postfixExpr[i].getIndex());
            if(result.getIndex() == dollar) {
                return result;
            }
            temp.push(result);
        }
    }
    return temp.top();
}

#endif
