#include "memory.h"

void Init_List_Head(list_head_t *list)
{
	list -> next = list;
	list -> prev = list;
}

void List_Add(list_head_t *new_1st, list_head_t *prev, list_head_t *next)
{
	next -> prev = new_1st;
	new_1st -> next = next;
	prev -> next = new_1st;
	new_1st -> prev = prev;
}

void Add_Front(list_head_t *new_1st, list_head_t *head)
{
	List_Add(new_1st, head, head -> next);
}

void Add_Rear(list_head_t *new_1st, list_head_t *head)
{
	List_Add(new_1st, head -> prev, head);
}
void List_Del(list_head_t *prev, list_head_t *next)
{
	next->prev = prev;
	prev->next = next;
}

void Del_Entry(list_head_t *entry)
{
	List_Del( entry->prev, entry->next);
}

void List_Remove_Chain(list_head_t *chain_head, list_head_t *chain_tail)
{
	chain_head->prev->next = chain_tail -> next;
	chain_tail->next->prev = chain_head -> prev;
}

void List_Add_Chain(list_head_t *chain_head, list_head_t *chain_tail, list_head_t *head)
{
	chain_head->prev=head;
	chain_tail->next=head->next;
	head->next->prev=chain_tail;
	head->next=chain_head;
}

void List_Add_Chain_Tail(list_head_t *chain_head, list_head_t *chain_tail, list_head_t *head)
{
	chain_head->prev=head->prev;
	head->prev->next=chain_head;
	head->prev=chain_tail;
	chain_tail->next=head;
}

uint16_t IsListEmpty(list_head_t *head)
{
	return head->next == head;
}

void Init_Page_Buddy(void)
{
	uint32_t i;

	for(i = 0; i < MAX_BUDDY_PAGE_NUM; i++)
	{
		Init_List_Head(&page_buddy[i]);
	}
}

void Init_Page_Map(void)
{
	uint32_t i;

	page_t *pg = (page_t *)KERNEL_PAGE_START;

	Init_Page_Buddy();

	for(i = 0; i < (KERNEL_PAGE_NUM); pg++, i++)
	{
		//
		//	Fill Struct Page
		//
		pg -> vaddr = KERNEL_PAGING_START + i * PAGE_SIZE;
		pg -> flags = PAGE_AVAILABLE;
		pg -> counter = 0;
		Init_List_Head( &(pg->list) );

		//
		//	Memory for Max Buddy is Possible
		//
		if( i < (KERNEL_PAGE_NUM & (~PAGE_NUM_FOR_MAX_BUDDY) ) )
		{
			//
			//	Assign Max Buddy Order
			//	 - First -> Order
			//	 - Other -> -1
			//
			if( (i & PAGE_NUM_FOR_MAX_BUDDY) == 0)
			{
				pg -> order = MAX_BUDDY_PAGE_NUM - 1;
			}
			else
			{
				pg -> order = -1;
			}

			List_Add_Tail( &(pg->list), &page_buddy[MAX_BUDDY_PAGE_NUM - 1] );
		}
		//
		//	Not enough to merge into a max buddy is done as min buddy
		//
		else
		{
			pg -> order = 0;

			List_Add_Tail( &(pg->list), &page_buddy[0] );
		}
	}
}