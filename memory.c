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
	new_1st -> prev = prev;
	prev -> next = new_1st;
}

void Add_Head(list_head_t *new_1st, list_head_t *head)
{
	List_Add(new_1st, head, head -> next);
}

void Add_Tail(list_head_t *new_1st, list_head_t *head)
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
	List_Del(entry->prev, entry->next);
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
	uint32_t i, max_num = 0, min_num = 0;

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
				max_num++;
			}
			else
			{
				pg -> order = -1;
			}

			Add_Tail( &(pg->list), &page_buddy[MAX_BUDDY_PAGE_NUM - 1] );
		}
		//
		//	Not enough to merge into a max buddy is done as min buddy
		//
		else
		{
			pg -> order = 0;

			Add_Tail( &(pg->list), &page_buddy[0] );

			min_num++;
		}
	}

	//
	//	Print Init Memory Structure
	//
	printk("Allocation Range: %x to %x\n", KERNEL_PAGING_START, KERNEL_PAGING_END);
	printk("Total Page Number : %d\n", KERNEL_PAGE_NUM);
	printk("Init Page Number :");
	printk("%dK(%d), 1K(%d)\n", 1<<(MAX_BUDDY_PAGE_NUM-1), max_num, min_num);
}

page_t* Get_Pages_from_List(int32_t order)
{
	int32_t neworder = order;
	page_t *pg;
	list_head_t *tlst,*tlst1;

	for( ; neworder < MAX_BUDDY_PAGE_NUM; neworder++)
	{
		if( IsListEmpty(&page_buddy[neworder]) )
		{
			continue;
		}
		else
		{
			pg = LIST_ENTRY(page_buddy[neworder].next, page_t,list);
			tlst = &(BUDDY_END(pg,neworder)->list);
			tlst->next->prev = &page_buddy[neworder];
			page_buddy[neworder].next = tlst->next;

			goto OUT_OK;
		}
	}

	return 0;
	
OUT_OK:

	for(neworder--; neworder >= order; neworder--)
	{
		tlst1 = &(BUDDY_END(pg,neworder)->list);

		tlst = &(pg->list);

		pg = NEXT_BUDDY_START(pg, neworder);

		LIST_ENTRY(tlst, page_t, list)->order = neworder;

		List_Add_Chain_Tail(tlst, tlst1, &page_buddy[neworder]);
	}

	pg->flags |= PAGE_BUDDY_BUSY;
	pg->order = order;

	return pg;
}

void Put_Pages_to_List(page_t *pg, int32_t order)
{
	page_t *tprev,*tnext;

	if( !(pg->flags & PAGE_BUDDY_BUSY) )
	{
		printk("Warning! Force the Allocate Memory Area\n");
		return;
	}

	pg->flags &= ~(PAGE_BUDDY_BUSY);

	for( ; order < MAX_BUDDY_PAGE_NUM-1; order++)
	{
		tnext = NEXT_BUDDY_START(pg, order);
		tprev = PREV_BUDDY_START(pg, order);

		if( (!(tnext->flags & PAGE_BUDDY_BUSY)) && (tnext->order == order) )
		{
			pg->order++;
			tnext->order = -1;
			List_Remove_Chain(&(tnext->list), &(BUDDY_END(tnext, order)->list));
			BUDDY_END(pg, order)->list.next = &(tnext->list);
			tnext->list.prev = &(BUDDY_END(pg, order)->list);

			continue;
		}
		else if( (!(tprev->flags & PAGE_BUDDY_BUSY)) && (tprev->order == order) )
		{
			pg->order=-1;
			
			List_Remove_Chain(&(pg->list), &(BUDDY_END(pg, order)->list));
			BUDDY_END(tprev, order)->list.next = &(pg->list);
			pg->list.prev = &(BUDDY_END(tprev, order)->list);
			
			pg = tprev;
			pg->order++;

			continue;
		}
		else
		{
			break;
		}
	}

	List_Add_Chain(&(pg->list), &((tnext-1)->list), &page_buddy[order]);
}

void *Page_Address(page_t *pg)
{
	return (void *)(pg->vaddr);
}

page_t *Alloc_Pages(uint32_t flag, int32_t order)
{
	page_t *pg;
	int32_t i;

	pg = Get_Pages_from_List(order);

	if(pg == NULL)
		return NULL;

	for(i = 0; i < (1 << order); i++)
	{
		(pg+i)->flags |= (PAGE_DIRTY | flag);
	}

	return pg;
}

void Free_Pages(page_t *pg, int32_t order)
{
	int32_t i;

	for(i = 0; i < (1<<order); i++)
	{
		(pg+i)->flags &= ~PAGE_DIRTY;
	}

	Put_Pages_to_List(pg, order);
}

page_t *Virt_to_Page(uint32_t addr)
{
	uint32_t i;

	i = ((addr) - KERNEL_PAGING_START) >> PAGE_SHIFT;

	if(i > KERNEL_PAGE_NUM)
		return NULL;

	return (page_t *)KERNEL_PAGE_START+i;
}

void *Get_Free_Pages(int32_t flag, int32_t order)
{
	page_t *page;

	page = Alloc_Pages(flag, order);

	if(!page)
		return NULL;

	return	Page_Address(page);
}

void Put_Free_Pages(void *addr, int32_t order)
{
	Free_Pages(Virt_to_Page((uint32_t)addr), order);
}
