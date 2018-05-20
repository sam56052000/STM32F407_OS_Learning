#ifndef PRINT_H_
#define PRINT_H_

#include "peripherals.h"

//
//	Variable Define
//	
//	*va_list  : Typedef Define to declare argv variable
//	numbers   : Use to save number as string
//
typedef char *va_list;
char numbers[30];

//
//	Print Macro 
//
//	int sizeof : To get the size of type(n) which align int
//	Va Start   : Set the address of first variable(t) into argv(ap)
//	Va Arg     : Get the address(ap) of pop data, and convert into target type(t)
//	Va End     : Reset the argv variable(ap)
//
#define INT_SIZEOF(n)	((sizeof(n)+sizeof(uint32_t)-1)&~(sizeof(uint32_t)-1))
#define VA_START(ap, v)	(ap=(va_list)&v+INT_SIZEOF(v))
#define VA_ARG(ap, t)	(*(t*)((ap+=INT_SIZEOF(t))-INT_SIZEOF(t)))
#define VA_END(ap)		(ap=(va_list)0)

//
//	Format Base Bits
//
#define FORMAT_BASE_MASK		0x00FF
#define FORMAT_BASE_O			0x0008
#define FORMAT_BASE_X			0x0010
#define FORMAT_BASE_D			0x000A
#define FORMAT_BASE_B			0x0002
#define FORMAT_BASE_RESET		0x0000

#define FORMAT_BASE(x)			(FORMAT_BASE_MASK&(x))
#define SET_FORMAT_BASE(x,t)	(x)&=~FORMAT_BASE_MASK;\
								(x)|=(t);
#define do_div(n,base)			({									\
								int16_t __res;						\
								__res=((uint32_t)n)%(uint32_t)base;	\
								n=((uint32_t) n)/(uint32_t)base;	\
								__res;})

//
//	Format Type Bits
//
#define FORMAT_TYPE_MASK		0xFF00
#define FORMAT_TYPE_SIGN_BIT	0x0100
#define FORMAT_TYPE_NONE		0x0000
#define FORMAT_TYPE_CHAR		0x0100
#define FORMAT_TYPE_UCHAR		0x0200
#define FORMAT_TYPE_INT16		0x0300
#define FORMAT_TYPE_UINT16		0x0400
#define FORMAT_TYPE_INT32		0x0500
#define FORMAT_TYPE_UINT32		0x0600
#define FORMAT_TYPE_INT64		0x0700
#define FORMAT_TYPE_UINT64		0x0800
#define FORMAT_TYPE_PTR			0x0A00
#define FORMAT_TYPE_STR			0x0C00
#define FORMAT_TYPE_PERCENT		0x1000

#define FORMAT_TYPE(x)			((x)&FORMAT_TYPE_MASK)
#define SET_FORMAT_TYPE(x,t)	(x)&=~FORMAT_TYPE_MASK;\
								(x)|=(t);
#define FORMAT_SIGNED(x)		((x)&FORMAT_TYPE_SIGN_BIT)

//
//	Format Flag Bits
//
#define FORMAT_FLAG_MASK		0xFFFF0000
#define FORMAT_FLAG_SPACE		0x00010000
#define FORMAT_FLAG_ZEROPAD		0x00020000
#define FORMAT_FLAG_LEFT		0x00040000
#define	FORMAT_FLAG_WIDTH		0x00100000

#define FORMAT_FLAG(x)			((x)&FORMAT_FLAG_MASK)
#define SET_FORMAT_FLAG(x,f)	((x)|=(f))

//
//	Function
//
void printk(char *fmt, ...);
void VsnPrintf(char *buf, int32_t size, char *fmt, va_list args);
uint32_t Format_Decode(char *fmt, uint32_t *flags);
void memcopy(char *dest, char *src, uint16_t count);
char *number(char *str, int32_t num, int32_t base, uint32_t flags);

#endif