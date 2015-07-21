cc = gcc
demo : calculator.y token.l
	bison -d calculator.y
	flex token.l
	cc -o demo calculator.tab.c lex.yy.c -lfl -lm

clean : 
	rm *.c
	rm *.h
	rm demo
	rm *~
