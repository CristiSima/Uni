#include "Stack.h"
#include <stdio.h>
#include <string.h>


#define is_open(c) ((c=='(')||(c=='[')||(c=='{'))
#define FAIL()			\
do 						\
{						\
	st_free(st);		\
	return 0;			\
}						\
while(0)

int check_parentheses(char *str)
{
	stack_t *st=st_create(1);
	for(;str[0];str++)
	{
		if(is_open(str[0]))
		{
			st_push(st,str);
			continue;
		}

		if(str[0]==')')
		{
			if(((char*)st_peek(st))[0]!='(')
				FAIL();
			st_pop(st);
		}
		else if(str[0]==']')
		{
			if(((char*)st_peek(st))[0]!='[')
				FAIL();
			st_pop(st);
		}
		else if(str[0]=='}')
		{
			if(((char*)st_peek(st))[0]!='{')
				FAIL();
			st_pop(st);
		}
		else
		{
			// BAD
			FAIL();
		}
	}

	st_free(st);
	return 1;
}

#undef FAIL
#undef is_open

int main()
{
	char str[51]="{{(()(){})}}";
	printf("\"%s\":%d\n",str,check_parentheses(str));
	strcpy(str,"[[{(([}]))}]]");
	printf("\"%s\":%d\n",str,check_parentheses(str));
}
// gcc Parantheses.c Stack.c LinkedList.c -o Parantheses;./Parantheses
