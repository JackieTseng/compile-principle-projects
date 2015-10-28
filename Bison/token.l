%{
    #include "calculator.tab.h"
%}

%%

"+"     { return ADD;   }
"-"     { return SUB;   }
"*"     { return MUL;   }
"/"     { return DIV;   }
"("     { return LEFT;  }
")"     { return RIGHT; }
"<"     { return LT;    }
">"     { return GT;    }
"<="    { return LE;    }
">="    { return GE;    }
"!="    { return NEQ;   }
"=="    { return EQ;    }
"?"     { return QU;    }
":"     { return CO;    }
"True"  | 
"TRUE"  | 
"true"  { return TRUE;  }
"False" | 
"FALSE" | 
"false" { return FALSE; }
"Or"    | 
"OR"    | 
"or"    { return OR;    }
"And"   | 
"AND"   | 
"and"   { return AND;   }
"Xor"   | 
"XOR"   | 
"xor"   { return XOR;   }
"Not"   | 
"NOT"   | 
"not"   { return NOT;   }
"ln"    { return LN;    }
"sqrt"  { return SQR;   }
"<<"    { return LS;    }
">>"    { return RS;    }
"^"     { return SQU;   }
"sin"   { return SIN;   }
"cos"   { return COS;   }
"tan"   { return TAN;   }
"!"     { return FAC;   }
"%"     { return MOD;   }
"log"   { return LOG;   }
([0-9]+[.]*[0-9]*|[0-9]+[.]*[0-9]+) { yylval.a = atof(yytext); return NUMBER; }
\n      { return EOL; }
[ \t]   { }
.       { printf("Valid Character %c\n", *yytext);}

%%
