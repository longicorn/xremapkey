#ifndef DEVICE_H
#define DEVICE_H

extern int open_keybord_device();
extern int open_uinput();
extern void init_input_device(int fd);

#endif
