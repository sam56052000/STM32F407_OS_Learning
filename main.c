#include "main.h"

int main(void)
{	
	LED_ON(PIN_LED5);
	LED_ON(PIN_LED6);

	uint32_t a = KERNEL_PAGE_NUM;
	uint32_t b = PAGE_NUM_FOR_MAX_BUDDY;
	uint32_t c = (KERNEL_PAGE_NUM & (~PAGE_NUM_FOR_MAX_BUDDY) );

	uint32_t paging_start = KERNEL_PAGING_START;
	uint32_t paging_end = KERNEL_PAGING_END;
	uint32_t page_start = KERNEL_PAGE_START;
	uint32_t page_end = KERNEL_PAGE_END;

	printk("paging: %x, %x\n", paging_start, paging_end);
	printk("page: %x, %x\n", page_start, page_end);
	printk("page num:%d, %d, %d\n", a, b, c);
	
	while(1)
	{

	}
}