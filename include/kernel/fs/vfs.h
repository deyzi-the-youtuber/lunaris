#include <stdint.h>
#include <common.h>

#ifndef __VFS_H
#define __VFS_H

typedef struct vfs_node vfs_node_t;

typedef uint32_t (*fs_read)(uint32_t, uint32_t, uint32_t, uint8_t *);
typedef uint32_t (*fs_write)(uint32_t, uint32_t, uint32_t, uint8_t *);
typedef void (*fs_open)(struct vfs_node *, uint8_t read, uint8_t write);
typedef void (*fs_close)(struct vfs_node *);

typedef struct vfs_node
{
	char name[256];
	uint32_t mask;
	uint32_t uid;
	uint32_t gid;
	uint32_t flags;
	uint32_t inode;
	uint32_t length;
	uint32_t impl;
	fs_read read;
	fs_write write;
	fs_open open;
	fs_close close;
	//readdir_type_t readdir;
	//finddir_type_t finddir;
	struct fs_node * ptr;
}vfs_node_t;

struct dirent
{
	char name[256];
	uint32_t ino;
};

typedef enum
{
  VFS_FILE               = 1,
  VFS_CHARACTER_DEVICE   = 2,
  VFS_BLOCK_DEVICE       = 3,
  VFS_PIPE               = 4,
  VFS_SYMBOLIC_LINK      = 5,
  VFS_SOCKET             = 6,
  VFS_DIRECTORY          = 128,
  VFS_MOUNT_POINT        = 256
}file_type_t;

uint32_t vfs_read(vfs_node_t * node, uint32_t offset, uint32_t size, uint8_t * buffer);
uint32_t vfs_write(vfs_node_t * node, uint32_t offset, uint32_t size, uint8_t * buffer);
void vfs_open(vfs_node_t * node, uint8_t read, uint8_t write);
void vfs_close(vfs_node_t * node);

#endif