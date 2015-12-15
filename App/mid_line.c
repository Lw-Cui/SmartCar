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
#include "camera.h"
#include "point.h"


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


uint8 is_same_color(uint8 img[][CAMERA_W], Point p1, Point p2) {
	return img[p1.x][p1.y] == img[p2.x][p2.y];
}
/*!
 *  @brief		边线上色函数
 *  @param      img				二维图像数组
 *  @param		prev			前驱节点数组
 *  @param		end				边线最后一点
 *  @since      v1.0
 */
void gray_boundary(uint8 img[][CAMERA_W], Point prev[][CAMERA_W], Point end) {
	for (; !equal(prev[end.x][end.y], end); end = prev[end.x][end.y])
		img[end.x][end.y] = 150;
}

/*!
 *  @brief		计数器
 *  @param		prev			前驱节点数组
 *  @param		end				边线最后一点
 *  @return		路径节点数目
 *  @since      v1.0
 */
uint16 boundary_count(Point prev[][CAMERA_W], Point end) {
	uint16 cnt = 0;
	for (; !equal(prev[end.x][end.y], end); end = prev[end.x][end.y])
		cnt++;
	return cnt;
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
 *  @brief      绘制边界
 *  @param      img				二维图像数组
 *  @param      prev			前驱数组
 *  @param		s, b			两条边界起始点
 *  @since      v1.0
 */
void draw_mid_line(uint8 img[][CAMERA_W], Point prev[][CAMERA_W], Point s, Point b) {
	int lenb = boundary_count(prev, b);
	int lens = boundary_count(prev, s);
	double ratio = (double)boundary_count(prev, b) / (double)boundary_count(prev, s);

	if (ratio < 1.0) {
		swap(&s, &b);
		ratio = 1 / ratio;
	}

	double cnt = 1.0;
	int bpos = 0;
	while (!equal(prev[b.x][b.y], b) && !equal(prev[s.x][s.y], s)) {
		img[(s.x + b.x) / 2][(s.y + b.y) / 2] = 100;

		cnt += ratio;
		for (; bpos < (int)cnt; bpos++) {
			b = prev[b.x][b.y];
			img[(s.x + b.x) / 2][(s.y + b.y) / 2] = 100;
		}

		s = prev[s.x][s.y];
	}
	/*
	while (!equal(prev[b.x][b.y], b) && !equal(prev[s.x][s.y], s)) {
		for (uint8 j = 0; j < (int)(ratio + 1); j++) {
			img[(s.x + b.x) / 2][(s.y + b.y) / 2] = 100;
			b = prev[b.x][b.y];
		}

		s = prev[s.x][s.y];
	}
	*/
}

/*!
 *  @brief      遍历最近一行，找到边界
 *  @param      img				图像数组
 *  @since      v2.0
 */
void traversal(uint8 img[][CAMERA_W]) {
	Point prev[CAMERA_H][CAMERA_W];

	for (int i = 0; i < CAMERA_H; i++)
		for (int j = 0; j < CAMERA_W; j++)
			set(&prev[i][j], EMPTY, EMPTY);

	Point left_start = {CAMERA_H - 1, CAMERA_W / 2};
	Point right_start = {CAMERA_H - 1, CAMERA_W / 2};
	Point left_end = {EMPTY, EMPTY}, right_end = {EMPTY, EMPTY};

	while (is_valid(left_start) && is_valid(right_start)) {

		if (empty(left_end)) {
			left_end =  processing(img, left_start, prev);
			if (!empty(left_end)) {
				gray_boundary(img, prev, left_end);
			} else {
				left_start.y--;
			}
		}

		if (empty(right_end)) {
			right_end = processing(img, right_start, prev);
			if (!empty(right_end)) {
				gray_boundary(img, prev, right_end);
			} else {
				right_start.y++;
			}
		}

		if (!empty(right_end) && !empty(left_end)) {

			vcan_sendimg(img, CAMERA_H, CAMERA_W);                  //发送解压后的图像数据

			draw_mid_line(img, prev, left_end, right_end);
			break;
		}

	}
}
