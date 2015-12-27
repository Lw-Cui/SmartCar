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

/*!
 *  @brief		计数器
 *  @param		prev			前驱节点数组
 *  @param		end				边线最后一点
 *  @return		路径节点数目
 *  @since      v1.0
 */
uint16 boundary_traversal(uint8 img[][CAMERA_W], Point prev[][CAMERA_W], Point end) {
	if (empty(end))
		return 0;
	uint16 cnt = 0;
	for (; !equal(prev[end.x][end.y], end); end = prev[end.x][end.y]) {
		cnt++;
#ifdef _SEND_
		img[end.x][end.y] = 150;
#endif
	}
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
uint8 is_same_color(uint8 img[][CAMERA_W], Point p1, Point p2) {
	return img[p1.x][p1.y] == img[p2.x][p2.y];
}

#define QLEN 250
#define LENGTH 8
Point search(uint8 img[][CAMERA_W], Point position, Point prev[][CAMERA_W]) {
	static int8 XTRAV[LENGTH] = { 1,  0, -1,  1, -1,  1,  0, -1};
	static int8 YTRAV[LENGTH] = {-1, -1, -1,  0,  0,  1,  1,  1};
	Point queue[QLEN];
	uint16 beg = 0, end = 0;

	set(&prev[position.x][position.y], position.x, position.y);
	queue[end++] = position;

	while (beg != end) {
		Point p = queue[beg++];
		beg %= QLEN;

		for (uint8 i = 0; i < LENGTH; i++) {
			Point np; 

			set(&np, p.x + XTRAV[i], p.y + YTRAV[i]);
			if (is_valid(np) && !visit(prev, np) && !is_same_color(img, np, p)) {
				prev[np.x][np.y] = p;

				queue[end++] = np;
				end %= QLEN;

				if (terminal(np) && abs(np.x - position.x) > 5)
					return np;
			}
		}
	}

	set(&position, EMPTY, EMPTY);
	return position;
}

/*!
 *  @brief      将数组逆序
 *  @param		new_dir		目标数组
 *  @param		len			数组长度
 *  @since      v1.0
 */
void reverse(Point new_dir[], int len) {
	for (int i = 0; i < len / 2; i++)
		swap(&new_dir[i], &new_dir[len - i - 1]);
}

/*!
 *  @brief      边界
 *  @param      img				二维图像数组
 *  @param      prev			前驱数组
 *  @param		s, b			两条边界的终点
 *  @since      v1.0
 */
int16 mid_line(uint8 img[][CAMERA_W], Point prev[][CAMERA_W], Point new_dir[], Point s, Point b) {
	double ratio = (double)boundary_traversal(img, prev, b) / boundary_traversal(img, prev, s);
	if (ratio < 1.0) {
		swap(&s, &b);
		ratio = 1 / ratio;
	}

	double cnt = 1.0;
	int bpos = 0, end = 0;
	while (!equal(prev[b.x][b.y], b) && !equal(prev[s.x][s.y], s)) {
		cnt += ratio;
		for (; bpos < (int)cnt; bpos++) {
			set(&new_dir[end++], (s.x + b.x) / 2, (s.y + b.y) / 2);
			b = prev[b.x][b.y];
		}
		s = prev[s.x][s.y];
	}
	reverse(new_dir, end);
#ifdef _SEND_
	for (int i = 0; i != end / 2; i++)
		img[new_dir[i].x][new_dir[i].y] = 100;
#endif
	return end;
}

/*!
 *  @brief      人工边界
 *  @param      prev	回溯数组
 *  @param      start	起始点
 *  @param      end		终点
 *  @since      v1.0
 */
void add_boundary(Point prev[][CAMERA_W], Point *start, Point *end, int8 ypos) {
	set(start, CAMERA_H - 1, ypos);
	set(end, CAMERA_H - 2, ypos);
	prev[end->x][end->y] = prev[start->x][start->y] = *start;
}

/*!
 *  @brief      左右边线查找
 *  @param      img			图像数组
 *  @param      prev		图像回溯
 *  @param      startp		起始指针
 *  @param      endp		终点指针
 *  @since      v2.0
 */
void find_left_boundary(uint8 img[][CAMERA_W], Point prev[][CAMERA_W], Point *startp, Point *endp) {
	while (startp->x > CAMERA_H / 2) {
		*endp = search(img, *startp, prev);
		if (empty(*endp)) {
			if (startp->y != 0)
				startp->y -= 1;
			else
				startp->x--;
		}
	}
	if (empty(*endp))
		add_boundary(prev, startp, endp, 0);
}

void find_right_boundary(uint8 img[][CAMERA_W], Point prev[][CAMERA_W], Point *startp, Point *endp) {
	while (startp->x > CAMERA_H / 2) {
		*endp = search(img, *startp, prev);
		if (empty(*endp)) {
			if (startp->y != CAMERA_W - 1)
				startp->y += 1;
			else
				startp->x--;
		}
	}
	if (empty(*endp))
		add_boundary(prev, startp, endp, CAMERA_W - 1);
}

/*!
 *  @brief      遍历，找到边界
 *  @param      img				图像数组
 *  @since      v2.0
 */
uint8 direction(uint8 img[][CAMERA_W], Point new_dir[CAMERA_W]) {
	Point prev[CAMERA_H][CAMERA_W];
	memset(prev, EMPTY, sizeof(prev)); //EMPTY -1

	Point left_start = {CAMERA_H - 1, CAMERA_W / 2}, right_start = {CAMERA_H - 1, CAMERA_W / 2};
	Point left_end = {EMPTY, EMPTY}, right_end = {EMPTY, EMPTY};

	find_left_boundary(img, prev, &left_start, &left_end);
	find_right_boundary(img, prev, &right_start, &right_end);

	return mid_line(img, prev, new_dir, left_end, right_end);
}
