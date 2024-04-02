#include <fs/vfs.h>
#include <fs/devfs.h>
#include <lunaris/mm.h>
#include <errno.h>
#include <lunaris/task.h>
#include <lunaris/debug.h>
#include <lunaris/fcntl.h>
#include <common.h>
#include <stdint.h>
#include <lunaris/printk.h>
#include <lunaris/video.h>

/* my very simple devfs implementation */

device_node_t * device_list = 0;
int dev_id = 0;

int devfs_get_device_byname(char * dev)
{
  int i = 0;
  if (strlen(dev) > 1)
  {
    if (dev[0] == '/')
    {
      if (dev[0] == '/' && dev[1] != 0)
        strchr(dev, '/');
    }
    if (dev[strlen(dev) - 1] == '/')
    {
      dev[strlen(dev) - 1] = '\0';
    }
  }

  do
  {
    /* should return 0 if there are no visible differences */
    if (strcmp(device_list[i].name, dev) == 0)
    {
      return i;
    }
    i++;
  } while (i < dev_id);
  
  /* no such device */
  return -ENODEV;
}

int devfs_get_registered_devices(void)
{
  int j = 0;
  for (int i = 0; i < DEVFS_DEVICES_MAX; i++)
  {
    if (device_list[i].node_type != 0)
    {
      j++;
    }
  }
  return j;
}

device_node_t devfs_get_device_node(int node_id)
{
  device_node_t node;
  /* we're gonna look for a device node that matches the id of node_id */
  for (int i = 0; i < dev_id; i++)
  {
    if (device_list[i].id == node_id)
    {
      node = device_list[i];
      return node;
    }
  }
  return node;
}

bool devfs_read_dir(device_node_t * node, void * buffer, uint32_t count)
{
  device_node_t ttydev;
  device_node_t * parent;
  ttydev = devfs_get_device_node(devfs_get_device_byname("tty0"));
  if (strcmp(ttydev.name, "tty0"))
  {
    DebugOutput("[DEVFS] TTY device not created yet!\n");
    return -ENODEV;
  }
  if (!node)
  {
    return -EINVAL;
  }
  if (devfs_get_device_byname(node->name) == -ENODEV)
  {
    return -ENODEV;
  }
  if (node->node_type != VFS_DIRECTORY)
  {
    return -ENOTDIR;
  }
  /*
    TODO: 
    add root directory reading and not just parent directory reading 
  */
  for (int i = 0; i < dev_id; i++)
  {
    parent = device_list[i].parent_node;
    //DebugOutput("[DEVFS] parent node info: %s | %s\n", parent->name, node->name);
    if (memcmp(parent->name, node->name, strlen(node->name)) == 0)
    {
      /* dont show the root node */
      if (strcmp(device_list[i].name, "/") != 0)
      {
        //DebugOutput("[DEVFS] Found a child %s!\n", device_list[i].node_type == VFS_DIRECTORY ? "directory" : "file");
        devfs_write((uint32_t)&ttydev, strlen(strcat(device_list[i].name, " ")), strcat(device_list[i].name, " "));
      }
    }
  }
  video_putc('\n');
  return 0;
}

void devfs_create_device(file_type_t type, bool r, bool w, char * name,  void (*devfs_read)(uint8_t * buffer, uint32_t count), void (*devfs_write)(uint8_t * buffer, uint32_t count), char * parent)
{
  devfs_device_t dev;
  memset((uint8_t *)&dev, 0, sizeof(devfs_device_t));
  strcpy(dev.name, name);
  dev.device_type = type;
  dev.read = r;
  dev.write = w;
  strcpy(dev.parent, parent);
  if (devfs_read)
    dev.devfs_read = devfs_read;
  if (devfs_write)
    dev.devfs_write = devfs_write;
  devfs_register(&dev);
}

int devfs_get_free_node_id()
{
  for (int i = 0; i < DEVFS_DEVICES_MAX; i++)
  {
    if (device_list[i].node_type == 0)
    {
      return i;
    }
  }
  /* no free node id found :( */
  return -1;
}

int devfs_unregister(device_node_t * node)
{
  if (!node) return -EINVAL;
  /* first, check if the device is registered */
  if (devfs_get_device_byname(node->name) == -ENODEV)
  {
    /* return null if we get an unregistered device */
    DebugOutput("[DEVFS] WARNING: tried to unregister an unregistered device!\n");
    return 0;
  }
  DebugOutput("[DEVFS] Unregistering device \"%s\"\n", node->name);
  memset(node, 0, sizeof(device_node_t));
  dev_id--;
  free(node);
}

devfs_device_t devfs_node2dev(device_node_t * node)
{
  devfs_device_t dev;
  if (!node) return dev;
  strcpy(dev.name, node->name);
  dev.device_type = node->node_type;
  dev.read = node->read;
  dev.write = node->write;
  strcpy(dev.parent, node->parent_node->name);
  return dev;
}

device_node_t * devfs_register(devfs_device_t * dev)
{
  if (!dev) return (device_node_t *)-EINVAL;
  if (devfs_get_device_byname(dev->name) != -ENODEV)
  {
    printk("devfs: cannot register device \"%s\" (device exists)\n", dev->name);
    DebugOutput("[DEVFS] WARNING: tried to register a registered device!\n");
    return NULL;
  }
  if (dev->device_type != VFS_DIRECTORY)
  {
    DebugOutput("[DEVFS] Registering device \"%s\"\n", dev->name);
    //printk("devfs: registering device \"%s\"\n", dev->name);
  }
  else
  {
    if (strcmp(dev->name, "/") != 0)
    {
      DebugOutput("[DEVFS] Registering directory \"%s\"\n", dev->name);
      //printk("devfs: registering directory \"%s\"\n", dev->name);
    }
    else
    {
      DebugOutput("[DEVFS] Creating root node...\n");
      //printk("devfs: creating root node...\n");
    }
  }
  device_node_t * dev_node = (device_node_t *)malloc(sizeof(device_node_t));
  if (!dev_node)
  {
    printk("devfs: failed to allocate memory for device!\n");
    return (device_node_t *)NULL;
  }
  memset((uint8_t *)dev_node, 0, sizeof(device_node_t));
  /* Copy all data from the device to the node */
  strcpy(dev_node->name, dev->name);
  dev_node->node_type = dev->device_type;
  dev_node->read = dev->read;
  dev_node->write = dev->write;
  int node_id = devfs_get_device_byname(dev->parent);
  dev_node->parent_node = &device_list[node_id];
  int free_id = devfs_get_free_node_id();
  dev_node->id = free_id;
  dev_node->devfs_write = dev->devfs_write;
  dev_node->devfs_read = dev->devfs_read;
  device_list[free_id] = *dev_node;
  dev_id++;
  /*DebugOutput("[DEVFS] Device node info:\n-  Node type: %d\n-  Readable: %s\n-  Writeable: %s\n-  ID: %d\n",
    dev_node->node_type, dev_node->read ? "true" : "false", dev_node->write ? "true" : "false", dev_node->id
  );*/
  return dev_node;
}

device_node_t * devfs_path2node(char * name)
{
  device_node_t * node = NULL;
  char * path = strstr(name, "/dev");
  char * next;
  if (!path)
  {
    DebugOutput("[DEVFS] Path does not start with /dev\n");
    return (device_node_t *)NULL;
  }
  if (path[0] != '/')
  {
    return (device_node_t *)NULL;
  }
  while(next)
  {
    next = strstr(path, "/");
    int length = next - path;
    if (*next == '/' && *(next + 1) != 0)
    {
      path = next + 1;
    }
    else
    {
      int dev_num = devfs_get_device_byname((char *)path);
      if (dev_num == -ENODEV)
      {
        DebugOutput("[DEVFS] WARNING: Name2Node was given a bogus name!\n");
        return (device_node_t *)NULL;
      }
      *node = devfs_get_device_node(dev_num);
      break;
    }
  }
  return (device_node_t *)node;
}

device_node_t * devfs_open(const char * pathname, int flags, int mode)
{
  device_node_t * node = NULL;
  DebugOutput("[DEVFS] Opening file: %s\n", pathname);
  if (flags & O_CREAT)
  {
    if (!devfs_get_device_node(devfs_get_device_byname("/")).write)
    {
      DebugOutput("[DEVFS] WARNING: Tried to create a file while devfs was read-only\n");
      return (device_node_t *)-EROFS;
    }
    return (device_node_t *)-ENOSYS;
  }
  else
  {
    node = devfs_path2node((char *)pathname);
  }
  return node;
}

uint32_t devfs_read(uint32_t node, uint32_t count, uint8_t * buffer)
{
  device_node_t * device = (device_node_t *)node;
  DebugOutput("[DEVFS] Reading device \"%s\"...\n", device->name);
  /* check permissions */
  if (device->read == false)
  {
    DebugOutput("[DEVFS] Cannot read \"%s\": permission denied\n", device->name);
    /* your not allowed to read the file */
    return -EPERM;
  }
  /* check if the device is valid */
  if (!device)
  {
    DebugOutput("[DEVFS] Cannot read \"%s\": invalid argument\n", device->name);
    return -EINVAL;
  }
  /* check if device node is registered */
  if (devfs_get_device_byname(device->name) == -ENODEV)
  {
    DebugOutput("[DEVFS] Cannot read \"%s\": no such device\n", device->name);
    printk("devfs: cannot read %s%s: no such device\n", DEVFS_PREFIX, device->name);
    return -ENODEV;
  }
  device->devfs_read(buffer, count);
  return 0;
}

uint32_t devfs_write(uint32_t node, uint32_t count, uint8_t * buffer)
{
  device_node_t * device = (device_node_t *)node;
  /* check permissions */
  if (device->write != true)
  {
    /* your not allowed to read the file */
    DebugOutput("[DEVFS] Cannot write to \"%s\": permission denied\n", device->name);
    DebugOutput("[DEVFS] %s is %s %s\n", device->name, device->read ? "readable" : "not readable", device->write ? "and writeable" : "but not writeable");
    printk("devfs: cannot write to %s%s: permission denied\n", DEVFS_PREFIX, device->name);
    return -EPERM;
  }
  /* check if the device is valid */
  if (!device)
  {
    DebugOutput("[DEVFS] Cannot write to \"%s\": invalid argument\n", device->name);
    return -EINVAL;
  }
  /* check if device node is registered */
  if (devfs_get_device_byname(device->name) == -ENODEV)
  {
    DebugOutput("[DEVFS] Cannot write to \"%s\": no such device\n", device->name);
    printk("devfs: cannot write to %s%s: no such device\n", DEVFS_PREFIX, device->name);
    return -ENODEV;
  }
  device->devfs_write(buffer, count);
  return 0;
}

int devfs_init(bool read, bool write)
{
  vfs_node_t * devfs_root = (vfs_node_t *)malloc(sizeof(vfs_node_t));
  if (!devfs_root) return -ENOMEM;
  strcpy(devfs_root->name, "DEV-FS");
  printk("devfs: init\n");
  DebugOutput("[DEVFS] Initializing...\n");
  devfs_root->read = devfs_read;
  /* Allocate memory for the device list */
  device_list = (device_node_t *)malloc(DEVFS_DEVICES_MAX * sizeof(device_node_t));
  memset((uint8_t *)device_list, 0, sizeof(device_node_t));
  if (!read && !write)
  {
    return -EINVAL;
  }
  /* create root node */
  devfs_create_device(VFS_DIRECTORY, read, write, "/", NULL, NULL, "");
  vfs_register_filesystem(devfs_root);
  return 0;
}

/*
  TODO:
  Work on mounting the psuedo-filesystem
*/
int devfs_mount(bool read, bool write)
{
  /* mount the filesystem as r/w (read-write) */
  if (devfs_init(read, write))
  {
    return -1;
  }/*
  file_t * dev_inode = (file_t *)malloc(sizeof(file_t));
  if (!dev_inode) return -ENOMEM;
  if (!ext2_getFile("dev", dev_inode, fs))
  {
    DebugOutput("[MOUNT] /dev: no such file or directory\n");
    printk("devfs: cannot mount to /dev: no such file or directory\n");
    return -ENOENT;
  }
  if (dev_inode->isDirectory == false)
  {
    DebugOutput("[MOUNT] /dev: not a directory\n");
    return -ENOTDIR;
  }*/
  return 0;
}
