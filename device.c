#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <linux/uinput.h>


#define INPUT_DIR "/dev/input"
#define UINPUT_DEVICE1 "/dev/uinput"
#define UINPUT_DEVICE2 "/dev/input/uinput"

#define BITS_PER_LONG (sizeof(long) * 8)
#define NBITS(x) ((((x)-1)/BITS_PER_LONG)+1)
#define OFF(x)  ((x)%BITS_PER_LONG)
#define BIT(x)  (1UL<<OFF(x))
#define LONG(x) ((x)/BITS_PER_LONG)
#define test_bit(bit, array)	((array[LONG(bit)] >> OFF(bit)) & 1)

int open_keybord_device();
int open_uinput();
int test_key_device(char* dev_file);

int open_keybord_device()
{
  DIR* input_dir;
  struct dirent* entry;
  struct stat st;
  char dev_path[1024];
  int fd;

  input_dir = opendir(INPUT_DIR);
  if (input_dir == NULL) {
    perror("opendir");
    return -1;
  }

  while (1) {
    entry = readdir(input_dir);
    if (entry == NULL) {
      break;
    }

    snprintf(dev_path, sizeof(dev_path), "%s/%s", INPUT_DIR, entry->d_name);
    if (stat(dev_path, &st) == 0 && S_ISCHR(st.st_mode)) {
      if (test_key_device(dev_path) == 0) {
        fd = open(dev_path, O_RDONLY);
        if (fd > 0) {
          return fd;
        }
        close(fd);
      }
    }
  }

  return 0;
}

int open_uinput()
{
  struct stat st;
  if (stat(UINPUT_DEVICE1, &st) == 0 && S_ISCHR(st.st_mode)) {
    return open(UINPUT_DEVICE1, O_WRONLY);
  }
  if (stat(UINPUT_DEVICE2, &st) == 0 && S_ISCHR(st.st_mode)) {
    return open(UINPUT_DEVICE2, O_WRONLY);
  }
  return -1;
}

int test_key_device(char* dev_file)
{
  int fd;

  fd = open(dev_file, O_RDONLY);
  if (fd < 0) {
    perror("Failed to open device file.");
    return -1;
  }

  unsigned long bit[NBITS(KEY_MAX)];
  memset(bit, 0, sizeof(bit));
  ioctl(fd, EVIOCGBIT(EV_KEY, KEY_MAX), bit);
  close(fd);

  if (test_bit(KEY_E, bit) &&
      test_bit(KEY_P, bit) &&
      test_bit(KEY_I, bit) &&
      test_bit(KEY_A, bit) &&
      test_bit(KEY_N, bit) &&
      test_bit(KEY_Q, bit) &&
      test_bit(KEY_ENTER, bit)) {
    return 0;
  }

  return -1;
}
