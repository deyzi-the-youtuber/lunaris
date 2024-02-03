#include <stdint.h>
#include <common.h>
#include <kernel/fs/vfs.h>

#ifndef __DEVFS_H
#define __DEVFS_H

#define DEVFS_ROOT_DIR "/"
#define DEVFS_PREFIX "/dev/"
#define DEVFS_DEVICES_MAX 256

typedef struct
{
  file_type_t device_type;
  char name[64];
  char parent[64];
  bool read;
  bool write;
  void (*devfs_read)(uint8_t * buffer, uint32_t count);
  void (*devfs_write)(uint8_t * buffer, uint32_t count);
} devfs_device_t;

typedef struct device_node_t
{
  file_type_t node_type;
  char name[64];
  struct device_node_t * parent_node;
  bool read;
  bool write;
  uint32_t id;
  void (*devfs_read)(uint8_t * buffer, uint32_t count);
  void (*devfs_write)(uint8_t * buffer, uint32_t count);
} device_node_t;

/* initialize devfs */
int devfs_init(bool read, bool write);
/* manually registers a device */
device_node_t * devfs_register(devfs_device_t * dev);
/* creates a device */
void devfs_create_device(file_type_t type, bool r, bool w, char * name,  void (*devfs_read)(uint8_t * buffer, uint32_t count), void (*devfs_write)(uint8_t * buffer, uint32_t count), char * parent);
/* gets device node id by name */
int devfs_get_device_byname(char * dev);
/* gets device node by node id */
device_node_t devfs_get_device_node(int node_id);
/* converts a node to a device */
devfs_device_t devfs_node2dev(device_node_t * node);
/* reads a device if device has read permissions */
uint32_t devfs_read(uint32_t node, uint32_t offset, uint32_t count, uint8_t * buffer);
/* writes a device if device has write permissions */
uint32_t devfs_write(uint32_t node, uint32_t offset, uint32_t count, uint8_t * buffer);
/* converts a path into a node */
device_node_t * devfs_path2node(char * name);
device_node_t * devfs_open(const char * pathname, int flags, int mode);
/* mounts devfs */
int devfs_mount(bool read, bool write);
bool devfs_read_dir(device_node_t * node, void * buffer, uint32_t count);

#endif