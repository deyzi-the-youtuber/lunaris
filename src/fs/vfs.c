#include <fs/vfs.h>
#include <errno.h>
#include <lunaris/debug.h>
#include <lunaris/printk.h>
#include <lunaris/task.h>
#include <lunaris/syscall.h>

vfs_node_t * fs = 0;
int _fs = 0;

uint32_t vfs_read(vfs_node_t * node, uint32_t size, uint8_t * buffer)
{
  if (node->read)
    return node->read((uint32_t)node, size, buffer);
  else
    return -EINVAL;
}

uint32_t vfs_write(vfs_node_t * node, uint32_t size, uint8_t * buffer)
{
  if (node->write)
    return node->write((uint32_t)node, size, buffer);
  else
    return -EINVAL;
}

void vfs_open(vfs_node_t * node, uint8_t read, uint8_t write)
{
  if (node->read)
    node->open((struct vfs_node *)node, read, write);
}

void vfs_close(vfs_node_t * node)
{
  if (node->write)
    node->close((struct vfs_node *)node);
}

void vfs_register_filesystem(vfs_node_t * filesystem)
{
  printk("vfs: registering filesystem \"%s\"...\n", filesystem->name);
  DebugOutput("[VFS] Registering filesystem \"%s\"...\n", filesystem->name);
  fs[++_fs] = *filesystem;
}

int find_fd(vfs_node_t * file)
{
  Process * p = getCurrentProcess();
  int i = 0;
  for(; i < MAX_OPEN_FILES; ++i)
  {
    if (!p->open_files[i])
    {
      p->open_files[i] = file;
      break;
    }
  }
  if (i == MAX_OPEN_FILES)
  {
    return -1;
  }
  return i + 2;
}

vfs_node_t vfs_get_fs(char * name)
{
  int i = 0;
  do
  {
    vfs_node_t node = fs[i];
    if (strcmp(node.name, name) == 0)
    {
      return node;
    }
    i++;
  } while (i < 256);
  return;
}
