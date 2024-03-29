/* 
	The MIT License (MIT)

	Copyright (c) 2015 lw

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#include "common.h"
#include "point.h"
#include "camera.h"
#include "control.h"
#include "MKL_it.h"
#include "math.h"


/*
 *  @brief      底边修正
 *  @param      
 *  @param      len			数组长度
 *  @since      v1.0
 */
int16 bottom_amendment(uint8 img[][CAMERA_W]) {
	int left = 0;
	int right = CAMERA_W - 1;
	for (int i = 0; i < CAMERA_W / 2; i++) {
		if (img[CAMERA_H - 1][CAMERA_W / 2 - i] == BLACK && left == 0)
			left = CAMERA_W / 2 - i;
		if (img[CAMERA_H - 1][CAMERA_W / 2 + i] == BLACK && right == CAMERA_W - 1)
			right = CAMERA_W / 2 + i;
	}
	return (right + left) / 2 - CAMERA_W / 2;
}

/*
 *  @brief      边线修正
 *  @param      
 *  @param      len			数组长度
 *  @since      v1.0
 */
#define MAX_LEN 120
#define MIN_LEN 20
int16 boundary_amendment(Point new_dir[], int len) {
	if (len < MIN_LEN) return 0;
	int offset = 0;

	if (len > MAX_LEN) {
		offset = new_dir[MAX_LEN / 2].y - CAMERA_W / 2;
	} else if (len < MAX_LEN) {
		offset = new_dir[len / 2].y - CAMERA_W / 2;
		offset *= ((double)MAX_LEN / len);
	}
	return offset;
}


/*
#define K1 2
#define K2 2.3
*/

#define K1 2
#define K2 2.3
#define K3 2.3
int16 get_offset(uint8 img[][CAMERA_W], Point new_dir[], int len) {
	int boundary = boundary_amendment(new_dir, len);
	int botton = bottom_amendment(img);

	if (botton * boundary > 0)
		return boundary * K1 + botton * K2;
	else
		return boundary * K3;
}

/*!
 *  @brief      确定速度
 *  @param      offset		偏移量
 *  @param      velocity	当前速度
 *  @since      v1.0
 */
/*
int16 get_velocity(int16 offset, int16 velocity) {
	static int v_max = 150, v_min = 50;
	static int v_max_cnt = 0, v_min_cnt = 0;

	if (abs(offset) < 50) {
		if (velocity < v_max)
			velocity += 5;
		else
			v_max_cnt++;
	} else {
		if (velocity > v_min)
			velocity -= 5;
		else 
			v_min_cnt++;
	}

#define HIT 10
	if (v_max_cnt > HIT) {
		v_max += 10;
		v_max_cnt = 0;
	} else if (v_min_cnt > HIT) {
		v_min -= 10;
		v_min_cnt = 0;
	}
}
*/
#define VINIT 120
int16 get_velocity() {
	return VINIT;
}
