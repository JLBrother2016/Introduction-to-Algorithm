/*
 * An implemention of quick sort.
 * @para: base: base address of the elements for sorting
 *		  num: numbers of elements
 *		  width: size of each element
 *		  comp: compare function of element
 * After sort, these element will arrange in an ascending order
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "macros.h"

static int64_t __compare_by_point(const void *elem1, const void *elem2);
void quickSort(void *base, size_t low, size_t high, size_t width,\
				int64_t (*cmp)(const void *, const void *));
size_t partition(void *base, size_t low, size_t high, size_t width,\
				int64_t (*cmp)(const void *, const void *));


void myqSort(void *base, size_t num, size_t width,\
				int64_t (*comp)(const void *, const void *))
{
	if (!comp)
		comp = __compare_by_point;

	quickSort(base, 0, num - 1, width, comp);
}

static int64_t __compare_by_point(const void *elem1, const void *elem2)
{
	return elem1 - elem2;
}

void quickSort(void *base, size_t low, size_t high, size_t width,\
				int64_t (*cmp)(const void *, const void *))
{
	size_t pos;

	if (low >= high)
		return ;
	pos = partition(base, low, high, width, cmp);
	if (pos > 0)
		quickSort(base, low, pos - 1, width, cmp);
	quickSort(base, pos + 1, high, width, cmp);
}

size_t partition(void *base, size_t low, size_t high, size_t width,\
					int64_t (*cmp)(const void *, const void *))
{
	size_t i, j;
	void *tmp;

	i = low;
	j = high;
	if (!(tmp = (char *)malloc(width)))
		log(E, "malloc failed.");
	memcpy(tmp, (char *)base + width * i, width);
	while (i < j) {
		while (i < j && cmp((char *)base + width * j, tmp) >= 0)
			j--;
		memcpy((char *)base + width * i, (char *)base + width * j, width);
		while (i < j && cmp((char *)base + width * i, tmp) <= 0)
			i++;
		memcpy((char *)base + width * j, (char *)base + width * i, width);
	}
	memcpy((char *)base + width * i, tmp, width);
	free(tmp);
	return i;
}
