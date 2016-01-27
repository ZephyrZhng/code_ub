%{
	#include <stdio.h>
%}

%%  
[0-9]+        { printf("number "); }
[a-zA-Z]+     { printf("word ");   }
[ \t]         ; //忽略空格
%%