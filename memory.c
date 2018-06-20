#include "memory.h"

//
//	Buddy Algorithm
//
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
	page_t *tprev,*tnext = NULL;

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

//
//	slab Algorithm
//
int32_t Find_Right_Order(int32_t size)
{
	int order;

	for(order = 0; order <= KMEM_CACHE_MAX_ORDER; order++)
	{
		if( size <= (KMEM_CACHE_MAX_WAST)*(1<<order) )
		{
			return order;
		}
	}

	if( size > (1<<order) )
		return -1;

	return order;
}

int32_t Kmem_Cache_Line_Object(void *head, int32_t size, int32_t order)
{
	void **pl;
	char *p;
	pl = (void **)head;
	p = (char *)head + size;

	int32_t i, s = PAGE_SIZE * (1<<order);

	for(i = 0; s > size; i++, s -= size)
	{
		*pl = (void *)p;
		pl = (void **)p;
		p = p + size;
	}

	if(s == size)
		i++;

	// Need Add the Last slab to NULL

	return i;
}

kmem_cache_t *Kmem_Cache_Create(kmem_cache_t *cache, int32_t size, uint32_t flags)
{
	void **nf_block = &(cache->nf_block);

	int32_t order = Find_Right_Order(size);

	if(order == -1)
		return NULL;

	if( (cache->head_page = Alloc_Pages(0,order)) == NULL)
		return NULL;

	*nf_block = Page_Address(cache->head_page);

	cache->obj_nr = Kmem_Cache_Line_Object(*nf_block,size,order);
	cache->obj_size = size;
	cache->page_order = order;
	cache->flags = flags;
	cache->end_page = BUDDY_END(cache->head_page,order);
	cache->end_page->list.next = NULL;

	return cache;
}

void Kmem_Cache_Destroy(kmem_cache_t *cache)
{
	int32_t order = cache->page_order;
	page_t *pg = cache->head_page;
	list_head_t *list;

	while(1)
	{
		list = BUDDY_END(pg,order)->list.next;
		Free_Pages(pg,order);
		
		if(list)
		{
			pg = LIST_ENTRY(list, page_t, list);
		}
		else
		{
			return;
		}
	}
}

void Kmem_Cache_Free(kmem_cache_t *cache, void *objp)
{
	*(void **)objp = cache->nf_block;
	cache->nf_block = objp;
	cache->obj_nr++;
}

void *Kmem_Cache_Alloc(kmem_cache_t *cache)
{
	void *p;
	page_t *pg;

	if(cache == NULL)
		return NULL;

	void **nf_block = &(cache->nf_block);
	uint32_t *nr = &(cache->obj_nr);
	int32_t order = cache->page_order;

	if(!*nr)
	{
		if( (pg = Alloc_Pages(0,order)) == NULL)
			return NULL;

		*nf_block = Page_Address(pg);
		cache->end_page->list.next = &pg->list;
		cache->end_page = BUDDY_END(pg,order);
		cache->end_page->list.next = NULL;
		*nr += Kmem_Cache_Line_Object(*nf_block, cache->obj_size, order);
	}

	(*nr)--;
	p = *nf_block;
	*nf_block = *(void **)p;
	pg = Virt_to_Page((uint32_t)p);
	pg->cachep = cache;

	return p;
}

//
//	Memory Allocation Algorithm
//
int32_t Kmalloc_Init(void)
{
	//
	//	Kmalloc Struct Init
	//
	kmalloc_cache[0].obj_size = 0;
	kmalloc_cache[0].obj_nr = 0;
	kmalloc_cache[0].page_order = 0;
	kmalloc_cache[0].flags = 0;
	kmalloc_cache[0].head_page = NULL;
	kmalloc_cache[0].end_page = NULL;
	kmalloc_cache[0].nf_block = NULL;

	int32_t i=0;

	for(i = 0; i < KMALLOC_CACHE_SIZE; i++)
	{
		if(Kmem_Cache_Create(&kmalloc_cache[i], (i+1)*KMALLOC_MINIMAL_SIZE_BIAS,0) == NULL)
			return -1;
	}

	return 0;
}

void *Kmalloc(uint32_t size)
{
	int32_t index = Kmalloc_Cache_Size_to_Index(size);

	if(index >= KMALLOC_CACHE_SIZE)
		return NULL;

	return Kmem_Cache_Alloc(&kmalloc_cache[index]);
}

void Kfree(void *addr)
{
	page_t *pg;

	pg = Virt_to_Page((uint32_t)addr);

	Kmem_Cache_Free(pg->cachep, addr);
}
