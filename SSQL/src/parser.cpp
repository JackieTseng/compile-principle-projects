#include "parser.h"

Parser::Parser() {
    ip = 0;
    initPdt();
}

Parser::Parser(vector<Token> s) {
    ip = 0;
    sequence = s;
    initPdt();
}

void Parser::inputSequence(vector<Token> s) {
    sequence = s;
}

/* check if an statement match the grammar */
bool Parser::parsing() {
    /* look at the pseudo code in page 144 */
    int pt = 0;
    int X = ssql_stmt;
    stack<int> parser;
    parser.push(semicolon);
    parser.push(X);
    while(X != semicolon) {
        if(sequence[pt].getIndex() == dollar) {
            cout << "Grammar error occurs: " << sequence[pt].getValue() << endl;
            return false;
        }
        else if(X == sequence[pt].getIndex()) {
            parser.pop();
            ++pt;
        }
        else if(X >= 0 && X <= 33) {
            cout << "Grammar error occurs: should not be a terminal token(code: " << X << ")" << endl;
            return false;
        }
        else if(pdt[X][sequence[pt].getIndex()].size() == 0) {
            cout << "Grammar error occurs: no such statement(code: " << X << ")" << endl;
            return false;
        }
        else {
            parser.pop();
            if(pdt[X][sequence[pt].getIndex()][0] != E) {
                for(int i = pdt[X][sequence[pt].getIndex()].size() - 1; i >= 0; --i) {
                    parser.push(pdt[X][sequence[pt].getIndex()][i]);
                }
            }
        }
        X = parser.top();
    }
    return true;
}

/* initialize the predict analysis table */
void Parser::initPdt() {
    pdt[simple_expr][add].push_back(simple_term);
    pdt[simple_expr][add].push_back(simple_expr_);
    pdt[simple_expr][bracket_l].push_back(simple_term);
    pdt[simple_expr][bracket_l].push_back(simple_expr_);
    pdt[simple_expr][num].push_back(simple_term);
    pdt[simple_expr][num].push_back(simple_expr_);
    pdt[simple_expr][sub].push_back(simple_term);
    pdt[simple_expr][sub].push_back(simple_expr_);
    pdt[simple_expr_][add].push_back(add);
    pdt[simple_expr_][add].push_back(simple_term);
    pdt[simple_expr_][add].push_back(simple_expr_);
    pdt[simple_expr_][bracket_r].push_back(E);
    pdt[simple_expr_][comma].push_back(E);
    pdt[simple_expr_][sub].push_back(sub);
    pdt[simple_expr_][sub].push_back(simple_term);
    pdt[simple_expr_][sub].push_back(simple_expr_);
    pdt[simple_expr_][dollar].push_back(E);
    pdt[simple_term][add].push_back(simple_unary);
    pdt[simple_term][add].push_back(simple_term_);
    pdt[simple_term][bracket_l].push_back(simple_unary);
    pdt[simple_term][bracket_l].push_back(simple_term_);
    pdt[simple_term][num].push_back(simple_unary);
    pdt[simple_term][num].push_back(simple_term_);
    pdt[simple_term][sub].push_back(simple_unary);
    pdt[simple_term][sub].push_back(simple_term_);
    pdt[simple_term_][add].push_back(E);
    pdt[simple_term_][bracket_r].push_back(E);
    pdt[simple_term_][comma].push_back(E);
    pdt[simple_term_][_div].push_back(_div);
    pdt[simple_term_][_div].push_back(simple_unary);
    pdt[simple_term_][_div].push_back(simple_term_);
    pdt[simple_term_][star].push_back(star);
    pdt[simple_term_][star].push_back(simple_unary);
    pdt[simple_term_][star].push_back(simple_term_);
    pdt[simple_term_][sub].push_back(E);
    pdt[simple_term_][dollar].push_back(E);
    pdt[simple_unary][add].push_back(add);
    pdt[simple_unary][add].push_back(simple_unary);
    pdt[simple_unary][bracket_l].push_back(bracket_l);
    pdt[simple_unary][bracket_l].push_back(simple_expr);
    pdt[simple_unary][bracket_l].push_back(bracket_r);
    pdt[simple_unary][num].push_back(num);
    pdt[simple_unary][sub].push_back(sub);
    pdt[simple_unary][sub].push_back(simple_unary);
    pdt[_bool][add].push_back(_comp);
    pdt[_bool][bracket_l].push_back(bracket_l);
    pdt[_bool][bracket_l].push_back(disjunct);
    pdt[_bool][bracket_l].push_back(bracket_r);
    pdt[_bool][_id].push_back(_comp);
    pdt[_bool][_not].push_back(_not);
    pdt[_bool][_not].push_back(_bool);
    pdt[_bool][num].push_back(_comp);
    pdt[_bool][sub].push_back(_comp);
    pdt[_comp][add].push_back(expr);
    pdt[_comp][add].push_back(rop);
    pdt[_comp][add].push_back(expr);
    pdt[_comp][_id].push_back(expr);
    pdt[_comp][_id].push_back(rop);
    pdt[_comp][_id].push_back(expr);
    pdt[_comp][num].push_back(expr);
    pdt[_comp][num].push_back(rop);
    pdt[_comp][num].push_back(expr);
    pdt[_comp][sub].push_back(expr);
    pdt[_comp][sub].push_back(rop);
    pdt[_comp][sub].push_back(expr);
    pdt[conjunct][add].push_back(_bool);
    pdt[conjunct][add].push_back(conjunct_);
    pdt[conjunct][bracket_l].push_back(_bool);
    pdt[conjunct][bracket_l].push_back(conjunct_);
    pdt[conjunct][_id].push_back(_bool);
    pdt[conjunct][_id].push_back(conjunct_);
    pdt[conjunct][_not].push_back(_bool);
    pdt[conjunct][_not].push_back(conjunct_);
    pdt[conjunct][num].push_back(_bool);
    pdt[conjunct][num].push_back(conjunct_);
    pdt[conjunct][sub].push_back(_bool);
    pdt[conjunct][sub].push_back(conjunct_);
    pdt[conjunct_][_and].push_back(_and);
    pdt[conjunct_][_and].push_back(_bool);
    pdt[conjunct_][_and].push_back(conjunct_);
    pdt[conjunct_][bracket_r].push_back(E);
    pdt[conjunct_][_or].push_back(E);
    pdt[conjunct_][semicolon].push_back(E);
    pdt[conjunct_][dollar].push_back(E);
    pdt[disjunct][add].push_back(conjunct);
    pdt[disjunct][add].push_back(disjunct_);
    pdt[disjunct][bracket_l].push_back(conjunct);
    pdt[disjunct][bracket_l].push_back(disjunct_);
    pdt[disjunct][_id].push_back(conjunct);
    pdt[disjunct][_id].push_back(disjunct_);
    pdt[disjunct][_not].push_back(conjunct);
    pdt[disjunct][_not].push_back(disjunct_);
    pdt[disjunct][num].push_back(conjunct);
    pdt[disjunct][num].push_back(disjunct_);
    pdt[disjunct][sub].push_back(conjunct);
    pdt[disjunct][sub].push_back(disjunct_);
    pdt[disjunct_][bracket_r].push_back(E);
    pdt[disjunct_][_or].push_back(_or);
    pdt[disjunct_][_or].push_back(conjunct);
    pdt[disjunct_][_or].push_back(disjunct_);
    pdt[disjunct_][semicolon].push_back(E);
    pdt[disjunct_][dollar].push_back(E);
    pdt[expr][add].push_back(term);
    pdt[expr][add].push_back(expr_);
    pdt[expr][_id].push_back(term);
    pdt[expr][_id].push_back(expr_);
    pdt[expr][num].push_back(term);
    pdt[expr][num].push_back(expr_);
    pdt[expr][sub].push_back(term);
    pdt[expr][sub].push_back(expr_);
    pdt[expr_][add].push_back(add);
    pdt[expr_][add].push_back(term);
    pdt[expr_][add].push_back(expr_);
    pdt[expr_][_and].push_back(E);
    pdt[expr_][bracket_r].push_back(E);
    pdt[expr_][eq_2].push_back(E);
    pdt[expr_][ge].push_back(E);
    pdt[expr_][gt].push_back(E);
    pdt[expr_][le].push_back(E);
    pdt[expr_][lt].push_back(E);
    pdt[expr_][ne].push_back(E);
    pdt[expr_][_or].push_back(E);
    pdt[expr_][semicolon].push_back(E);
    pdt[expr_][sub].push_back(sub);
    pdt[expr_][sub].push_back(term);
    pdt[expr_][sub].push_back(expr_);
    pdt[expr_][dollar].push_back(E);
    pdt[rop][eq_2].push_back(eq_2);
    pdt[rop][ge].push_back(ge);
    pdt[rop][gt].push_back(gt);
    pdt[rop][le].push_back(le);
    pdt[rop][lt].push_back(lt);
    pdt[rop][ne].push_back(ne);
    pdt[term][add].push_back(unary);
    pdt[term][add].push_back(term_);
    pdt[term][_id].push_back(unary);
    pdt[term][_id].push_back(term_);
    pdt[term][num].push_back(unary);
    pdt[term][num].push_back(term_);
    pdt[term][sub].push_back(unary);
    pdt[term][sub].push_back(term_);
    pdt[term_][add].push_back(E);
    pdt[term_][_and].push_back(E);
    pdt[term_][bracket_r].push_back(E);
    pdt[term_][_div].push_back(_div);
    pdt[term_][_div].push_back(unary);
    pdt[term_][_div].push_back(term_);
    pdt[term_][eq_2].push_back(E);
    pdt[term_][ge].push_back(E);
    pdt[term_][gt].push_back(E);
    pdt[term_][le].push_back(E);
    pdt[term_][lt].push_back(E);
    pdt[term_][ne].push_back(E);
    pdt[term_][_or].push_back(E);
    pdt[term_][semicolon].push_back(E);
    pdt[term_][star].push_back(star);
    pdt[term_][star].push_back(unary);
    pdt[term_][star].push_back(term_);
    pdt[term_][sub].push_back(E);
    pdt[term_][dollar].push_back(E);
    pdt[unary][add].push_back(add);
    pdt[unary][add].push_back(unary);
    pdt[unary][_id].push_back(_id);
    pdt[unary][num].push_back(num);
    pdt[unary][sub].push_back(sub);
    pdt[unary][sub].push_back(unary);
    pdt[column_list][_id].push_back(_id);
    pdt[column_list][_id].push_back(column_list_);
    pdt[column_list_][bracket_r].push_back(E);
    pdt[column_list_][comma].push_back(comma);
    pdt[column_list_][comma].push_back(_id);
    pdt[column_list_][comma].push_back(column_list_);
    pdt[column_list_][from].push_back(E);
    pdt[create_stmt][create].push_back(create);
    pdt[create_stmt][create].push_back(table);
    pdt[create_stmt][create].push_back(_id);
    pdt[create_stmt][create].push_back(bracket_l);
    pdt[create_stmt][create].push_back(decl_list);
    pdt[create_stmt][create].push_back(bracket_r);
    pdt[create_stmt][create].push_back(semicolon);
    pdt[decl][_id].push_back(_id);
    pdt[decl][_id].push_back(_int);
    pdt[decl][_id].push_back(default_spec);
    pdt[decl][primary].push_back(primary);
    pdt[decl][primary].push_back(key);
    pdt[decl][primary].push_back(bracket_l);
    pdt[decl][primary].push_back(column_list);
    pdt[decl][primary].push_back(bracket_r);
    pdt[decl_list][_id].push_back(decl);
    pdt[decl_list][_id].push_back(decl_list_);
    pdt[decl_list][primary].push_back(decl);
    pdt[decl_list][primary].push_back(decl_list_);
    pdt[decl_list_][bracket_r].push_back(E);
    pdt[decl_list_][comma].push_back(comma);
    pdt[decl_list_][comma].push_back(decl);
    pdt[decl_list_][comma].push_back(decl_list_);
    pdt[default_spec][bracket_r].push_back(E);
    pdt[default_spec][comma].push_back(E);
    pdt[default_spec][_default].push_back(_default);
    pdt[default_spec][_default].push_back(eq_1);
    pdt[default_spec][_default].push_back(simple_expr);
    pdt[delete_stmt][_delete].push_back(_delete);
    pdt[delete_stmt][_delete].push_back(from);
    pdt[delete_stmt][_delete].push_back(_id);
    pdt[delete_stmt][_delete].push_back(where_clause);
    pdt[delete_stmt][_delete].push_back(semicolon);
    pdt[insert_stmt][_insert].push_back(_insert);
    pdt[insert_stmt][_insert].push_back(into);
    pdt[insert_stmt][_insert].push_back(_id);
    pdt[insert_stmt][_insert].push_back(bracket_l);
    pdt[insert_stmt][_insert].push_back(column_list);
    pdt[insert_stmt][_insert].push_back(bracket_r);
    pdt[insert_stmt][_insert].push_back(values);
    pdt[insert_stmt][_insert].push_back(bracket_l);
    pdt[insert_stmt][_insert].push_back(value_list);
    pdt[insert_stmt][_insert].push_back(bracket_r);
    pdt[insert_stmt][_insert].push_back(semicolon);
    pdt[query_stmt][_select].push_back(_select);
    pdt[query_stmt][_select].push_back(select_list);
    pdt[query_stmt][_select].push_back(from);
    pdt[query_stmt][_select].push_back(_id);
    pdt[query_stmt][_select].push_back(where_clause);
    pdt[query_stmt][_select].push_back(semicolon);
    pdt[select_list][_id].push_back(column_list);
    pdt[select_list][star].push_back(star);
    pdt[ssql_stmt][create].push_back(create_stmt);
    pdt[ssql_stmt][_delete].push_back(delete_stmt);
    pdt[ssql_stmt][_insert].push_back(insert_stmt);
    pdt[ssql_stmt][_select].push_back(query_stmt);
    pdt[value_list][add].push_back(simple_expr);
    pdt[value_list][add].push_back(value_list_);
    pdt[value_list][bracket_l].push_back(simple_expr);
    pdt[value_list][bracket_l].push_back(value_list_);
    pdt[value_list][num].push_back(simple_expr);
    pdt[value_list][num].push_back(value_list_);
    pdt[value_list][sub].push_back(simple_expr);
    pdt[value_list][sub].push_back(value_list_);
    pdt[value_list_][bracket_r].push_back(E);
    pdt[value_list_][comma].push_back(comma);
    pdt[value_list_][comma].push_back(simple_expr);
    pdt[value_list_][comma].push_back(value_list_);
    pdt[where_clause][semicolon].push_back(E);
    pdt[where_clause][where].push_back(where);
    pdt[where_clause][where].push_back(disjunct);
}
