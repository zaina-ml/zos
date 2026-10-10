#ifndef DRIVERS_INPUT_KEYBOARD_H
#define DRIVERS_INPUT_KEYBOARD_H

/* Initializes the legacy PS/2 controller and keyboard (QEMU/PC compatible). */
int keyboard_init(void);

/* Called only by the IRQ1 handler. */
void keyboard_handle_irq(void);

/* Nonblocking: returns 1 and stores a character, or returns 0 if empty. */
int keyboard_try_read(char *out);

#endif
