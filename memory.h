#ifndef MEMORY_H_
#define MEMORY_H_

#include "./Boot/peripherals.h"
#include "./Boot/print.h"

//
//	page struct
//
typedef struct list_head_t
{
	struct list_head_t *next, *prev;

} list_head_t;

typedef struct page_t
{
	uint32_t vaddr;
	uint32_t flags;
	int32_t order;
	uint32_t counter;
	struct kmem_cache *cachep;
	list_head_t list;

} page_t;

#define OFFSET_OF(TYPE, MEMBER) ((unsigned int) &((TYPE *)0)->MEMBER)
#define CONTAINER_OF(ptr, type, member) ({ \
	const typeof( ((type *)0)->member ) *__mptr = (ptr); \
	(type *)( (char *)__mptr - OFFSET_OF(type,member) );})
#define LIST_ENTRY(ptr,type,member) CONTAINER_OF(ptr, type, member)
#define LIST_FOR_EACH(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

//
//	Dobulely Linked List Operator
//
void Init_List_Head(list_head_t *list);
void List_Add(list_head_t *new_1st, list_head_t *prev, list_head_t *next);
void Add_Front(list_head_t *new_1st, list_head_t *head);
void Add_Rear(list_head_t *new_1st, list_head_t *head);
void List_Del(list_head_t *prev, list_head_t *next);
void Del_Entry(list_head_t *entry);
void List_Remove_Chain(list_head_t *ch, list_head_t *ct);
void List_Add_Chain(list_head_t *ch, list_head_t *ct, list_head_t *head);
void List_Add_Chain_Tail(list_head_t *ch, list_head_t *ct, list_head_t *head);
uint16_t IsListEmpty(list_head_t *head);

//
//	Memory Boundary define STM32F4 spec
//
//	0x20000000 ~ 0x20003FFF : Golobal Data Ram(16K)
//	0x20004000 ~ 0x2001BFFF : Memory can Assign(96K)
//	0x2001C000 ~ 0x2001FFFF : Stack Ram(16K)
//
#define _MEM_END			0x2001C000
#define _MEM_START			0x20004000

//
//	Define one page is 1K
//
//	Page Size : 0x0000 0400
//	Page Mask : 0xFFFF FC00
//
#define PAGE_SHIFT			(10)
#define PAGE_SIZE			(1<<PAGE_SHIFT)
#define PAGE_MASK			(~(PAGE_SIZE-1))

//
//	Define Kernel Memory
//
//	Kernel Paging Memory -> Memory use to paging
//	 - Kernel Paging Start : 0x2000 4000
//	 - Kernel Paging End   : 0x2001 B400 
//	
//	Kernel page : Memory use to save page data
//	 - Kernel Page Num   : 93
//	 - Kernel Page Start : 0x2001 B414
//	 - Kernel Page End   : 0x2001 C000
//
#define KERNEL_MEM_END		(_MEM_END)
#define KERNEL_PAGING_START	((_MEM_START+(~PAGE_MASK))&((PAGE_MASK)))
#define	KERNEL_PAGING_END	(((KERNEL_MEM_END-KERNEL_PAGING_START) \
	/(PAGE_SIZE+sizeof(page_t))) \
	*(PAGE_SIZE)+KERNEL_PAGING_START)
#define KERNEL_PAGE_NUM		((KERNEL_PAGING_END-KERNEL_PAGING_START)/PAGE_SIZE)
#define KERNEL_PAGE_END		_MEM_END
#define KERNEL_PAGE_START	(KERNEL_PAGE_END-KERNEL_PAGE_NUM*sizeof(page_t))

//
//	Buddy Page Number -> page size from 1K to 32K
//	 - Average Page Number : 21
//	 - Page Number for Max : 31 (0x1F)
//
#define MAX_BUDDY_PAGE_NUM			(5)
#define AVERAGE_PAGE_NUM_PER_BUDDY	(KERNEL_PAGE_NUM/MAX_BUDDY_PAGE_NUM)
#define PAGE_NUM_FOR_MAX_BUDDY		((1<<MAX_BUDDY_PAGE_NUM)-1)

struct list_head_t page_buddy[MAX_BUDDY_PAGE_NUM];

//
//	Buddy Algorithm Function
//
void Init_Page_Buddy(void);
void Init_Page_Map(void);

//
//	Page Flag
//
#define PAGE_AVAILABLE				0x00
#define PAGE_DIRTY					0x01
#define PAGE_PROTECT				0x02
#define PAGE_BUDDY_BUSY				0x04
#define PAGE_IN_CACHE				0x08

#endif