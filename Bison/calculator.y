%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <math.h>
    #define VALID "Valid Operation"
    struct node {
        int isNumber;
        double num;
    };
    int counter = 0;
    struct node* list[10000];

    struct node* nodeCal(int a, double b) {
        struct node *n = malloc(sizeof(struct node));
        n->isNumber = a;
        n->num = b;
        list[counter++] = n;
        return n;
    }
    
    struct node* nodeCalLogic(int a, int b, int isNumber, int type) {
        struct node* n = malloc(sizeof(struct node));    
        n->isNumber = isNumber;
        switch (type) {
            case 1 : n->num = (a & b); break;
            case 2 : n->num = (a | b); break;
            case 3 : n->num = (a ^ b); break;
            case 4 : if (isNumber) n->num = (~a); else n->num = a ? 0 : 1; break;
        }
        list[counter++] = n;
        return n;
    }

    int isSame(struct node* a, struct node* b) {
        return a->isNumber == b->isNumber;
    }
    
    int isAllNum(struct node* a, struct node* b) {
        return a->isNumber && b->isNumber;
    }

    struct node* nodeStructM(struct node* a, struct node* b, int symbol) {
        if (!isAllNum(a, b)) { 
            yyerror(VALID); 
        }
        switch (symbol) {
            case 1 : return nodeCal(1, a->num + b->num);
            case 2 : return nodeCal(1, (0 - a->num));
            case 3 : return nodeCal(1, a->num - b->num);
            case 4 : return nodeCal(1, a->num * b->num);
            case 5 : if (b->num) {
                         return nodeCal(1, a->num / b->num);
                     } else {
                         yyerror(VALID);
                         return nodeCal(1, -1);
                     }
            case 6 : if (b->num > 0) {
                         return nodeCal(1, (int)log(a->num)); 
                     } else {
                         yyerror(VALID);
                         return nodeCal(1, -1);
                     }
            case 7 : return nodeCal(1, (int)a->num << (int)b->num);
            case 8 : return nodeCal(1, (int)a->num >> (int)b->num);
            case 9 : return nodeCal(1, sqrt(a->num));
            case 10: return nodeCal(1, pow(a->num, b->num));
            case 11: return nodeCal(1, sin(a->num));
            case 12: return nodeCal(1, cos(a->num));
            case 13: return nodeCal(1, sin(a->num) / cos(a->num));
            case 14: if (a->num >= 0) {
                         int sum = 1, i;
                         for (i = 1; i <= a->num; i++) {
                             sum *= i;
                         }
                         return nodeCal(1, sum);
                     } else {
                         yyerror(VALID);
                         return nodeCal(1, -1);
                     }
            case 15: if (b->num) {
                         return nodeCal(1, (int)a->num % (int)b->num);
                     } else {
                         yyerror(VALID);
                         return nodeCal(1, -1);
                     }
            case 16: if (b->num > 0) {
                         return nodeCal(1, (int)log10(a->num));
                     } else {
                         yyerror(VALID);
                         return nodeCal(1, -1);
                     }
        }
    }

    struct node* nodeStructR(struct node* a, struct node* b, int symbol) {
        if (isSame(a, b)) {
            switch (symbol) {
                case 1 : return nodeCal(0, (a->num - b->num <  0 ? 1 : 0)); break;
                case 2 : return nodeCal(0, (a->num - b->num >  0 ? 1 : 0)); break;
                case 3 : return nodeCal(0, (a->num - b->num <= 0 ? 1 : 0)); break;
                case 4 : return nodeCal(0, (a->num - b->num >= 0 ? 1 : 0)); break;
                case 5 : return nodeCal(0, (a->num - b->num != 0 ? 1 : 0)); break;
                case 6 : return nodeCal(0, (a->num - b->num == 0 ? 1 : 0)); break;
            }
        } else {
            yyerror(VALID); 
            return nodeCal(0, -1);
        }
    }

    struct node* nodeStructL(struct node* a, struct node* b, int symbol) {
        if (isSame(a, b)) {
            return nodeCalLogic(a->num, b->num, a->isNumber, symbol);
        } else { 
            yyerror(VALID); 
            return nodeCal(0, -1);
        } 
    }

    void setFree() {
        while (counter >= 0) {
            free(list[counter--]);
        }
    }
    
    int Error = 0;
%}

%union {
    struct node *n;
    double a; 
}

%type <n> exp

%token TRUE FALSE
%token <a> NUMBER
%right QU CO
%left AND OR XOR
%left NEQ EQ
%left LT GT LE GE
%left LS RS
%left ADD SUB
%left MUL DIV LN SQU SQR SIN COS TAN FAC MOD LOG
%right NOT
%nonassoc UMINUS
%token LEFT RIGHT
%token EOL

%expect 29

%%

calclist:
    | calclist exp EOL {
        if (!Error) {
            if ($2->isNumber) {
                int a = (int)$2->num;
                if (a == $2->num) {
                    printf("%d\n",a);
                } else {
                    printf("%.3lf\n", $2->num);
                }
            } else {
                printf($2->num ? "True\n" : "False\n");
            }
        }
        Error = 0;
    }
    ;

exp: { $$ = nodeCal(1, 0); }
    | exp ADD exp { $$ = nodeStructM($1, $3, 1);  }
    | SUB exp %prec UMINUS 
                  { $$ = nodeStructM($2, $2, 2);  }
    | exp SUB exp { $$ = nodeStructM($1, $3, 3);  }
    | exp MUL exp { $$ = nodeStructM($1, $3, 4);  }
    | exp DIV exp { $$ = nodeStructM($1, $3, 5);  }
    | LN  exp     { $$ = nodeStructM($2, $2, 6);  }
    | exp LS  exp { $$ = nodeStructM($1, $3, 7);  }
    | exp RS  exp { $$ = nodeStructM($1, $3, 8);  }
    | SQR exp     { $$ = nodeStructM($2, $2, 9);  }
    | exp SQU exp { $$ = nodeStructM($1, $3, 10); }
    | SIN exp     { $$ = nodeStructM($2, $2, 11); }
    | COS exp     { $$ = nodeStructM($2, $2, 12); }
    | TAN exp     { $$ = nodeStructM($2, $2, 13); }
    | exp FAC     { $$ = nodeStructM($1, $1, 14); }
    | exp MOD exp { $$ = nodeStructM($1, $3, 15); }
    | LOG exp     { $$ = nodeStructM($2, $2, 16); }
    | exp LT  exp { $$ = nodeStructR($1, $3, 1);  }
    | exp GT  exp { $$ = nodeStructR($1, $3, 2);  }
    | exp LE  exp { $$ = nodeStructR($1, $3, 3);  }
    | exp GE  exp { $$ = nodeStructR($1, $3, 4);  }
    | exp NEQ exp { $$ = nodeStructR($1, $3, 5);  }
    | exp EQ  exp { $$ = nodeStructR($1, $3, 6);  }
    | exp AND exp { $$ = nodeStructL($1, $3, 1);  }
    | exp OR  exp { $$ = nodeStructL($1, $3, 2);  }
    | exp XOR exp { $$ = nodeStructL($1, $3, 3);  }
    | NOT exp     { $$ = nodeStructL($2, $2, 4);  }
    | exp QU exp CO exp { if ($1->num) $$ = nodeCal($3->isNumber, $5->num); 
                          else $$ = nodeCal($5->isNumber, $5->num); }  
    | TRUE   { $$ = nodeCal(0, 1);  }
    | FALSE  { $$ = nodeCal(0, 0);  }
    | NUMBER { $$ = nodeCal(1, $1); }
    | LEFT exp RIGHT { $$ = $2; }
    ;

%%

void main(int argc, const char *argv[]) {
    if (argc > 1) { 
        FILE* f = fopen(argv[1], "r");
        if (!f) {
            perror(argv[1]);
            return;
        }
        yyrestart(f);
    }
    yyparse();
    setFree();
    return;
}

yyerror(char *s) {
    fprintf(stderr, "error: %s\n", s);
    Error = 1;
}
