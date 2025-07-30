#!/bin/bash
# Script to install the virtual keyboard module

echo "Building kernel module..."
make

echo "Installing kernel module..."
sudo insmod virtual_keyboard.ko

echo "Creating device node if not created automatically..."
if [ ! -e /dev/virtualkbd ]; then
    major=$(dmesg | grep "VirtualKBD: Initialized successfully" | grep -o "with major number [0-9]*" | awk '{print $4}')
    sudo mknod /dev/virtualkbd c $major 0
    sudo chmod 666 /dev/virtualkbd
fi

echo "Installation complete. You can now use the virtual keyboard."
echo "To load the module at boot, add 'virtual_keyboard' to /etc/modules."
