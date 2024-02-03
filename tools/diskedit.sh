# Disk utility for mounting and unmounting disk drives
# used specifically for running the Lunaris kernel
#
# Lunaris kernel 12-20-2023
# diskedit.sh v1.0.0

#!/bin/bash

set -e

echo "Disk Edit version v1.0.0"
echo "Created by Jaime Garza for the Lunaris kernel"

if [ "$#" -eq 0 ]; then
  echo "usage: diskedit.sh [foo.img], [-m] | [-u]"
  exit 1
fi

# Get arguments
if [ "$2" = "-m" ]; then
  echo "Creating loop device..."
  sudo losetup loop101 "$1" -P
  echo "Mounting disk..."
  # Partition number 1 is the Ext2 Filesystem. If it's not, something is wrong with the partition layout
  sudo mount /dev/loop101p1 /mnt
  echo "Done."
elif [ "$2" = "-u" ]; then
  echo "Unmounting loop device..."
  sudo umount /dev/loop101p1
  echo "Detaching loop device..."
  sudo losetup -d /dev/loop101
  echo "Done."
else
# User gave an invalid option. Set on fire, and run.
  echo "diskedit.sh: invalid option '$2'"
  exit 1
# Now we're dead :(
fi