#ifndef CAMERA_H_
#define CAMERA_H_
#include "include.h"

void img_extract(uint8 dist[][CAMERA_W], uint8 src[CAMERA_SIZE], uint32 srclen);
void vcan_sendimg(uint8 imgaddr[][CAMERA_W], uint32 height, uint32 width);

#endif
