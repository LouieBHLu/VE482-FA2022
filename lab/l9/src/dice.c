#include <linux/module.h>
#include <linux/printk.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/random.h>
// TODO: Add necessary header files
#include "dice.h"
#define max_str 16
int dice_major;            /* Major number of dice devices */
int dice_devs = DICE_DEVS; /* Number of dice devices */
int dice_num = DICE_NUM;   /* Initial number of dice in a device */
int gen_sides = GEN_SIDES; /* Initial number of sides of `arbitrary` dice game */
module_param(gen_sides, int, 0);
struct dice_dev dice_devices[DICE_DEVS]; /* An array of dice device */

static int __init dice_init(void);
static void __exit dice_exit(void);
static int dice_open(struct inode *inode, struct file *filp);
static int dice_release(struct inode *inode, struct file *filp);
static ssize_t dice_read(struct file *filp, char __user *buff, size_t count, loff_t *offp);
static ssize_t dice_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp);
static const char print_type2[6][4] = {
            "2","4","8","16","32","64"
};
MODULE_LICENSE("GPL");
module_init(dice_init);
module_exit(dice_exit);
// TODO: Make gen_sides a module parameter

static struct file_operations fops = {
    .read = dice_read,
    .write = dice_write,
    .open = dice_open,
    .release = dice_release,
};

static int __init dice_init(void)
{
    // TODO: Find Major number dynamically
    //  Hint: alloc_chrdev_region
    int i, reg_result;
    dev_t temp = MKDEV(42, 0);
    reg_result = register_chrdev_region(temp, DICE_DEVS, "Grandpa_dice");
    //TODO: Allocate memory for dices
    if (reg_result == 0){
        for ( i = 0; i < DICE_DEVS; i++){
            cdev_init(&dice_devices[i].dice_cdev, &fops);
            cdev_add(&dice_devices[i].dice_cdev, MKDEV(42, i), 1);
        }
        for ( i = 0; i < DICE_DEVS; i++){
            dice_devices[i].dice_type = i;
            dice_devices[i].num = DICE_NUM;
        }
        return 0;
    }
    //TODO: create a class, loop through registering cdev and creating device
    // Hint: class_create, cdev_init, cdev_add, device_create
    else
        return reg_result;
}

static void __exit dice_exit(void)
{
    // TODO: release all resources
    //  Hint: Pay attention to the order!
    //  Hint: device_destroy, cdev_del, class_destroy, kfree, unregister_chrdev_region
   cdev_del(&dice_devices[0].dice_cdev);
    cdev_del(&dice_devices[1].dice_cdev);
    cdev_del(&dice_devices[2].dice_cdev);
    unregister_chrdev_region(MKDEV(42, 0), DICE_DEVS);
}

static int dice_open(struct inode *inode, struct file *filp)
{
    // TODO: Find which dice is opened
    //  Hint: container_of, filp->private_data
    unsigned int temp = MINOR(inode->i_rdev);
    filp->private_data =  &dice_devices[temp];
    return 0;
}

static int dice_release(struct inode *inode, struct file *filp)
{
    // Do nothing here
    // If you allocate any resource in dice_open(), then you must release them here
    return 0;
}

static ssize_t dice_read(struct file *filp, char __user *buff, size_t count, loff_t *offp)
{
    // TODO: Generate dice patterns, generate random number
    //  Attention: handle count and offp carefully
    //  Hint: copy_to_user
    struct dice_dev *this;
    int rand, it, location = 0;
    char temp[512];
    int store[32];
    int current_type;
    this = (struct dice_dev *)filp->private_data;
    if (this->num > 32) current_type = 32;
    else current_type = this->num;

    if (this->dice_type == 1){
         for (it = 0; it < current_type; it++)
        {
            get_random_bytes(&rand, sizeof(int));
            location += snprintf(temp + location, max_str, "%s ", print_type2[((rand % 6) + 12) % 6]);
        }
        location += snprintf(temp + location, max_str, "\n");
    }
     else if (this->dice_type == 0){
        static const char print_type1[3][6][12] = {
    {"|     |  ", "|     |  ", "|  o  |  ", "| o o |  ", "| o o |  ", "| o o |  "},
    {"|  o  |  ", "| o o |  ", "|     |  ", "|     |  ", "|  o  |  ", "| o o |  "},
    {"|     |  ", "|     |  ", "| o o |  ", "| o o |  ", "| o o |  ", "| o o |  "}};
         for (it = 0; it < current_type; it++)
        {
            get_random_bytes(&store[it], sizeof(int));
            store[it] = ((store[it] % 6) + 12) % 6;
        }
        for (it = 0; it < current_type; it++)
            location =location + snprintf(temp + location, max_str, "-------  ");
        location += snprintf(temp + location, max_str, "\n");
        for (it = 0; it < current_type; it++)
            location =location+ snprintf(temp + location,max_str, "%s", print_type1[0][store[it]]);
        location += snprintf(temp + location, max_str, "\n");
        for (it = 0; it < current_type; it++)
            location =location+ snprintf(temp + location, max_str, "%s", print_type1[1][store[it]]);
        location += snprintf(temp + location, max_str, "\n");
        for (it = 0; it < current_type; it++)
            location =location+ snprintf(temp + location, max_str, "%s", print_type1[2][store[it]]);
        location += snprintf(temp + location, max_str, "\n");
        for (it = 0; it < current_type; it++)
            location =location + snprintf(temp + location, max_str, "-------  ");
        location += snprintf(temp + location, max_str, "\n");
    }
    else{
        for (it = 0; it < current_type; it++)
        {
            get_random_bytes(&rand, sizeof(int));
            rand = ((rand % gen_sides) + 2 * gen_sides) % gen_sides + 1;
            location += snprintf(temp + location, max_str, "%d ", rand);
        }
        location += snprintf(temp + location, max_str, "\n");
    }
    if (*offp >= location)
        return 0;
    if (*offp + count > location)
        count = location - *offp;
    if (copy_to_user(buff, temp + *offp, count) != 0)
        return -EINVAL;
    *offp += count;
    return count;
}
static ssize_t dice_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp)
{
    // TODO: Read in number of dice
    //  Attention: handle count and offp carefully
    //  Hint: copy_from_user
    char temp[max_str];
    struct dice_dev *this;
    long i = 0;
    this = (struct dice_dev *)filp->private_data;
    if (count > 16 || copy_from_user(temp, buff, count) != 0)
        return -EINVAL;
    temp[count - 1] = '\0';
    if (kstrtol(temp, 10, &i) != 0)
        return -EINVAL;
    dice_num = (int) i;
    this->num = dice_num;
    return count;
}
