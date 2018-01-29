/*
 * Given a set of items, each with a mass and a value, determine the number
 * of each item to include in a collection so that the total weight is less
 * than or equal to a given limit and the total value is as large as 
 * possible.
 * So, assume there are n items: I1, I2, ..., In, each weight wi and value
 * vi, and there are ki Ii in the set.
 * When all ki equals to 0 or 1, the problem is called 0/1 Knapsack problem;
 * When all ki are unbounded, it is called unbounded Knapsack problem;
 * When some ki is large than 1, but not all ki are ubounded, it is called
 * multiple Knapsack problem.
 * 
 * So, you should implement a program to deals with all these situations
 * Input: num of items N and the maximum weight W, then there are N lines: 
 * each line has three elements: weight, value and num of this kind of item.
 * If the num equals to -1, it denote there are infinite numbers of this 
 * kind of item.
 * Output: the total value as large as possible
 */


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define ITEM_MAX 100
#define KANPSACKWGT_MAX 10000

void input(int, int *, int *, int *);
void init(int *, int, int *, int *, int *, int *);
void dp(int, int, int *, int *, int *, int *);
void output(int, int *);

int main(void)
{
	int num, max_wgt;
	int itemNum[ITEM_MAX], revenue[KANPSACKWGT_MAX];
	int wgt[ITEM_MAX], val[ITEM_MAX];

	while (scanf("%d %d", &num, &max_wgt) != EOF) {
		input(num, wgt, val, itemNum);
		init(&num, max_wgt, wgt, val, itemNum, revenue);
		dp(num, max_wgt, wgt, val, itemNum, revenue);
		output(max_wgt, revenue);
	}

	return 0;
}

void input(int num, int *wgt, int *val, int *itemNum)
{
	int i;

	for (i = 0; i < num; i++)
		scanf("%d %d %d", &wgt[i], &val[i], &itemNum[i]);
}

void init(int *num, int max_wgt, int *wgt, int *val, int *itemNum, int *revenue)
{
	int i, j;

	revenue[0] = 0;
	for (i = 1; i <= max_wgt; i++)
		revenue[i] = -INT_MAX;
	for (i = 0; i < (*num); i++) {
		if (itemNum[i] > 1) {
			if (itemNum[i] * wgt[i] >= max_wgt)
				itemNum[i] = -1;
			else {
				j = 1;
				while (itemNum[i] > j) {
					itemNum[i] -= j;
					wgt[*num] = j * wgt[i];
					val[*num] = j * val[j];
					itemNum[(*num)++] = 1;
					j *= 2;
				}
				wgt[i] *= itemNum[i];
				val[i] *= itemNum[i];
				itemNum[i] = 1;
			}
		}
	}
}

void dp(int num, int max_wgt, int *wgt, int *val, int *itemNum, int *revenue)
{
	int i, j;

	for (i = 0; i < num; i++) {
		if (itemNum[i] == 1) {
			for (j = max_wgt; j >= wgt[i]; j--)
				if (revenue[j] < revenue[j - wgt[i]] + val[i])
					revenue[j] = revenue[j - wgt[i]] + val[i];
		} else if (itemNum[i] == -1) {
			for (j = wgt[i]; j <= max_wgt; j++)
				if (revenue[j] < revenue[j - wgt[i]] + val[i])
					revenue[j] = revenue[j - wgt[i]] + val[i];
		}
	}
}

void output(int max_wgt, int *revenue)
{
	int i, max;

	max = -INT_MAX;
	for (i = max_wgt; i > 0; i--)
		if (revenue[i] > max)
			max = revenue[i];
	printf("The max revenue is %d.\n", max);
}
