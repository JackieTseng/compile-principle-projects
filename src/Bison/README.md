Bison
=====

Bison project - Compiler Principle Project

Description
------
Using Bison and LLR(1) implement the simple calculator

##Usage
1. Compile

  `bison -d demo.y`<br>
  `flex demo.l`<br>
  `gcc -o demo demo.tab.c lex.yy.c -lfl -lm`<br>

  or

  `make` (Linux)

2. Run
  `./demo` or `./demo test.in`

###BISON HOMEWORK:
1. 实现基本功能
2. 附加功能有: 
* 对数函数ln,log;          格式:ln number   | log number
* 三角函数sin, cos, tan;   格式:sin number
* 开根号sqrt, 乘方^;       格式:sqrt number | number ^ number
* 左移<<, 右移>>;          格式:number << number
* 阶乘!, 求模%;            格式:number!     | number % number
* 可进行浮点数计算
