#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/keyboard.h>
#include <linux/moduleparam.h>
#include <linux/debugfs.h>
#include <linux/input.h>

const struct file_operations keys_fops = {
    .owner = THIS_MODULE,
    .read = read_keys
};

static ssize_t read_keys() {

}

static int __init init_module(void){
    printf("Hello, World!\n");
    return 0;
}
