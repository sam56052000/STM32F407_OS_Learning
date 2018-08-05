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
	struct kmem_cache_t *cachep;
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
void Add_Head(list_head_t *new_1st, list_head_t *head);
void Add_Tail(list_head_t *new_1st, list_head_t *head);
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
//	0x20004000 ~ 0x20017FFF : Memory can Assign(80K)
//	0x20018000 ~ 0x20019FFF : Application RAM(8K)
//	0x2001A000 ~ 0x2001BFFF : Task Stack RAM(8K)
//	0x2001C000 ~ 0x2001FFFF : Stack Ram(16K)
//
#define _MEM_END			0x20018000
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
#define PAGE_NUM_FOR_MAX_BUDDY		((1<<(MAX_BUDDY_PAGE_NUM-1))-1)

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

#define BUDDY_END(x,order)			((x)+(1<<(order))-1)
#define NEXT_BUDDY_START(x,order)	((x)+(1<<(order)))
#define PREV_BUDDY_START(x,order)	((x)-(1<<(order)))

//
//	Buddy Assign Function
//
page_t* Get_Pages_from_List(int32_t order);
void Put_Pages_to_List(page_t *pg, int32_t order);
void *Page_Address(page_t *pg);
page_t *Alloc_Pages(uint32_t flag, int32_t order);
void Free_Pages(page_t *pg, int32_t order);
void *Get_Free_Pages(int32_t flag, int32_t order);
void Put_Free_Pages(void *addr, int32_t order);
page_t *Virt_to_Page(uint32_t addr);

//
//	Slab Define
//
#define KMEM_CACHE_DEFAULT_ORDER	(0)
#define KMEM_CACHE_MAX_ORDER		(3)
#define KMEM_CACHE_SAVE_RATE		(90)
#define KMEM_CACHE_PERCENT			(100)
#define KMEM_CACHE_MAX_WAST			(PAGE_SIZE-KMEM_CACHE_SAVE_RATE*PAGE_SIZE/KMEM_CACHE_PERCENT)

typedef struct kmem_cache_t
{
	uint32_t obj_size;
	uint32_t obj_nr;
	uint32_t page_order;
	uint32_t flags;
	page_t *head_page;
	page_t *end_page;
	void *nf_block;

} kmem_cache_t;

int32_t Find_Right_Order(int32_t size);
int32_t Kmem_Cache_Line_Object(void *head, int32_t size, int32_t order);
kmem_cache_t *Kmem_Cache_Create(kmem_cache_t *cache, int32_t size, uint32_t flags);
void Kmem_Cache_Destroy(kmem_cache_t *cache);
void Kmem_Cache_Free(kmem_cache_t *cache, void *objp);
void *Kmem_Cache_Alloc(kmem_cache_t *cache);

//
//	Kmalloc Function
//
#define KMALLOC_BIAS_SHIFT					(5)				//32byte minimal
#define KMALLOC_MAX_SIZE					(256)
#define KMALLOC_MINIMAL_SIZE_BIAS			(1<<(KMALLOC_BIAS_SHIFT))
#define KMALLOC_CACHE_SIZE					(KMALLOC_MAX_SIZE/KMALLOC_MINIMAL_SIZE_BIAS)
#define Kmalloc_Cache_Size_to_Index(size)	((((size))>>(KMALLOC_BIAS_SHIFT)))

kmem_cache_t kmalloc_cache[KMALLOC_CACHE_SIZE];

int32_t Kmalloc_Init(void);
void *Kmalloc(uint32_t size);
void Kfree(void *addr);

#endif