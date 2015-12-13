#include "point.h"

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

uint8 empty(Point p) {
	return p.x == EMPTY && p.y == EMPTY;
}

uint8 visit(Point prev[][CAMERA_W], Point p) {
	return !empty(prev[p.x][p.y]);
}

uint8 equal(Point p1, Point p2) {
	return p1.x == p2.x && p1.y == p2.y;
}

void swap(Point *p1, Point *p2) {
	Point tmp = *p1;
	*p1 = *p2;
	*p2 = tmp;
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

