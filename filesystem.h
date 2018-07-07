#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include "driver.h"
#include "memory.h"
#include "./Boot/string.h"
#include "./Boot/print.h"

#define MAX_SUPER_BLOCK		8
#define ROMFS				0

typedef struct super_block_t super_block_t;
typedef struct inode_t inode_t;

struct inode_t
{
	char *name;
	uint32_t flags;
	uint32_t dsize;				//file data size
	int32_t daddr;				//real data bias inside the block device
	super_block_t *super;
	
};

struct super_block_t
{
	inode_t *(*Namei)(super_block_t *super, char *p);
	uint32_t (*Get_daddr)(inode_t *);
	storage_device_t *device;
	char *name;
};

//
//	Function
//
int32_t Register_File_System(super_block_t *type, uint32_t id);
void Unregister_File_System(uint32_t id);

super_block_t *fs_type[MAX_SUPER_BLOCK];

//
// romfs File System
//
#define be32_to_le32(x) \
	((uint32_t)( \
	(((uint32_t)(x) & (unsigned int)0x000000ffUL) << 24) | \
	(((uint32_t)(x) & (unsigned int)0x0000ff00UL) <<  8) | \
	(((uint32_t)(x) & (unsigned int)0x00ff0000UL) >>  8) | \
	(((uint32_t)(x) & (unsigned int)0xff000000UL) >> 24) ))

struct romfs_super_block_t
{
	uint32_t word0;
	uint32_t word1;
	uint32_t size;
	uint32_t checksum;
	char name[0];

} romfs_super_block_t;

typedef struct romfs_inode_t
{
	uint32_t next;
	uint32_t spec;
	uint32_t size;
	uint32_t checksum;
	char name[0];

} romfs_inode_t;

super_block_t romfs_super_block;

#define ROMFS_MAX_FILE_NAME					(64)
#define ROMFS_NAME_ALIGN_SIZE				(16)
#define ROMFS_SUPER_UP_MARGIN				(16)
#define ROMFS_NAME_MASK						(~(ROMFS_NAME_ALIGN_SIZE-1))
#define ROMFS_NEXT_MASK						0xfffffff0

#define ROMFS_GET_FIRST_FILE_HEADER(p)		\
	((((strlen(((romfs_inode_t *)(p))->name)+ROMFS_NAME_ALIGN_SIZE+ROMFS_SUPER_UP_MARGIN))&ROMFS_NAME_MASK)<<24)

#define ROMFS_GET_FILE_DATA_OFFSET(p,num)	\
	(((((num)+ROMFS_NAME_ALIGN_SIZE)&ROMFS_NAME_MASK)+ROMFS_SUPER_UP_MARGIN+(p)))

//
//	Function
//
char *Bmap(char *tmp, char *dir);
char *Get_the_File_Name(char *p, char *name);
inode_t *Simple_Romfs_Namei(super_block_t *block, char *dir);
uint32_t Romfs_Get_Data_Addr(inode_t *node);

void FileSystem_Init(void);

#endif