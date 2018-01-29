/*
 * We are given a sequence chain <A1, A2, ..., An> of n matrices to be 
 * multiplied. What are the minimum scalar multiplications to compute 
 * the matrices chain?
 * Input: l0, l1, l2, ..., ln (matrix k is a (lk-1 * lk) matrix)
 * Output: The mimimum scalar multiplications to compute and how
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define INPUT_MAX 100

void input(int, int *);
void init(int, int (*rcd)[INPUT_MAX], int (*count)[INPUT_MAX]);
void dp(int, int *, int (*rcd)[INPUT_MAX], int (*count)[INPUT_MAX]);
void output(int, int (*rcd)[INPUT_MAX], int (*count)[INPUT_MAX]);
void countParent(int, int, int (*rcd)[INPUT_MAX], int (*parenthesize)[2]);
void outputParent(int, int);

int main(void)
{
	int num;
	int arr[INPUT_MAX + 1], rcd[INPUT_MAX][INPUT_MAX], count[INPUT_MAX][INPUT_MAX];

	while (scanf("%d", &num) != EOF) {
		input(num, arr);
		init(num, rcd, count);
		dp(num, arr, rcd, count);
		output(num, rcd, count);
	}

	return 0;
}

void input(int num, int *arr)
{
	int i;

	for (i = 0; i <= num; i++)
		scanf("%d", &arr[i]);
}

void init(int num, int (*rcd)[INPUT_MAX], int (*count)[INPUT_MAX])
{
	int i, j;

	for (i = 0; i < num; i++) {
		for (j = i + 1; j < num; j++)
			count[i][j] = INT_MAX;
		count[i][i] = 0;
		rcd[i][i] = i;
	}
}

/*
 * c[i, j] = min(i + 1 <= k <= j){c[i, k] + c[k + 1, j] + li * lk+1 * lj+1}
 */
void dp(int num, int *arr, int (*rcd)[INPUT_MAX], int (*count)[INPUT_MAX])
{
	int i, j, k, l;

	for (l = 1; l < num; l++)
		for (i = 0; i < num - l; i++) {
			j = i + l;
			for (k = i + 1; k <= j; k++)
				if (count[i][j] > count[i][k - 1] + count[k][j] + arr[i] * arr[k] * arr[j + 1]) {
					count[i][j] = count[i][k - 1] + count[k][j] + arr[i] * arr[k] * arr[j + 1];
					rcd[i][j] = k;
				}
		}
}

void output(int num, int (*rcd)[INPUT_MAX], int (*count)[INPUT_MAX])
{
	int i;
	int parenthesize[INPUT_MAX + 1][2];

	for (i = 0; i <= num; i++)
		parenthesize[i][0] = parenthesize[i][1] = 0;

	printf("The minimum number of scalar multiplication is %d\n", count[0][num - 1]);
	countParent(0, num - 1, rcd, parenthesize);
	outputParent(0, parenthesize[0][0]);
	outputParent(1, parenthesize[0][1]);
	for (i = 0; i < num; i++) {
		printf("M%d", i);
		outputParent(0, parenthesize[i + 1][0]);
		outputParent(1, parenthesize[i + 1][1]);
	}
}

void countParent(int low, int high, int (*rcd)[INPUT_MAX], int (*parenthesize)[2])
{
	int pos;
	
	if (low >= high)
		return ;
	pos = rcd[low][high];
	if (pos > low + 1) {
		parenthesize[low][1]++;
		parenthesize[pos][0]++;
	}	
	if (high > pos) {
		parenthesize[pos][1]++;
		parenthesize[high + 1][0]++;
	}
//	parenthesize[low][1]++;
//	parenthesize[high + 1][0]++;

	countParent(low, pos - 1, rcd, parenthesize);
	countParent(pos, high, rcd, parenthesize);
}

/*
 * type   0:(    1:)
 */
void outputParent(int type, int num)
{
	int i;

	if (type)
		for (i = 0; i < num; i++)
			printf("(");
	else
		for (i = 0; i < num; i++)
			printf(")");
}
