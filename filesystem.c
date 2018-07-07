#include "filesystem.h"

int32_t Register_File_System(super_block_t *type, uint32_t id)
{
	// if(fs_type[id] == NULL)
	// {
		fs_type[id] = type;

		return 0;
	// }

	// return -1;
}

void Unregister_File_System(uint32_t id)
{
	fs_type[id] = NULL;
}

char *Bmap(char *tmp, char *dir)
{
	uint32_t n;
	char *p = strchr(dir, '/');

	if(!p)
	{
		strcpy(tmp, dir);
		return NULL;
	}

	n = p - dir;

	n = (n > ROMFS_MAX_FILE_NAME) ? ROMFS_MAX_FILE_NAME : n;

	strncpy(tmp, dir, n);

	return p+1;
}

char *Get_the_File_Name(char *p, char *name)
{
	char *tmp = p;
	int32_t index;

	for(index = 0; *tmp; tmp++)
	{
		if(*tmp == '/')
		{
			index = 0;
			continue;
		}
		else
		{
			name[index] = *tmp;
			index++;
		}
	}

	name[index] = '\0';

	return name;
}

inode_t *Simple_Romfs_Namei(super_block_t *block, char *dir)
{
	inode_t *inode;
	romfs_inode_t *p;
	uint32_t tmp, next,num;

	char name[ROMFS_MAX_FILE_NAME], fname[ROMFS_MAX_FILE_NAME];
	uint32_t max_p_size = ( ROMFS_MAX_FILE_NAME + sizeof(romfs_inode_t) );

	printk("step-1\n");

	max_p_size = max_p_size > (block->device->sector_size) ? max_p_size : (block->device->sector_size);
	
	Get_the_File_Name(dir,fname);

	printk("step0\n");

	if( (p = (romfs_inode_t *)Kmalloc(max_p_size)) == NULL)
	{
		printk("get error memory\n");
		goto ERR_OUT_NULL;
	}

	printk("step1\n");

	dir = Bmap(name,dir);

	// if( block->device->Dout(block->device, p, 0, block->device->sector_size) )
	// 	goto ERR_OUT_KMALLOC;
	block->device->Dout(block->device, p, 0, block->device->sector_size);

	next = ROMFS_GET_FIRST_FILE_HEADER(p);

	while(1)
	{
		tmp = ( be32_to_le32(next) )&ROMFS_NEXT_MASK;

		if(tmp >= block->device->storage_size)
		{
			goto ERR_OUT_KMALLOC;
		}

		if(tmp!=0)
		{
			// if( block->device->Dout(block->device,p,tmp,block->device->sector_size) )
			// {
			// 	goto ERR_OUT_KMALLOC;
			// }
			block->device->Dout(block->device,p,tmp,block->device->sector_size);

			if( !strcmp(p->name,name) )
			{
				if( !strcmp(name,fname) )
				{
					goto FOUND;
				}
				else
				{
					dir = Bmap(name,dir);
					next = p->spec;

					if(dir == NULL)
					{
						goto FOUNDDIR;
					}
				}
			}
			else
			{
				next = p->next;	
			}
		}
		else
		{
			goto ERR_OUT_KMALLOC;
			printk("get error memory2\n");
		}
	}

	

FOUNDDIR:
	printk("step2\n");
	while(1)
	{
		tmp = (be32_to_le32(next))&ROMFS_NEXT_MASK;

		if(tmp != 0)
		{
			// if( block->device->Dout(block->device, p, tmp, block->device->sector_size) )
			// {
			// 	goto ERR_OUT_KMALLOC;
			// }
			block->device->Dout(block->device, p, tmp, block->device->sector_size);

			if( !strcmp(p->name,name) )
			{
				goto FOUND;
			}
			else
			{
				next = p->next;
			}
		}
		else
		{
			goto ERR_OUT_KMALLOC;
		}
	}

FOUND:
	printk("step3\n");
	if( (inode = (inode_t *)Kmalloc(sizeof(inode_t)) ) == NULL)
	{
		goto ERR_OUT_KMALLOC;
	}

	num = strlen(p->name);

	if( (inode->name = (char *)Kmalloc(num) ) == NULL)
	{
		goto ERR_OUT_KMEM_CACHE_ALLOC;
	}

	strcpy(inode->name, p->name);

	inode->dsize = be32_to_le32(p->size);
	inode->daddr = tmp;			
	inode->super = &romfs_super_block;

	Kfree(p);

	return inode;

ERR_OUT_KMEM_CACHE_ALLOC:
	Kfree(inode);

ERR_OUT_KMALLOC:
	Kfree(p);

ERR_OUT_NULL:
	return NULL;
}

uint32_t Romfs_Get_Data_Addr(inode_t *node)
{
	int32_t name_size = strlen(node->name);

	return ROMFS_GET_FILE_DATA_OFFSET(node->daddr, name_size);
}

void FileSystem_Init(void)
{
	uint32_t a = Register_File_System(&romfs_super_block, ROMFS);

	printk("register result = %d\n", a);

	fs_type[ROMFS]->Namei = Simple_Romfs_Namei;

	fs_type[ROMFS]->Get_daddr = Romfs_Get_Data_Addr;

	fs_type[ROMFS]->device = &disk1;

	fs_type[ROMFS]->name = "romfs";
}