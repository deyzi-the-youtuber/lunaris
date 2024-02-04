#include <kernel/fs/vfs.h>
#include <kernel/errno.h>

uint32_t vfs_read(vfs_node_t * node, uint32_t offset, uint32_t size, uint8_t * buffer)
{
  if (node->read)
    return node->read(node, offset, size, buffer);
  else
    return -EINVAL;
}

uint32_t vfs_write(vfs_node_t * node, uint32_t offset, uint32_t size, uint8_t * buffer)
{
  if (node->write)
    return node->write(node, offset, size, buffer);
  else
    return -EINVAL;
}

void vfs_open(vfs_node_t * node, uint8_t read, uint8_t write)
{
  if (node->read)
    node->open(node, read, write);
}

void vfs_close(vfs_node_t * node)
{
  if (node->write)
    node->close(node);
}