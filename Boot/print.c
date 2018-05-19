#include "print.h"

//
//	Memory Copy Function
//
void memcopy(char *dest, char *src, uint16_t count)
{
	while(count --)
	{
		*dest++ = *src++;
	}
}

//
//	Calculate Number into string by type
//
char *number(char *str, int32_t num, int32_t base, uint32_t flags)
{
	static char *digits = "0123456789ABCDEF";

	int i = 0;
	int sign = 0;

	if( (FORMAT_SIGNED(flags)) && ((int32_t)num < 0) )
	{
		sign = 1;

		num = ~num + 1;
	}

	do
	{
		numbers[i++] = digits[do_div(num, base)];
	}
	while(num != 0);

	if( FORMAT_BASE(flags) == FORMAT_BASE_O )
	{
		numbers[i++]='0';
	}
	else if( FORMAT_BASE(flags) == FORMAT_BASE_X )
	{
		numbers[i++]='x';
		numbers[i++]='0';
	}
	else if( FORMAT_BASE(flags) == FORMAT_BASE_B )
	{
		numbers[i++]='b';
		numbers[i++]='0';
	}

	if(sign)
	{
		numbers[i++] = '-';
	}

	while(i-- > 0)
	{
		*str++ = numbers[i];
	}

	return str;
}

//
//	Decode the type of the variable
//
uint32_t Format_Decode(char *fmt, uint32_t *flags)
{
	char *start = fmt;

	//
	//	Set Default Format Type
	//
	SET_FORMAT_TYPE(*flags, FORMAT_TYPE_NONE);

	for( ; *fmt; ++fmt)
	{
		if(*fmt == '%')
		{
			break;
		}
	}

	fmt++;

	SET_FORMAT_BASE(*flags, FORMAT_BASE_RESET);

	switch(*fmt)
	{
		case 'c':
		{
			SET_FORMAT_TYPE(*flags, FORMAT_TYPE_CHAR);
		}
		break;

		case 's':
		{
			SET_FORMAT_TYPE(*flags, FORMAT_TYPE_STR);
		}
		break;

		case 'o':
		{
			SET_FORMAT_BASE(*flags, FORMAT_BASE_O);
			SET_FORMAT_TYPE(*flags, FORMAT_TYPE_UINT);
		}
		break;

		case 'x':
		case 'X':
		{
			SET_FORMAT_BASE(*flags, FORMAT_BASE_X);
			SET_FORMAT_TYPE(*flags, FORMAT_TYPE_UINT);
		}
		break;

		case 'd':
		case 'i':
		{
			SET_FORMAT_TYPE(*flags, FORMAT_TYPE_INT);
			SET_FORMAT_BASE(*flags, FORMAT_BASE_D);
		}
		break;

		case 'u':
		{
			SET_FORMAT_TYPE(*flags, FORMAT_TYPE_UINT);
			SET_FORMAT_BASE(*flags, FORMAT_BASE_D);
		}
		break;

		default:
		{

		}
		break;
	}

	//
	//	return read number of char
	//
	return (++fmt - start);
}

void VsnPrintf(char *buf, int32_t buf_size, char *fmt, va_list args)
{
	int num, copy_size;
	char *str, *end;
	uint32_t read, flags = 0;

	str = buf;
	end = buf + buf_size;

	//
	//	Check if buf address overflow
	//
	if(end < buf)
	{
		end = (char *)(-1);
		buf_size = end - buf;
	}

	//
	//	Decode the Data
	//
	while(*fmt)
	{
		char *old_fmt = fmt;

		read = Format_Decode(fmt, &flags);

		fmt += read;

		//
		//	Copy the normal string into buffer
		//
		copy_size = read;

		if(copy_size > end - str)
		{
			copy_size = end - str;
		}

		memcopy(str, old_fmt, copy_size);

		str += copy_size;

		//
		//	Print the Data
		//
		switch( FORMAT_TYPE(flags) )
		{
			case FORMAT_TYPE_NONE:
			{
				// DoNothing
			}
			break;

			case FORMAT_TYPE_CHAR:
			{
				str -= 2;

				char c = (char)VA_ARG(args, char);

				*str++ = c;
			}
			break;

			case FORMAT_TYPE_STR:
			{
				str -= 2;

				char *s;

				s = (char *)VA_ARG(args, char *);

				while( (str < end) && (*s != '\0') )
				{
					*str++ = *s++;
				}
			}
			break;

			default:
			{
				str -= 2;

				if(FORMAT_TYPE(flags) == FORMAT_TYPE_INT)
				{
					num = VA_ARG(args, int32_t);
				}
				else if(FORMAT_TYPE(flags) == FORMAT_TYPE_UINT)
				{
					num = VA_ARG(args, uint32_t);
				}
				else if(FORMAT_TYPE(flags) == FORMAT_TYPE_ULONG)
				{
					num = VA_ARG(args, uint64_t);
				}
				else if(FORMAT_TYPE(flags) == FORMAT_TYPE_LONG)
				{
					num = VA_ARG(args, int64_t);
				}
				else if(FORMAT_TYPE(flags) == FORMAT_TYPE_USHORT)
				{
					num = (uint16_t)VA_ARG(args, int16_t);
				}
				else if(FORMAT_TYPE(flags) == FORMAT_TYPE_SHORT)
				{
					num = (int16_t)VA_ARG(args, int16_t);
				}
				else
				{
					num = 0;
				}
			
				str = number(str, num, flags&FORMAT_BASE_MASK, flags);
			}
			break;
		}
	}

	//
	//	Set the last byte as char '\0'
	//
	if(str < end)
	{
		*str = '\0';
	}
	else
	{
		end[-1] = '\0';
	}
}

void printk(char *fmt, ...)
{
	static char print_buf[100];

	va_list args;

	VA_START(args, fmt);

	VsnPrintf(print_buf, sizeof(print_buf),fmt, args);

	VA_END(args);

	UART_Puts(print_buf);
}

void Test_Printk(void)
{
	char *s = "this is %s test";
	char c = 'C';
	int32_t d = -256;
	int32_t x = 0x12345678;

	printk("\nTesting printk:\n");
	printk("Print Char: %c\n", c);
	printk("Print String: %s\n", s);
	printk("Print Dec: %d\n", d);
	printk("Print Hex: %x\n", x);
}