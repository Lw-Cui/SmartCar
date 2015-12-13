#ifndef _POINT_H_
#define _POINT_H_
#include "include.h"
#define EMPTY -1

typedef struct {
	int16 x, y;
} Point;

void copy(Point *dist, Point *src);
void set(Point *p, int nx, int ny);
uint8 empty(Point p);
uint8 is_valid(Point p);
uint8 equal(Point p1, Point p2);
void swap(Point *p1, Point *p2);

#endif

