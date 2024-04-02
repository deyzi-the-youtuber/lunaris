#include <fs/vfs.h>
#include <fs/ext2.h>
#include <stdint.h>
#include <lunaris/mm.h>
#include <lunaris/blk/ide.h>
#include <lunaris/printk.h>
#include <lunaris/debug.h>
#include <errno.h>

uint8_t buffer[512];
ext2_superblock * sb;
ext2_block_group_descriptor * bgd;
ext2_inode * inode_table;
int ext2_start = 0;

bool ext2_is_valid(int part)
{
	readSector(0, part + 2, buffer); // Superblock begins at partition sector + 2
  DebugOutput("[EXT2] Superblock signature: 0x%x\n[EXT2] Expected signature: 0x%x\n", ((ext2_superblock *)buffer)->signature, EXT2_SIGNATURE);
	return ((ext2_superblock *)buffer)->signature == EXT2_SIGNATURE;
}

void ext2_read_superblock(int part)
{
  memset(buffer, 0, 512);
  readSector(0, part + 2, buffer);
  sb = (ext2_superblock *)buffer;
}

void ext2_read_inode(uint32_t inode, ext2_inode * buf)
{
  DebugOutput("[EXT2] Reading inode %d...\n", inode);
  uint32_t block_group = ext2_get_block_group_inode(inode);
  uint8_t * block = ext2_block_allocate();
  ext2_block_group_descriptor * bgd = (ext2_block_group_descriptor *)ext2_read_block(2, block);
  for(int i = 0; i < block_group; i++)
  {
    bgd++;
  }
  ext2_read_block(bgd->inode_table + ext2_get_block_inode(inode), (uint8_t *)buf);
  ext2_inode * _inode = (ext2_inode *)buf;
  uint32_t index = ext2_get_index_inode(inode) % (sb->block_size / sb->inode_size);
  for(int i = 0; i < index; i++)
  {
    _inode++;
  }
  memcpy(buf, _inode, sizeof(ext2_inode));
  ext2_block_free(block);
}

void ext2_list_dir(ext2_directory * dir)
{
  DebugOutput("[EXT2] Directory name length: %d\n", dir->name_length);
	uint32_t add = 0;
	while(dir->inode != 0 && add < sb->block_size)
  {
		char * name = (char *)malloc(dir->name_length + 1);
		name[dir->name_length] = 0;
		memcpy(name, &dir->type + 1, dir->name_length);
		if (name[0] != '.' && name[0] != '\0')
    {
      printk("%s\n", name);
    }
		add += dir->size;
		dir = (ext2_directory *)((uint32_t)dir + dir->size);
    free(name);
	}
}

int ext2_read_dir(uint32_t inode)
{
  DebugOutput("[EXT2] Reading directory...\n");
  ext2_inode * _inode_buf = ext2_inode_allocate();
  ext2_read_inode(inode, _inode_buf);
  if ((_inode_buf->type & 0xF000) != EXT2_S_IFDIR)
  {
    DebugOutput("[EXT2] Inode is not a directory!\n");
    free(_inode_buf);
    return -ENOTDIR;
  }
  uint8_t * buf = ext2_block_allocate();
  for(int i = 0; i < 12; i++)
  {
    uint32_t block = _inode_buf->block_pointers[i];
    if (block == 0 || block > sb->total_blocks)
    {
      break;
    }
    ext2_read_block(block, buf);
    ext2_list_dir((ext2_directory *)buf);
  }
  ext2_block_free(buf);
  free(_inode_buf);
  return 1;
}

void ext2_inode2vfs_node(ext2_inode * inode, vfs_node_t * node)
{
  if (!inode)
  {
    return (void)-EINVAL;
  }
  node->uid = inode->user_id;
  node->gid = inode->group_id;
  node->mask = inode->flags & 0xFFF;
  // reset flags before setting
  node->flags = 0;

  // messy spagheti incoming
  if (inode->type & EXT2_S_IFREG)
    node->flags |= VFS_FILE;
  if (inode->type & EXT2_S_IFDIR)
    node->flags |= VFS_DIRECTORY;
  if (inode->type & EXT2_S_IFBLK)
    node->flags |= VFS_BLOCK_DEVICE;
  if (inode->type & EXT2_S_IFCHR)
    node->flags |= VFS_CHARACTER_DEVICE;
  if (inode->type & EXT2_S_IFIFO)
    node->flags |= VFS_PIPE;
  if (inode->type & EXT2_S_IFLNK)
    node->flags |= VFS_SYMBOLIC_LINK;
  // yummy spaghetti
}

uint32_t ext2_find_file(char * fn, uint32_t dir_inode, ext2_inode * inode)
{
	if(fn[0] == '/')
  {
		fn++;
		dir_inode = 2;
	}
	uint32_t name_len = strlen(fn);
	if(name_len == 0) return dir_inode;
	uint8_t * buf = ext2_block_allocate();
	char * cfn = (char *)malloc(name_len);
	while(*fn != 0)
  {
		uint32_t strindex = indexOf('/', fn);
		substrr(0, strindex, fn, cfn);
		fn += strindex+(strindex == strlen(fn) ? 0 : 1);
		ext2_read_inode(dir_inode, inode);
		bool found = 0;
		for(int i = 0; i < 12 && !found; i++)
    {
			uint32_t block = inode->block_pointers[i];
			if (block == 0 || block > sb->total_blocks) break;
			ext2_read_block(block, buf);
			ext2_directory * dir = (ext2_directory *)buf;
			uint32_t add = 0;
			while(dir->inode != 0 && add < sb->block_size && !found)
      {
				char name[dir->name_length + 1];
        memset(name, '\0', dir->name_length + 1);
				name[dir->name_length] = '\0';
				memcpy(name, &dir->type + 1, dir->name_length);
				if(strcmp(name, cfn) == 0)
        {
          DebugOutput("[EXT2] Found inode of \"%s\"!\n", cfn);
					dir_inode = dir->inode;
					ext2_read_inode(dir_inode, inode);
					found = 1;
				}
				free(name);
				add += dir->size;
				dir = (ext2_directory *)((uint32_t)dir + dir->size);
			}
		}
		if(!found)
    {
			dir_inode = 0;
			fn += strlen(fn);
		}
	}
	free(cfn);
	ext2_block_free(buf);
	return dir_inode;
}

int ext2_get_file(char * fn, vfs_node_t * node)
{
  ext2_inode * inode = ext2_inode_allocate();
  uint32_t dir_inode = ext2_find_file(fn, 2, inode);
  if(!dir_inode)
  {
    return -ENOENT;
  }
  ext2_inode2vfs_node(inode, node);
  node->inode = dir_inode;
  strncpy(node->name, fn, sizeof(fn));
  free(inode);
  return 1;
}

void ext2_read_slink(uint32_t block, uint8_t * buf){
	uint8_t * bbuf = ext2_block_allocate();
	ext2_read_block(block, bbuf);
	uint32_t *blocks = (uint32_t *)bbuf;
	uint32_t numblocks = sb->block_size/sizeof(uint32_t);
	for(int i = 0; i < numblocks; i++){
		if(blocks[i] == 0) break;
		ext2_read_block(blocks[i], buf+i*sb->block_size);
	}
	ext2_block_free(bbuf);
}

void ext2_read_dlink(uint32_t block, uint8_t * buf){
	uint8_t * bbuf = ext2_block_allocate();
	ext2_read_block(block, bbuf);
	uint32_t * blocks = (uint32_t *)bbuf;
	uint32_t numblocks = sb->block_size/sizeof(uint32_t);
	uint32_t singsize = numblocks*sb->block_size;
	for(int i = 0; i < numblocks; i++){
		if(blocks[i] == 0) break;
		ext2_read_block(blocks[i], buf+i*singsize);
	}
	ext2_block_free(bbuf);
}

int ext2_read(vfs_node_t * file, uint8_t * buf)
{
  if (!file->inode)
    return 0;

  ext2_inode * inode = ext2_inode_allocate();
  ext2_read_inode(file->inode, inode);

  // Calculate the total number of blocks required to read
  uint32_t total_blocks = (inode->size_lower + sb->block_size - 1) / sb->block_size;
  uint32_t blocks_read = 0;

  for (int i = 0; i < 12 && blocks_read < total_blocks; i++) {
    uint32_t block = inode->block_pointers[i];
    if (block == 0 || block > sb->total_blocks)
      break;
    ext2_read_block(block, buf + blocks_read * sb->block_size);
    blocks_read++;
  }
  // Handle indirect blocks
  if (inode->s_pointer && blocks_read < total_blocks)
  {
    ext2_read_slink(inode->s_pointer, buf + blocks_read * sb->block_size);
    blocks_read += sb->block_size / sizeof(uint32_t);
  }
  if (inode->d_pointer && blocks_read < total_blocks)
  {
    ext2_read_dlink(inode->d_pointer, buf + blocks_read * sb->block_size);
    blocks_read += (sb->block_size / sizeof(uint32_t)) * (sb->block_size / sizeof(uint32_t));
  }
  free(inode);
  return 1;
}

int ext2_mount(int partition_start)
{
  vfs_node_t * ext2_root = (vfs_node_t *)malloc(sizeof(vfs_node_t));
  memset(ext2_root, 0, sizeof(vfs_node_t));
  if (!ext2_root) return -ENOMEM;
  strcpy(ext2_root->name, "EXT2-FS");
  DebugOutput("[EXT2] Initializing...\n");
  ext2_start = partition_start;
  printk("ext2: mounting filesystem...\n");
  if (!ext2_is_valid(partition_start))
  {
    printk("ext2: partition is not ext2\n");
    return -EINVAL;
  }
  printk("ext2: reading superblock...\n");
  ext2_read_superblock(partition_start);
  printk("ext2: version %d.%d\n", sb->version_major, sb->version_minor);
  sb->block_size = 1024 << sb->block_size;
  sb->fragment_size = 1024 << sb->fragment_size;
  if (sb->version_major < 1)
  {
    printk("ext2: setting defaults...\n");
    sb->first_inode = 11;
    sb->inode_size = 128;
  }
  vfs_register_filesystem(ext2_root);
  return 0;
}

uint32_t ext2_block_to_sector(uint32_t block)
{
  return ext2_start + (sb->block_size / 512) * block;
}

uint8_t * ext2_read_block(uint32_t block, uint8_t * buf)
{
  readSectors(0, ext2_block_to_sector(block), sb->block_size / 512, buf);
  return buf;
}

uint8_t * ext2_block_allocate()
{
  return (uint8_t *)malloc(sb->block_size);
}

void ext2_block_free(uint8_t * block)
{
  free((void *)block);
}

ext2_inode * ext2_inode_allocate()
{
  return (ext2_inode *)malloc(sb->inode_size);
}

void ext2_inode_free(ext2_inode * inode)
{
  free((void *)inode);
}

uint32_t ext2_get_block_group_inode(uint32_t inode)
{
  return (inode - 1) / sb->inodes_per_group;
}

uint32_t ext2_get_index_inode(uint32_t inode)
{
  return (inode - 1) % sb->inodes_per_group;
}

uint32_t ext2_get_block_inode(uint32_t inode)
{
  return (ext2_get_index_inode(inode) * sb->inode_size) / sb->block_size;
}
