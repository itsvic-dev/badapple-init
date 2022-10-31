#include "load.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <stdio.h>

const int TARGET_H = 48;

const char *ditherChars = " .*|#";
const int ditherLength = 5;

const char *frames[6572];

void process_frame(int i, unsigned char *data, int w, int h, int channels) {
  int TARGET_W = TARGET_H * (w / h);
  int widthOffset = w / TARGET_W;
  int heightOffset = h / TARGET_H;
  int rowSize = TARGET_W * 2 + 1;
  char *string = calloc(rowSize * TARGET_H + 1, 1);

  for (int y = 0; y < TARGET_H; y++) {
    for (int x = 0; x < TARGET_W; x++) {
      int trueX = x * widthOffset;
      int trueY = y * heightOffset;
      int color = data[(trueY * w + trueX) * channels];
      char c = ditherChars[color / (255 / (ditherLength - 1))];
      string[y * rowSize + x * 2] = c;
      string[y * rowSize + x * 2 + 1] = c;
    }
    string[y * rowSize + rowSize - 1] = '\n';
  }
  frames[i] = string;
}

void load_frame(int frame) {
  // load the frame
  char framePath[17];
  sprintf(framePath, "frames/%04d.png", frame);

  int w;
  int h;
  int channels;
  unsigned char *data = stbi_load(framePath, &w, &h, &channels, 0);

  process_frame(frame, data, w, h, channels);

  stbi_image_free(data);
}

void show_frame(int frame) {
  putchar('\033');
  putchar('c');
  puts(frames[frame]);
}
