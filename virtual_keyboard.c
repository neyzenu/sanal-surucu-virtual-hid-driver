#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/input.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/version.h>

#define DEVICE_NAME "virtualkbd"
#define CLASS_NAME "virtualkbd"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("User");
MODULE_DESCRIPTION("Virtual Keyboard Driver");
MODULE_VERSION("0.1");

static struct input_dev *vkbd_dev;
static int major_number;
static struct class *vkbd_class = NULL;
static struct device *vkbd_device = NULL;

static int vkbd_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "VirtualKBD: Device opened\n");
    return 0;
}

static int vkbd_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "VirtualKBD: Device closed\n");
    return 0;
}

static ssize_t vkbd_write(struct file *file, const char __user *buffer,
                          size_t length, loff_t *offset)
{
    char command[8];
    int keycode, pressed;
    
    if (length > sizeof(command))
        return -EINVAL;
        
    if (copy_from_user(command, buffer, length))
        return -EFAULT;
    
    // Format of command: [keycode]:[0/1] (0 for release, 1 for press)
    if (sscanf(command, "%d:%d", &keycode, &pressed) != 2)
        return -EINVAL;
        
    if (keycode < 0 || keycode > KEY_MAX)
        return -EINVAL;
        
    // Send the key event
    input_report_key(vkbd_dev, keycode, pressed);
    input_sync(vkbd_dev);
    
    printk(KERN_INFO "VirtualKBD: Key %d %s\n", keycode, pressed ? "pressed" : "released");
    
    return length;
}

static struct file_operations fops = {
    .open = vkbd_open,
    .release = vkbd_release,
    .write = vkbd_write,
};

static int __init vkbd_init(void)
{
    int ret;
    
    // Register the character device
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "VirtualKBD: Failed to register character device\n");
        return major_number;
    }
    
    // Create the device class - compatible with different kernel versions
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 0, 0)
    vkbd_class = class_create(CLASS_NAME);
#else
    vkbd_class = class_create(THIS_MODULE, CLASS_NAME);
#endif
    if (IS_ERR(vkbd_class)) {
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "VirtualKBD: Failed to create device class\n");
        return PTR_ERR(vkbd_class);
    }
    
    // Create the device
    vkbd_device = device_create(vkbd_class, NULL, MKDEV(major_number, 0),
                               NULL, DEVICE_NAME);
    if (IS_ERR(vkbd_device)) {
        class_destroy(vkbd_class);
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "VirtualKBD: Failed to create device\n");
        return PTR_ERR(vkbd_device);
    }
    
    // Allocate input device
    vkbd_dev = input_allocate_device();
    if (!vkbd_dev) {
        printk(KERN_ALERT "VirtualKBD: Not enough memory for input device\n");
        device_destroy(vkbd_class, MKDEV(major_number, 0));
        class_destroy(vkbd_class);
        unregister_chrdev(major_number, DEVICE_NAME);
        return -ENOMEM;
    }
    
    // Configure input device
    vkbd_dev->name = "Virtual Keyboard";
    vkbd_dev->phys = "virtual/input0";
    vkbd_dev->id.bustype = BUS_VIRTUAL;
    vkbd_dev->id.vendor = 0x1234;
    vkbd_dev->id.product = 0x5678;
    vkbd_dev->id.version = 0x0100;
    
    // Set the capabilities for keyboard keys
    set_bit(EV_KEY, vkbd_dev->evbit);
    for (int i = 0; i < KEY_MAX; i++)
        set_bit(i, vkbd_dev->keybit);
    
    // Register the input device
    ret = input_register_device(vkbd_dev);
    if (ret) {
        printk(KERN_ALERT "VirtualKBD: Failed to register input device\n");
        input_free_device(vkbd_dev);
        device_destroy(vkbd_class, MKDEV(major_number, 0));
        class_destroy(vkbd_class);
        unregister_chrdev(major_number, DEVICE_NAME);
        return ret;
    }
    
    printk(KERN_INFO "VirtualKBD: Initialized successfully with major number %d\n", major_number);
    return 0;
}

static void __exit vkbd_exit(void)
{
    input_unregister_device(vkbd_dev);
    device_destroy(vkbd_class, MKDEV(major_number, 0));
    class_destroy(vkbd_class);
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "VirtualKBD: Exited\n");
}

module_init(vkbd_init);
module_exit(vkbd_exit);
