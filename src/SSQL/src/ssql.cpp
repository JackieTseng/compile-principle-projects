#include "lexer.cpp"
#include "parser.cpp"
#include "interpreter.cpp"

int main(int argc, char* argv[]) {
    /* if no input file */
    if (argc != 2) {
        printf("Argument error\n");
        return -1;
    } else {
        /* interpret the corresponding input file */
        char* fileCStr = argv[1];
        string fileStr(fileCStr);
        Lexer lexer(fileStr);
        Interpreter interpreter(lexer.getTokenList());
        interpreter.interpret();
    }
    return 0;
}
