#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/keyboard.h>
#include <linux/moduleparam.h>
// #include <linux/debugfs.h>
#include <linux/input.h>

#define BUF_LEN (PAGE_SIZE << 2)
#define US 0
#define HEX 1
#define DEC 2

#define WINDOWS_V_VERSION "1.0"

static int codes;

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Lucas Zordan Nunes <lucaszordan0204@gmail.com>");
MODULE_VERSION(WINDOWS_V_VERSION);
MODULE_DESCRIPTION("Windows + v featura in linux xD");

module_param(codes, int, 0644);
MODULE_PARM_DESC(codes, "log format (0:US keys (default), 1:hex keycodes, 2:dec keycodes)");

static struct dentry *subdir;
static char keys_buf[BUF_LEN];

static ssize_t read_keys(struct file *filp, char *buffer, size_t len, loff_t *offset);

static int spy_keyboard_cb(struct notifier_block *nblock, unsigned long code, void *_param);

static struct notifier_block intercept = {
    .notifier_call = spy_keyboard_cb,
}

const struct file_operations keys_fops = {
    .owner = THIS_MODULE,
    .read = read_keys
};

static ssize_t read_keys(struct file *filp, char *buffer, size_t len, loff_t *offset) {
    return simple_read_from_buffer(buffer, len, offset, keys_buf, buf_pos);
}

int spy_keyboard_cb(struct notifier_block *nblock, unsigned long code, void *_param) {
    // code to verify if windows + v is pressed
    return NOTIFY_OK;
}

static int __init init_windows_v(void){
    printf("Hello, World!\n");

    if (codes < 0 || codes > 2) {
        return -EINVAL;
    }

    register_keyboard_notifier();
    return 0;
}

static void __exit exit_windows_v(void) {
    unregister_keyboard_notifier();
}

module_init(init_windows_v);
module_exit(exit_windows_v);
