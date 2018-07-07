#include "filesystem.h"

int32_t Register_File_System(super_block_t *type, uint32_t id)
{
	if(fs_type[id] == NULL)
	{
		fs_type[id] = type;

		return 0;
	}

	return -1;
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
	uint32_t tmp, next, num;

	char name[ROMFS_MAX_FILE_NAME], fname[ROMFS_MAX_FILE_NAME];

	Get_the_File_Name(dir,fname);

	//
	//	Load File into Memory
	//
	uint32_t max_p_size = ( ROMFS_MAX_FILE_NAME + sizeof(romfs_inode_t) );

	max_p_size = max_p_size > (block->device->sector_size) ? max_p_size : (block->device->sector_size);
	
	if( (p = (romfs_inode_t *)Kmalloc(max_p_size)) == NULL)
	{
		goto ERR_OUT_NULL;
	}

	block->device->Dout(block->device, p, 0, block->device->sector_size);

	next = ROMFS_GET_FIRST_FILE_HEADER(p);

	//
	//	The First dir is disk name, output "name" is secondary dir
	//
	dir = Bmap(name, dir);

	while(1)
	{
		//
		//	Caluclate File Offset
		//
		tmp = ( be32_to_le32(next) ) & ROMFS_NEXT_MASK;

		if(tmp >= block->device->storage_size)
		{
			goto ERR_OUT_KMALLOC;
		}

		if(tmp != 0)
		{
			block->device->Dout(block->device, p, tmp, block->device->sector_size);

			//
			//	Compare the file name (have dir and file)
			//
			if( !strcmp(p->name, name) )
			{
				if( !strcmp(name, fname) )
				{
					goto FOUND;
				}
				else
				{
					dir = Bmap(name, dir);
					
					//
					//	The spec have first file address
					//
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
		}
	}

FOUNDDIR:
	while(1)
	{
		tmp = (be32_to_le32(next)) & ROMFS_NEXT_MASK;

		if(tmp != 0)
		{
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
	//
	//	Init File System Structure
	//
	uint32_t i;

	for(i = 0; i < MAX_SUPER_BLOCK; i++)
	{
		fs_type[i] = NULL;
	}

	//
	//	Init romfs File System
	//
	Register_File_System(&romfs_super_block, ROMFS);

	fs_type[ROMFS]->Namei = Simple_Romfs_Namei;
	fs_type[ROMFS]->Get_daddr = Romfs_Get_Data_Addr;
	fs_type[ROMFS]->device = storage[DISK_1];
	fs_type[ROMFS]->name = "romfs";
}