import java.io.*;

class Yytoken {
    Yytoken () {
    }
}
%%

%class Lexer
%debug

ALPHA = [A-Za-z]
DIGIT = [0-9]
CHAR = " "|"-"
ISBN_SHORT = \d{1,5}{CHAR}\d{1,7}{CHAR}\d{1,6}{CHAR}(\d|'X') 
ISBN_LONG  = (97(8|9)){CHAR}{ISBN_SHORT}
OTHER = ({ALPHA}|{DIGIT}|CHAR)+
NEWLINE = \n|\r|\n\r

%%

{ISBN_SHORT} {
    String str = yytext();
    int len = yylength();
    int count = 0, S = 0;
    for (int i = 0, flag = 10; i < len - 1; i++) {
        if (Character.isDigit(str.charAt(i))) {
            count++;
            S += Integer.parseInt(String.valueOf(str.charAt(i))) * (flag--);
        }
    }
    if (count != 9) {
        try {
            FileWriter w = new FileWriter("output.txt", true);
            w.write("Error\n");
            w.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    } else {
        int M = S % 11;
        int C = (11 - M) % 11;
        if ((C == 10 && str.charAt(len - 1) == 'X') || 
            (C == Integer.parseInt(String.valueOf(str.charAt(len - 1))))) {
            int index_1 = 0, index_2 = 0;
            for (int j = 0, flag = 0; j < len; j++) {
                if (str.charAt(j) == ' ' || str.charAt(j) == '-') {
                    if (flag == 0) {
                        index_1 = j;
                        flag++;
                    } else {
                        index_2 = j;
                        break;
                    }
                }
            }
            int group = Integer.parseInt(str.substring(0, index_1));
            int publish = Integer.parseInt(str.substring(index_1 + 1, index_2));
            String Area;
            switch (group) {
                case 0  : Area = "English";;break;
                case 7  : Area = "Chinese";break;
                default : Area = "Other";break;
            }
            try {
                FileWriter w = new FileWriter("output.txt", true);
                w.write(Area + " " + publish + "\n");
                w.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        } else {
            try {
                FileWriter w = new FileWriter("output.txt", true);
                w.write("Error\n");
                w.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}

{NEWLINE} { }

{ISBN_LONG} {
    String str = yytext();
    int len = yylength();
    int count = 0, S = 0;
    for (int i = 0, flag = 0; i < len - 1; i++) {
        if (Character.isDigit(str.charAt(i))) {
            count++;
            S += Integer.parseInt(String.valueOf(str.charAt(i))) * (((flag++) % 2 == 0) ? 1 : 3);
        }
    }
    if (count != 12) {
        try {
            FileWriter w = new FileWriter("output.txt", true);
            w.write("Error\n");
            w.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    } else {
        int M = S % 10;
        int C = (10 - M) % 10;
        if (C == Integer.parseInt(String.valueOf(str.charAt(len - 1)))) {
            int index_1 = 0, index_2 = 0, index_3 = 0;
            for (int j = 0, flag = 0; j < len; j++) {
                if (str.charAt(j) == ' ' || str.charAt(j) == '-') {
                    if (flag == 0) {
                        index_1 = j;
                        flag++;
                    } else if (flag == 1) {
                        index_2 = j;
                        flag++;
                    } else {
                        index_3 = j;
                        break;
                    }
                }
            }
            int group = Integer.parseInt(str.substring(index_1 + 1, index_2));
            int publish = Integer.parseInt(str.substring(index_2 + 1, index_3));
            String Area;
            switch (group) {
                case 0  : Area = "English";;break;
                case 7  : Area = "Chinese";break;
                default : Area = "Other";break;
            }
            try {
                FileWriter w = new FileWriter("output.txt", true);
                w.write(Area + " " + publish + "\n");
                w.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        } else {    
            try {
                FileWriter w = new FileWriter("output.txt", true);
                w.write("Error\n");
                w.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}

{OTHER} {
    try {
        FileWriter w = new FileWriter("output.txt", true);
        w.write("Error\n");
        w.close();
    } catch (IOException e) {
        e.printStackTrace();
    }
}

. { }
