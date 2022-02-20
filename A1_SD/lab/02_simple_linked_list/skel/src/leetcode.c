
struct ListNode* deleteDuplicates(struct ListNode* head)
{
    struct ListNode *node,*aux;
    for(aux=head;aux!=NULL && aux->next!=NULL;aux=aux->next)
    {
        while(aux->next!=NULL && aux->val==aux->next->val)
        {
            node=aux->next;
            aux->next=aux->next->next;
            free(node);
        }
    }
    return head;
}

struct ListNode* mergeTwoLists(struct ListNode* l1, struct ListNode* l2)
{
    struct ListNode *head=NULL,*last,*aux;

    if(l1==l2 && l1==NULL)
        return head;
		
    head=last=malloc(sizeof(struct ListNode));
    last->next=NULL;

    if(l1!=NULL && (l2==NULL || l1->val<l2->val))
    {
        last->val=l1->val;
        l1=l1->next;
    }
    else
    {
        last->val=l2->val;
        l2=l2->next;
    }

    while(l1!=NULL && l2!=NULL)
    {
        last->next=malloc(sizeof(struct ListNode));
        last=last->next;
        last->next=NULL;

        if(l1->val<l2->val)
        {
            last->val=l1->val;
            l1=l1->next;
        }
        else
        {
            last->val=l2->val;
            l2=l2->next;
        }
    }

    for(aux=l1;aux!=NULL;aux=aux->next)
    {
        last->next=malloc(sizeof(struct ListNode));
        last=last->next;
        last->next=NULL;
        last->val=aux->val;
    }
    for(aux=l2;aux!=NULL;aux=aux->next)
    {
        last->next=malloc(sizeof(struct ListNode));
        last=last->next;
        last->next=NULL;
        last->val=aux->val;
    }

    return head;
}
