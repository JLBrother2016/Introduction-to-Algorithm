/*
 * An implemention of heap sort.
 * This program can be use to implement priority queue.
 * @ base: base address of heap
 * @ num: number of elements
 * @ width: size of each elements
 * @ cmp: a compare function, if return val(elem1) - val(elem2)
 * F heapSort: sort function
 * F extractTop: extract and return the top elem
 * F addElement: add a new element
 * F adjustPosition: when an element at postion pos changes its value, \
 *					 use this function to ajust its position
 * F heapify: a shift-down function
 * F heapiup: a shift-up function
 * F swap: exchange two element
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "macros.h"

#define PRT(a) (((a) - 1) / 2)
#define LEF(a) ((a) * 2 + 1)
#define RIG(a) ((a) * 2 + 2)

void heapSort(void *base, size_t num, size_t width, \
				int64_t (*cmp)(const void *, const void *));
void *extractTop(void *base, size_t *num, size_t width, \
				int64_t (*cmp)(const void *, const void *));
void addElement(void *base, size_t *num, size_t width, void *value, \
				int64_t (*cmp)(const void *, const void *));
void adjustPosition(void *base, size_t num, size_t width, size_t pos,\
				int64_t (*cmp)(const void *, const void *));
void heapiup(void *base, size_t num, size_t width, size_t pos, \
				int64_t (*cmp)(const void *, const void *));
void heapify(void *base, size_t num, size_t width, size_t pos, \
				int64_t (*cmp)(const void *, const void *));
static void swap(void *, void *, size_t);
				

void heapSort(void *base, size_t num, size_t width, \
				int64_t (*cmp)(const void *, const void *))
{
	int i;

	for (i = PRT(num - 1); i >= 0; i--)
		heapify(base, num, width, i, cmp);
}

void *extractTop(void *base, size_t *num, size_t width, \
				int64_t (*cmp)(const void *, const void *))
{
	swap((char *)base, (char *)base + width * (--(*num)), width);
	heapify(base, *num, width, 0, cmp);
	return (char *)base + width * (*num);
}

void adjustPostion(void *base, size_t num, size_t width, size_t pos,\
				int64_t (*cmp)(const void *, const void *))
{
	heapify(base, num, width, pos, cmp);
	heapiup(base, num, width, pos, cmp);
}

void heapiup(void *base, size_t num, size_t width, size_t pos, \
			int64_t (*cmp)(const void *, const void *))
{
	size_t ptmp;

	while (pos > 0) {
		ptmp = PRT(pos);
		if (cmp((char *)base + width * pos, \
				(char *)base + width * ptmp) <= 0) 
			return ;
		swap((char *)base + width * pos, (char *)base + width * ptmp, width);
		pos = ptmp;
	}
}

void heapify(void *base, size_t num, size_t width, size_t pos, \
			int64_t (*cmp)(const void *, const void *))
{
	size_t ctmp;

	while (pos <= PRT(num - 1)) {
		if (LEF(pos) + 1 > num)
			return ;
		ctmp = LEF(pos);
		if (RIG(pos) < num && cmp((char *)base + width * ctmp, \
									(char *)base + width * (ctmp + 1)) < 0)
			ctmp++;
		if (cmp((char *)base + width * pos, \
				(char *)base + width * ctmp) >= 0)
			return ;
		swap((char *)base + width * pos, (char *)base + width * ctmp, width);
		pos = ctmp;
	}
}

static void swap(void *add1, void *add2, size_t width)
{
	size_t i;
	char c;

	for (i = 0; i < width; i++) {
		c = *((char *)add1 + i);
		*((char *)add1 + i) = *((char *)add2 + i);
		*((char *)add2 + i) = c;
	}
}
	
