#ifndef RGBCMD_H
#define RGBCMD_H

typedef enum {
  RGB_MODE_ON,
  RGB_MODE_OFF,
  RGB_MODE_BLINK,
  RGB_MODE_RAINBOW,
  RGB_MODE_RANDOM,
} rgbcmd_mode_t;

typedef struct rgbcmd {
  rgbcmd_mode_t mode;
  unsigned char red;
  unsigned char green;
  unsigned char blue;
} rgbcmd_t;

rgbcmd_t *open_rgbcmd(int need_to_create);

void rgbcmd_json(rgbcmd_t *cmd, char *buffer, size_t length);

#endif
