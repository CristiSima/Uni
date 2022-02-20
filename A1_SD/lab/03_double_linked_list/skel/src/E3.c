/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */

struct ListNode* removeNthFromEnd(struct ListNode* head, int n)
{
	struct ListNode *node;
	if(head!=NULL)
    {
		node=removeNthFromEnd(head->next,n);
        if(node!=NULL && node->next==node)
		{
			// didn't reach the nth
			node->val++;
			if(node->val==n)
			{
				// reaches the nth
				free(node);
				node=head->next;
			}
		}
		else
		{
			// passed the nth
			if(head->next!=node)
				free(head->next);
			head->next=node;
			node=head;
		}
    }
	else
	{
		//reached the end
		node=malloc(sizeof(struct ListNode));
		node->next=node;
		node->val=0;
	}
    return node;
}
