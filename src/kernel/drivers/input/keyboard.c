#include <drivers/input/keyboard.h>
#include <arch/x86_64/io.h>
#include <stdint.h>

/* One ISR producer, one foreground consumer. Must be power of two. */
#define KEY_QUEUE_SIZE 128
static char queue[KEY_QUEUE_SIZE];
static uint8_t queue_head;
static uint8_t queue_tail;

static uint8_t left_shift;
static uint8_t right_shift;
static uint8_t caps_lock;
static uint8_t extended_prefix;

/* US QWERTY, translated PS/2 scancode set 1. */
static const char unshifted[128] = {
    [0x02] = '1', [0x03] = '2', [0x04] = '3', [0x05] = '4',
    [0x06] = '5', [0x07] = '6', [0x08] = '7', [0x09] = '8',
    [0x0A] = '9', [0x0B] = '0', [0x0C] = '-', [0x0D] = '=',
    [0x0E] = '\b', [0x0F] = '\t',
    [0x10] = 'q', [0x11] = 'w', [0x12] = 'e', [0x13] = 'r',
    [0x14] = 't', [0x15] = 'y', [0x16] = 'u', [0x17] = 'i',
    [0x18] = 'o', [0x19] = 'p', [0x1A] = '[', [0x1B] = ']',
    [0x1C] = '\n',
    [0x1E] = 'a', [0x1F] = 's', [0x20] = 'd', [0x21] = 'f',
    [0x22] = 'g', [0x23] = 'h', [0x24] = 'j', [0x25] = 'k',
    [0x26] = 'l', [0x27] = ';', [0x28] = '\'', [0x29] = '`',
    [0x2B] = '\\',
    [0x2C] = 'z', [0x2D] = 'x', [0x2E] = 'c', [0x2F] = 'v',
    [0x30] = 'b', [0x31] = 'n', [0x32] = 'm', [0x33] = ',',
    [0x34] = '.', [0x35] = '/', [0x39] = ' '
};

static const char shifted[128] = {
    [0x02] = '!', [0x03] = '@', [0x04] = '#', [0x05] = '$',
    [0x06] = '%', [0x07] = '^', [0x08] = '&', [0x09] = '*',
    [0x0A] = '(', [0x0B] = ')', [0x0C] = '_', [0x0D] = '+',
    [0x1A] = '{', [0x1B] = '}', [0x27] = ':', [0x28] = '"',
    [0x29] = '~', [0x2B] = '|', [0x33] = '<', [0x34] = '>',
    [0x35] = '?'
};

static void queue_push(char value)
{
    uint8_t head = __atomic_load_n(&queue_head, __ATOMIC_RELAXED);
    uint8_t next = (uint8_t)((head + 1u) & (KEY_QUEUE_SIZE - 1));
    if (next == __atomic_load_n(&queue_tail, __ATOMIC_ACQUIRE))
        return; /* full: drop new character */

    queue[head] = value;
    __atomic_store_n(&queue_head, next, __ATOMIC_RELEASE);
}

int keyboard_try_read(char *out)
{
    if (!out)
        return 0;

    uint8_t tail = __atomic_load_n(&queue_tail, __ATOMIC_RELAXED);
    if (tail == __atomic_load_n(&queue_head, __ATOMIC_ACQUIRE))
        return 0;

    *out = queue[tail];
    __atomic_store_n(&queue_tail,
                     (uint8_t)((tail + 1u) & (KEY_QUEUE_SIZE - 1)),
                     __ATOMIC_RELEASE);
    return 1;
}

void keyboard_handle_irq(void)
{
    uint8_t code = inb(0x60);

    if (code == 0xE0 || code == 0xE1) {
        extended_prefix = 1;
        return;
    }
    if (extended_prefix) {
        extended_prefix = 0;
        return; /* arrows and other extended keys not supported yet */
    }

    if (code == 0x2A) { left_shift = 1; return; }
    if (code == 0x36) { right_shift = 1; return; }
    if (code == 0xAA) { left_shift = 0; return; }
    if (code == 0xB6) { right_shift = 0; return; }
    if (code & 0x80)
        return; /* ignore ordinary key releases */
    if (code == 0x3A) { caps_lock ^= 1; return; }

    char ch = unshifted[code];
    uint8_t is_shifted = left_shift || right_shift;
    if (ch >= 'a' && ch <= 'z') {
        if (is_shifted ^ caps_lock)
            ch = (char)(ch - 'a' + 'A');
    } else if (is_shifted && shifted[code]) {
        ch = shifted[code];
    }
    if (ch)
        queue_push(ch);
}

static int controller_wait_write(void)
{
    for (int i = 0; i < 100000; ++i) {
        if (!(inb(0x64) & 0x02))
            return 1;
    }
    return 0;
}

static int controller_wait_read(void)
{
    for (int i = 0; i < 100000; ++i) {
        if (inb(0x64) & 0x01)
            return 1;
    }
    return 0;
}

static int controller_command(uint8_t command)
{
    if (!controller_wait_write())
        return 0;
    outb(0x64, command);
    return 1;
}

static int controller_data(uint8_t data)
{
    if (!controller_wait_write())
        return 0;
    outb(0x60, data);
    return 1;
}

int keyboard_init(void)
{
    /* Minimal legacy i8042 setup: first port only, translated set 1. */
    if (!controller_command(0xAD) || !controller_command(0xA7))
        return 0;

    /* Discard stale data before reading the controller's config byte. */
    for (int i = 0; i < 32 && (inb(0x64) & 0x01); ++i)
        (void)inb(0x60);

    if (!controller_command(0x20) || !controller_wait_read())
        return 0;
    uint8_t config = inb(0x60);
    config = (uint8_t)((config | 0x40) & (uint8_t)~0x13);
    /* Translation on, port 1 clock on, both port IRQs off during init. */
    if (!controller_command(0x60) || !controller_data(config))
        return 0;
    if (!controller_command(0xAE))
        return 0;

    /* Tell the keyboard to start sending scancodes; consume its ACK. */
    if (!controller_data(0xF4) || !controller_wait_read())
        return 0;
    if (inb(0x60) != 0xFA)
        return 0;

    /* Enable the first PS/2 port's interrupt line at the controller. */
    config |= 0x01;
    if (!controller_command(0x60) || !controller_data(config))
        return 0;
    return 1;
}
