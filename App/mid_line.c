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
static int8 XTRAV[LEN] = { 1,  0, -1,  1, -1,  1,  0, -1};
static int8 YTRAV[LEN] = {-1, -1, -1,  0,  0,  1,  1,  1};

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
 *  @brief		������ɫ����
 *  @param      img				��άͼ������
 *  @param		prev			ǰ���ڵ�����
 *  @param		end				�������һ��
 *  @since      v1.0
 */
void gray_boundary(uint8 img[][CAMERA_W], Point prev[][CAMERA_W], Point end) {
	for (; !equal(prev[end.x][end.y], end); end = prev[end.x][end.y])
		img[end.x][end.y] = 150;
}

/*!
 *  @brief		������
 *  @param		prev			ǰ���ڵ�����
 *  @param		end				�������һ��
 *  @return		·���ڵ���Ŀ
 *  @since      v1.0
 */
uint16 boundary_count(Point prev[][CAMERA_W], Point end) {
	uint16 cnt = 0;
	for (; !equal(prev[end.x][end.y], end); end = prev[end.x][end.y])
		cnt++;
	return cnt;
}

/*!
 *  @brief      ��������
 *  @param      img		ͼ������
 *  @param      prev	���߻���
 *  @param      position	��ʼλ��
 *  @return     ���ߵ����һ��
 *  @since      v1.0
 */

Point search(uint8 img[][CAMERA_W], Point position, Point prev[][CAMERA_W]) {
#define QLEN 250
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
 *  @brief      �߽�
 *  @param      img				��άͼ������
 *  @param      prev			ǰ������
 *  @param		s, b			�����߽���յ�
 *  @since      v1.0
 */
int16 find_mid_line(uint8 img[][CAMERA_W], Point prev[][CAMERA_W], Point new_dir[], Point s, Point b) {
	double ratio = (double)boundary_count(prev, b) / (double)boundary_count(prev, s);

	if (ratio < 1.0) {
		swap(&s, &b);
		ratio = 1 / ratio;
	}

	double cnt = 1.0;
	int bpos = 0;
	int16 end = 0;

	while (!equal(prev[b.x][b.y], b) && !equal(prev[s.x][s.y], s)) {
		cnt += ratio;
		for (; bpos < (int)cnt; bpos++) {
#ifdef _DEBUG_
			img[(s.x + b.x) / 2][(s.y + b.y) / 2] = 100;
#endif
			set(&new_dir[end++], (s.x + b.x) / 2, (s.y + b.y) / 2);
			b = prev[b.x][b.y];
		}
		s = prev[s.x][s.y];
	}

	return end;
}


/*!
 *  @brief      �������һ�У��ҵ��߽�
 *  @param      img				ͼ������
 *  @since      v2.0
 */
uint8 traversal(uint8 img[][CAMERA_W], Point new_dir[CAMERA_W]) {
	Point prev[CAMERA_H][CAMERA_W];

	for (int i = 0; i < CAMERA_H; i++)
		for (int j = 0; j < CAMERA_W; j++)
			set(&prev[i][j], EMPTY, EMPTY);

	Point left_start = {CAMERA_H - 1, CAMERA_W / 2};
	Point right_start = {CAMERA_H - 1, CAMERA_W / 2};
	Point left_end = {EMPTY, EMPTY}, right_end = {EMPTY, EMPTY};

	while (is_valid(left_start) && is_valid(right_start)) {

		if (empty(left_end)) {
			left_end =  search(img, left_start, prev);
			if (!empty(left_end)) {
				gray_boundary(img, prev, left_end);
			} else {
				left_start.y--;
			}
		}

		if (empty(right_end)) {
			right_end = search(img, right_start, prev);
			if (!empty(right_end)) {
				gray_boundary(img, prev, right_end);
			} else {
				right_start.y++;
			}
		}

		if (!empty(right_end) && !empty(left_end)) {

#ifdef _DEBUG_
			vcan_sendimg(img, CAMERA_H, CAMERA_W);                  //���ͽ�ѹ���ͼ������
#endif
			return find_mid_line(img, prev, new_dir, left_end, right_end);
		}
	}
	return 0;
}
