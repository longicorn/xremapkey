#ifndef KEYBORD_H
#define KEYBORD_H

extern void send_key_event(int fd, int button, int value);
extern int handle_key_event(int fd, struct input_event* event);

#endif
