#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/input.h>
#include <linux/uinput.h>

void send_key_event(int fd, int button, int value);
int handle_key_event(int fd, struct input_event* event);

void send_key_event(int fd, int button, int value)
{
  struct input_event event;
  gettimeofday(&event.time, NULL);
  event.type = EV_KEY;
  event.code = button;
  event.value = value;
  write(fd, &event, sizeof(event));

  event.type = EV_SYN;
  event.code = SYN_REPORT;
  event.value = 0;
  write(fd, &event, sizeof(event));
}

int handle_key_event(int fd, struct input_event* event)
{
  switch(event->code) {
    case KEY_H:
      send_key_event(fd, BTN_LEFT, event->value);
      break;
    case KEY_J:
      for(int i=0; i<10; i++) {
        send_key_event(fd, KEY_DOWN, event->value);
      }
      break;
    case KEY_K:
      for(int i=0; i<10; i++) {
        send_key_event(fd, KEY_UP, event->value);
      }
      break;
    case KEY_L:
      send_key_event(fd, BTN_RIGHT, event->value);
      break;
    default:
      break;
  }

  return 0;
}
