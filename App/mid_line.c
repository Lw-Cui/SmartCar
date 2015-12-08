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
#include "mid_line.h"
#define LEN 8

/*!
 *  @brief      判断是否到边界
 *  @param      p 需要判断的点
 *  @return     是否？
 *  @since      v1.0
 */
uint8 terminal(Point p) {
	return (p.x == 0 || p.y == 0 || p.y == CAMERA_W);
}

uint8 is_valid(Point p) {
	return (p.x >= 0 && p.x < CAMERA_H && p.y >= 0 && p.y < CAMERA_W);
}

uint8 is_same_color(uint8 imgaddr[][CAMERA_W], Point p1, Point p2) {
	return (imgaddr[p1.x][p1.y] == imgaddr[p2.x][p2.y]);
}

uint8 empty(Point p) {
	return p.x == EMPTY && p.y == EMPTY;
}

uint8 equal(Point p1, Point p2) {
	return p1.x == p2.x && p1.y == p2.y;
}

/*!
 *  @brief      Point 设置函数
 *  @param      ...
 *  @since      v1.0
 */
void copy(Point *dist, Point *src) {
	dist->x = src->x;
	dist->y = src->y;
}

void set(Point *p, int nx, int ny) {
	p->x = nx;
	p->y = ny;
}

int8 XTRAV[LEN] = { 1,  0, -1,  1, -1,  1,  0, -1};
int8 YTRAV[LEN] = {-1, -1, -1,  0,  0,  1,  1,  1};

/* Vertical line */
void vertical_line(uint8 imgaddr[][CAMERA_W]) {
	int pos = 0;
	while (pos < CAMERA_H) {
		imgaddr[pos][CAMERA_W / 2] = 127;
		pos++;
	}
}

/*!
 *  @brief		边线上色函数
 *  @param      img				二维图像数组
 *  @since      v5.0            img_extract(img, imgbuff,CAMERA_SIZE);
 *  Sample usage:
 */

#define SUITABLE 20

uint8 gray_boundary(uint8 img[][CAMERA_W], Point prev[][CAMERA_W], Point end) {
	uint16 cnt = 0;

	for (Point b = end; equal(prev[b.x][b.y], b); b = prev[b.x][b.y])
		cnt++;
	
	if (cnt > SUITABLE)
		for (; equal(prev[end.x][end.y], end); end = prev[end.x][end.y])
			img[end.x][end.y] = 128;
	else
		return 0;

	return 1;
}

/*!
 *  @brief      查找中线
 *  @param      imgaddr		图像数据
 *  @param      prev	边线回溯
 *  @param      position	起始位置
 *  @return     边线的最后一点
 *  @since      v1.0
 */
#define QLEN 250
Point processing(uint8 imgaddr[][CAMERA_W], Point position, Point prev[][CAMERA_W]) {
	Point queue[QLEN];
	uint16 beg = 0, end = 0;

	set(&prev[position.x][position.y], position.x, position.y);
	queue[end++] = position;

	while (beg != end) {
		Point p = queue[beg++];
		beg %= QLEN;

		for (uint8 i = 0; i < LEN; i++) {
			Point np; 

			set(&np, p.x + XTRAV[i], p.y + YTRAV[i]);
			if (is_valid(np) && empty(np) && !is_same_color(imgaddr, np, p)) {
				prev[np.x][np.y] = p;

				queue[end++] = np;
				end %= QLEN;

				if (end == beg || terminal(np)) return np;
			}
		}
	}

	set(&position, EMPTY, EMPTY);
	return position;
}
