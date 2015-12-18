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

#define LEFTMOST 325
#define RIGHTMOST 550
#define MID 425
/*!
 *  @brief      确定方向
 *  @param      new_dir		中线数组
 *  @param      len			数组长度
 *  @since      v1.0
 */
int16 get_offset(Point start, Point end) {
	double tanx = (double)abs(start.y - end.y) / (double)abs(start.x - start.x);
	double tan30 = sqrt(3) / 3;
	tanx = fmax(tanx, tan30);

	if (end.y < start.y)
		return tanx / tan30 * 100 * -1;
	else
		return tanx / tan30 * 100;
}

/*!
 *  @brief      确定方向
 *  @param      new_dir		中线数组
 *  @param      len			数组长度
 *  @since      v1.0
 *	@reference
		tpm_pwm_duty(TPM1,TPM_CH0,320);
		330 - 425 - 550 int 舵机占空比

		tpm_pwm_duty(TPM0,TPM_CH0,50);	
		tpm_pwm_duty(TPM0,TPM_CH1, 0);
 */
#define THRESHOLD 20
void direction(Point new_dir[], int16 len) {
	static Point start = {EMPTY, EMPTY}, end = {EMPTY, EMPTY};

	if (len > THRESHOLD || empty(start) || empty(end)) {
		set(&start, new_dir[len - 1].x, (new_dir[len - 1].y + CAMERA_W / 2) / 2);

		int end_x = new_dir[len / 4].x * 0.1 + new_dir[len / 2].x * 0.6 + new_dir[len * 3 / 4].x * 0.3;
		int end_y = new_dir[len / 4].y * 0.1 + new_dir[len / 2].y * 0.6 + new_dir[len * 3 / 4].y * 0.3;
		set(&end, end_x, end_y);
	}

	int16 offset = get_offset(start, end);
	tpm_pwm_duty(TPM1,TPM_CH1, MID + offset);

	tpm_pwm_duty(TPM0,TPM_CH0, 50);
	tpm_pwm_duty(TPM0,TPM_CH1,0);
}
