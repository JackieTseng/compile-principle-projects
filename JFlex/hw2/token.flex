import java_cup.runtime.*;
class Yytoken {
    public int m_index;
    public String m_text;
  
    Yytoken (int index, String text) {
        m_index = index;
        m_text = text;
    }

    public String toString() {
        return "(Text: "+m_text+ "    index : "+m_index+")\n";
    }
}

%%

%{
    int counter = 0;
%}

%class Token
%unicode
%debug

FILENAME = <.*?>
NEWLINE = \r|\n|\r\n
FUNCTIONNAME = {ID}+[\s]*\(
ID = [A-Za-z_]+[A-Za-z0-9_]*
NUMBER = \d+[.]*\d*|\d+[.]*\d+ 
WHITE_SPACE_CHAR = [\n\r\ \t\b\012]
NONNEWLINE_WHITE_SPACE_CHAR = [\ \t\b\012]
STRING = \"(\\\"|[^\n\r\"]|\\{WHITE_SPACE_CHAR}+\\)*\"

%%

<YYINITIAL> {
  "#include" { return (new Yytoken(1, yytext())); }
  ","        { return (new Yytoken(6, yytext())); }
  ";"        { return (new Yytoken(7, yytext())); }
  "{"        { return (new Yytoken(8, yytext())); }
  "}"        { return (new Yytoken(9, yytext())); }
  "+"        { return (new Yytoken(10, yytext())); }
  "-"        { return (new Yytoken(11, yytext())); }
  "("        { return (new Yytoken(12, yytext())); }
  ")"        { if (counter == 0) {
                   return (new Yytoken(13, yytext())); 
               } else {
                   --counter;
               }
             }
  "*"        { return (new Yytoken(14, yytext())); }
  "/"        { return (new Yytoken(15, yytext())); }
  "<"        { return (new Yytoken(16, yytext())); }
  ">"        { return (new Yytoken(17, yytext())); }
  "="        { return (new Yytoken(18, yytext())); }
  "int"      { return (new Yytoken(19, yytext())); }
  "double"   { return (new Yytoken(20, yytext())); }
  "if"       { return (new Yytoken(21, yytext())); }
  "return"   { return (new Yytoken(22, yytext())); }
}

{FILENAME} { return (new Yytoken(2, yytext())); }

{ID} { return (new Yytoken(24, yytext())); }

{NUMBER} { return (new Yytoken(23, yytext())); }

{STRING} { return (new Yytoken(4, yytext())); }

{FUNCTIONNAME} {
    ++counter;
    String temp = "";
    for (int i = 0; i != yylength() - 1; i++) {
        temp += ((yycharat(i) == ' ') ? "" : yycharat(i));
    }
    switch(temp) {
        case "if" : yypushback(1); return (new Yytoken(21, temp));
        case "return" : yypushback(1); return (new Yytoken(22, temp));
        default : return (new Yytoken(3, temp + "()"));
    }
}

{NEWLINE} { }

{NONNEWLINE_WHITE_SPACE_CHAR}+ { }

. { }
