#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/keyboard.h>
#include <linux/moduleparam.h>
#include <linux/input.h>
#include <linux/printk.h>

#define BUF_LEN (PAGE_SIZE << 2)
#define US 0
#define HEX 1
#define DEC 2

#define WINDOWS_V_VERSION "1.0"

static int codes;

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Lucas Zordan Nunes <lucaszordan0204@gmail.com>");
MODULE_VERSION(WINDOWS_V_VERSION);
MODULE_DESCRIPTION("Windows + v feature in linux xD");

module_param(codes, int, 0644);
MODULE_PARM_DESC(codes, "log format (0:US keys (default), 1:hex keycodes, 2:dec keycodes)");

//static char keys_buf[BUF_LEN];

//static ssize_t read_keys(struct file *filp, char *buffer, size_t len, loff_t *offset);

//static int spy_keyboard_cb(struct notifier_block *nblock, unsigned long code, void *_param);

//static struct notifier_block intercept = {
//    .notifier_call = spy_keyboard_cb,
//}

//static void read_keys() {
//    return;
//}

//int spy_keyboard_cb(struct notifier_block *nblock, unsigned long code, void *_param) {
//    // code to verify if windows + v is pressed
//    return NOTIFY_OK;
//}

static int __init init_windows_v(void){
    pr_info("Hello, World!\n");

    if (codes < 0 || codes > 2) {
        return -EINVAL;
    }

//    register_keyboard_notifier();
    return 0;
}

static void __exit exit_windows_v(void) {
    pr_info("Goodbye");
//    unregister_keyboard_notifier();
}

module_init(init_windows_v);
module_exit(exit_windows_v);
