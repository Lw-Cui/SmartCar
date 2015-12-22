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
#include "control.h"
#include "MKL_it.h"
#include "math.h"

/*!
 *  @brief      确定方向
 *  @param      new_dir		中线数组
 *  @param      len			数组长度
 *  @since      v1.0
 */
/*
int16 get_offset(Point start, Point end) {
	double tanx = (double)abs(start.y - end.y) / (double)abs(start.x - end.x);
	double tan30 = sqrt(3) / 3;
	if (tanx > tan30)
		tanx = tan30;

	if (end.y < start.y)
		return tanx / tan30 * 70 * -1;
	else
		return tanx / tan30 * 70;
}
*/
int16 get_offset(Point start, Point end) {
	double k = 4.3;
	int16 offset = (end.y - start.y) * k;
	if (offset > 0)
		return fmin(70, offset);
	else
		return fmax(-70, offset);
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
int16 get_velocity(int16 offset, int16 velocity) {
#define VINIT 170
	return VINIT;
}

/*!
 *  @brief      确定方向
 *  @param      new_dir		中线数组
 *  @param      len			数组长度
 *  @since      v1.0
 *	@reference
		tpm_pwm_duty(TPM1,TPM_CH0,320);
		舵机占空比 int 272 - 342 - 412

		电机占空比 int 1000
		tpm_pwm_duty(TPM0,TPM_CH0,50);	
		tpm_pwm_duty(TPM0,TPM_CH1, 0);
 */

void direction(Point new_dir[], int16 len) {
	static Point start = {EMPTY, EMPTY}, end = {EMPTY, EMPTY};
	static int velocity = VINIT;
	int offset = 0;
	
#define LEN 20
	if (len > LEN || empty(start) || empty(end)) {
		set(&start, new_dir[len - 1].x, (new_dir[len - 1].y + CAMERA_W / 2) / 2);

		int end_x = new_dir[len / 4].x * 0.3 + new_dir[len / 2].x * 0.7;
		int end_y = new_dir[len / 4].y * 0.3 + new_dir[len / 2].y * 0.7;
		set(&end, end_x, end_y);

		offset = get_offset(start, end);
		velocity = get_velocity(offset, velocity);
	}
	tpm_pwm_duty(TPM1,TPM_CH0, MID + offset);

#ifdef _DEBUG_
	tpm_pwm_duty(TPM0,TPM_CH0, 0);
#else
	tpm_pwm_duty(TPM0,TPM_CH0, velocity);
#endif
	tpm_pwm_duty(TPM0,TPM_CH1,0);
}
