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

/*!
 *  @brief      判断是否到边界
 *  @param      p 需要判断的点
 *  @return     是否？
 *  @since      v1.0
 */
uint8 terminal(Point p) {
	return p.x == 0 || p.y == 0 || p.y == CAMERA_W - 1;
}

uint8 is_valid(Point p) {
	return (p.x >= 0 && p.x < CAMERA_H && p.y >= 0 && p.y < CAMERA_W);
}

uint8 is_same_color(uint8 img[][CAMERA_W], Point p1, Point p2) {
	return img[p1.x][p1.y] == img[p2.x][p2.y];
}

uint8 empty(Point p) {
	return p.x == EMPTY && p.y == EMPTY;
}

uint8 visit(Point prev[][CAMERA_W], Point p) {
	return !empty(prev[p.x][p.y]);
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

#define LEN 8
int8 XTRAV[LEN] = { 1,  0, -1,  1, -1,  1,  0, -1};
int8 YTRAV[LEN] = {-1, -1, -1,  0,  0,  1,  1,  1};

/* Vertical line */
void vertical_line(uint8 img[][CAMERA_W]) {
	int pos = 0;
	while (pos < CAMERA_H) {
		img[pos][CAMERA_W / 2] = 127;
		pos++;
	}
}

/*!
 *  @brief		边线上色函数
 *  @param      img				二维图像数组
 *  @since      v5.0            img_extract(img, imgbuff,CAMERA_SIZE);
 *  Sample usage:
 */
uint8 gray_boundary(uint8 img[][CAMERA_W], Point prev[][CAMERA_W], Point end) {
	for (; !equal(prev[end.x][end.y], end); end = prev[end.x][end.y])
		img[end.x][end.y] = 150;
	return 1;
}

/*!
 *  @brief      查找中线
 *  @param      img		图像数据
 *  @param      prev	边线回溯
 *  @param      position	起始位置
 *  @return     边线的最后一点
 *  @since      v1.0
 */
#define QLEN 250
	Point queue[QLEN];
Point processing(uint8 img[][CAMERA_W], Point position, Point prev[][CAMERA_W]) {
	uint16 beg = 0, end = 0;

	set(&prev[position.x][position.y], position.x, position.y);
	queue[end++] = position;

	while (beg != end) {
		Point p = queue[beg++];
		beg %= QLEN;

		for (uint8 i = 0; i < LEN; i++) {
			Point np; 

			if (np.y == 59)
				printf("");

			set(&np, p.x + XTRAV[i], p.y + YTRAV[i]);
			if (is_valid(np) && !visit(prev, np) && !is_same_color(img, np, p)) {
				prev[np.x][np.y] = p;

				queue[end++] = np;
				end %= QLEN;

				if (terminal(np))
					return np;
			}
		}
	}

	set(&position, EMPTY, EMPTY);
	return position;
}

/*!
 *  @brief      遍历最近一行，找到边界
 *  @param      img				图像数组
 *  @param      left_prev, right_prev		记录边界的数组
 *  @since      v1.0
 */
void traversal(uint8 img[][CAMERA_W]) {
	Point left_end = {EMPTY, EMPTY}, right_end = {EMPTY, EMPTY};
	Point pos = {CAMERA_H - 1, 0};
	Point end;
	Point prev[CAMERA_H][CAMERA_W];

	for (int i = 0; i < CAMERA_H; i++)
		for (int j = 0; j < CAMERA_W; j++)
			set(&prev[i][j], EMPTY, EMPTY);

	  while (pos.y < CAMERA_W) {
		end =  processing(img, pos, prev);
		if (!empty(end))
			gray_boundary(img, prev, end);
		 pos.y++;
	}

}

