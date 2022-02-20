#include "utils.h"
#include "CircularDoublyLinkedList.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ll_is_palindrome(doubly_linked_list_t* list)
{
	DIE(list==NULL,"got no list");
	DIE(list->head==NULL,"got empty list");

	dll_node_t *start,*end;
	start=list->head;
	end=start->prev;

	do
	{
		if(memcmp(start->data,end->data,1))
		{
			return 0;
		}
		start=start->next;
		end=end->prev;
	}
	while(start!=end && start->next!=end);

	return 1;
}

doubly_linked_list_t *gen_str_dll(char *str)
{
	doubly_linked_list_t *list=dll_create(2);
	char temp[2];
	temp[1]=0;
	for(;str[0]!=0;str++)
	{
		temp[0]=str[0];
		dll_add_nth_node(list,100,temp);
	}
	return list;
}

void test_gen_str_dll(char *str)
{
	doubly_linked_list_t *list=gen_str_dll(str);
	printf("\"%s\"\t.ll_is_palindrome:%d\n",str,ll_is_palindrome(list));
}

int main()
{
	// test_gen_str_dll("");
	test_gen_str_dll("a");
	test_gen_str_dll("aa");
	test_gen_str_dll("aba");
	test_gen_str_dll("abba");
	test_gen_str_dll("abaa");
	return 0;
}
// gcc E2.c CircularDoublyLinkedList.c -o E2;./E2
