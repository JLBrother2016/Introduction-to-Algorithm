/*
 * We are given a sequence K = <k1, k2, ..., kn> of n distinct
 * keys in sorted order, and we wish to build a binary search 
 * tree from these keys. For each key ki, we have a probabilty
 * pi that a search will be for ki. Some searchs may be for 
 * values not in K, and so we also have n+1 "dummy keys" d0, d1
 * ... dn. For each dummy key di, it represents all values 
 * between di and di+1(if existed), we have a probability qi 
 * that a search will correspond to di.
 * Input: num of keys
 *		  n keys and their probability
 *		  n+1 dummy keys and their probability
 * Output: Minimum average nodes visited in a search
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define KEY_MAX 100

void input(int, float *, float *);
void initArr(int, float *, float *, \
		float (*cost)[KEY_MAX], float (*weight)[KEY_MAX]);
void dp(int, float *, float *, int (*choice)[KEY_MAX], \
		float (*cost)[KEY_MAX], float (*weight)[KEY_MAX]);
void output(int, float (*cost)[KEY_MAX], int (*choice)[KEY_MAX]);
float getCost(int, int, float (*cost)[KEY_MAX], float *);

int main(void)
{
	int num;
	int choice[KEY_MAX][KEY_MAX];
	float cost[KEY_MAX][KEY_MAX], weight[KEY_MAX][KEY_MAX]; 
	float keypbt[KEY_MAX], dumKeypbt[KEY_MAX + 1];

	while (scanf("%d", &num) != EOF) {
		input(num, keypbt, dumKeypbt);
		initArr(num, keypbt, dumKeypbt, cost, weight);
		dp(num, keypbt, dumKeypbt, choice, cost, weight);
		output(num, cost, choice);
	}

	return 0;
}

void input(int num, float *keypbt, float *dumKeypbt)
{
	int i;

	for (i = 0; i < num; i++)
		scanf("%f", &keypbt[i]);
	for (i = 0; i <= num; i++)
		scanf("%f", &dumKeypbt[i]);
}

void initArr(int num, float *keypbt, float *dumKeypbt, \
			float (*cost)[KEY_MAX], float (*weight)[KEY_MAX])
{
	int i, j;

	for (i = 0; i < num; i++) {
			weight[i][i] = keypbt[i] + dumKeypbt[i] + dumKeypbt[i + 1];
			cost[i][i] = INT_MAX;
		for (j = i + 1; j < num; j++) {
			weight[i][j] = weight[i][j - 1] + keypbt[j] + dumKeypbt[j + 1];
			cost[i][j] = INT_MAX;
		}
	}
}

/*
 * cost[i][j] = min(i <= k <= j){cost[i][k - 1] + cost[k + 1][j] + weight[i, j]}
 * Pay Attention to this formula when k = i and k = j
 * cost[i][i - 1] = dumKeypbt[i] and cost[j + 1][j] = dumKeypbt[j + 1]
 *
 * According to 15.5-4, Knuth has shown that there are always roots of 
 * optimal subtrees such that choice[i, j-1] <= choice[i, j] <= 
 * choice[i+1, j]. Use this method, we can modify the procedure to run
 * in O(n*n).
 */
void dp(int num, float *keypbt, float *dumKeypbt, int (*choice)[KEY_MAX] \
		, float (*cost)[KEY_MAX], float (*weight)[KEY_MAX])
{
	int i, j, l, k;
	int low, high;

	for (l = 0; l < num; l++)
		for (i = 0; i < num - l; i++) {
			j = i + l;
			if (j - 1 >= i)
				low = choice[i][j - 1];
			else
				low = i;
			if (j >= i + 1)
				high = choice[i + 1][j];
			else
				high = j;
			for (k = low; k <= high; k++)
//			for (k = i; k <= j; k++)
				if (cost[i][j] > (getCost(i, k - 1, cost, dumKeypbt) + \
						getCost(k + 1, j, cost, dumKeypbt) + weight[i][j])) {
					cost[i][j] = (getCost(i, k - 1, cost, dumKeypbt) + \
						getCost(k + 1, j, cost, dumKeypbt) + weight[i][j]);
					choice[i][j] = k;
				}
		}
}

float getCost(int low, int high, float (*cost)[KEY_MAX], float *dumKeypbt)
{
	return low > high ? dumKeypbt[low] : cost[low][high];
}

void output(int num, float (*cost)[KEY_MAX], int (*choice)[KEY_MAX])
{
	int i, j, l;

	printf("The average miminum number of nodes visited per search \
		is %f\n", cost[0][num - 1]);
	printf("The cost matrix:\n");
	for (l = num - 1; l >= 0; l--) {
		for (j = 0; j < l; j++)
			printf("\t");
		for (i = 0; i < num - l; i++) {
			j = i + l;
			printf("%16.2f", cost[i][j]);
		}
		printf("\n");
	}
	printf("The choice matrix:\n");
	for (l = num - 1; l >= 0; l--) {
		for (j = 0; j < l; j++)
			printf("\t");
		for (i = 0; i < num - l; i++) {
			j = i + l;
			printf("%16d", choice[i][j]);
		}
		printf("\n");
	}
}
