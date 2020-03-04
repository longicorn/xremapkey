#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <linux/input.h>

#include "device.h"
#include "keybord.h"

void event_loop(int kb_fd, int uinput_fd);

int main(int argc, char **argv)
{
  int kb_fd, uinput_fd;

  kb_fd = open_keybord_device();
  if (kb_fd == 0) {
    printf("error\n");
    return 1;
  }

  uinput_fd = open_uinput();
  if (uinput_fd == 0) {
    printf("error\n");
    return 1;
  }
  init_input_device(uinput_fd);

  event_loop(kb_fd, uinput_fd);

  close(kb_fd);
  close(uinput_fd);

  return 0;
}

int get_current_window_pid()
{
  char *str = "xdotool getwindowfocus getwindowpid";
  char buf[256];
  FILE *fp;
  
  fp = popen(str, "r");
  if (fp == NULL) {
    return 0;
  }
  fgets(buf, 256, fp);
  pclose(fp);

  return atoi(buf);
}

void event_loop(int kb_fd, int uinput_fd)
{
  struct input_event event;
  fd_set rfds;
  int retval;

  while(1) {
    FD_ZERO(&rfds);
    FD_SET(kb_fd, &rfds);

    retval = select(kb_fd+1, &rfds, NULL, NULL, NULL);
    if (retval == -1) {
      break;
    }
    else if (retval) {
      read(kb_fd, &event, sizeof(event));
      handle_key_event(uinput_fd, &event);
    }
    else {
      continue;
    }
  }
}
