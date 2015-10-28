#ifndef BASIC_H
#define BASIC_H

#include <cstdio>
#include <cstring>
#include <string>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stack>
#include <queue>
#include <set>
#define BUFFER_SIZE 4096 /* the page size of the buffer pool */
using namespace std;

/* terminal token */
#define add 0
#define _and 1
#define bracket_l 2
#define bracket_r 3
#define comma 4
#define create 5
#define _default 6
#define _delete 7
#define _div 8
#define E 9
#define eq_1 10
#define eq_2 11
#define from 12
#define ge 13
#define gt 14
#define _id 15
#define _insert 16
#define _int 17
#define into 18
#define key 19
#define le 20
#define lt 21
#define ne 22
#define _not 23
#define num 24
#define _or 25
#define primary 26
#define _select 27
#define semicolon 28
#define star 29
#define sub 30
#define table 31
#define values 32
#define where 33
#define dollar 34

/* non-terminal token */
#define _bool 35
#define column_list 36
#define column_list_ 37
#define _comp 38
#define conjunct 39
#define conjunct_ 40
#define create_stmt 41
#define decl 42
#define decl_list 43
#define decl_list_ 44
#define default_spec 45
#define delete_stmt 46
#define disjunct 47
#define disjunct_ 48
#define expr 49
#define expr_ 50
#define insert_stmt 51
#define query_stmt 52
#define rop 53
#define select_list 54
#define simple_expr 55
#define simple_expr_ 56
#define simple_term 57
#define simple_term_ 58
#define simple_unary 59
#define ssql_stmt 60
#define term 61
#define term_ 62
#define unary 63
#define value_list 64
#define value_list_ 65
#define where_clause 66

#define unary_add 67
#define unary_sub 68

string MAX_NUM("4294967295");

/***
    Specific a exact location in the two page buffer
    flag:determine which buffer do the pointer point to
    index:determine the index of the pointer in its buffer page
***/
struct pointer {
    int flag, index;
    pointer() {
        flag = index = 0;
    }
    pointer(const pointer& p) {
        flag = p.flag;
        index = p.index;
    }
};

inline bool isCharacter(char unknown) {
    return (unknown >= 'a' && unknown <= 'z') || (unknown >= 'A' && unknown <= 'Z');
}

inline bool isUnderline(char unknown) {
    return unknown == '_';
}

inline bool isNumber(char unknown) {
    return unknown >= '0' && unknown <= '9';
}

inline bool lessThanNumberLimit(string number) {
    if (number.size() != MAX_NUM.size())
        return (number.size() < MAX_NUM.size());
    else
        return (number < MAX_NUM);
}

inline bool isComparison(char unknown) {
    return unknown == '>' || unknown == '<' || unknown == '=';
}

inline bool isLogic(char unknown) {
    return unknown == '|' || unknown == '!' || unknown == '&';
}

inline bool isArithmetic(char unknown) {
    return unknown == '+' || unknown == '-' || unknown == '*' || unknown == '/';
}

inline bool other(char unknown) {
    return unknown == ',' || unknown == ';' || unknown == '(' || unknown == ')';
}

inline bool isSpace(char unknown) {
    return unknown == ' ' || unknown == '\n' || unknown == '\t' || unknown == '\r';
}

string getString(int number) {
    stringstream ss;
    string str;
    ss << number;
    ss >> str;
    return str;
}

inline bool isRop(int temp) {
    return temp == gt || temp == ge || temp == eq_2 ||
           temp == lt || temp == le || temp == ne;
}

inline int getPriority(const int& op) {
    if(op == unary_add || op == unary_sub)
        return 6;
    if(op == star || op == _div)
        return 5;
    if(op == add || op == sub)
        return 4;
    if(isRop(op))
        return 3;
    return op == _not ? 2 : 1;
}

inline bool isLogicToken(int temp) {
    return temp == _not || temp == _and || temp == _or;
}

#endif
