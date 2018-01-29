/*
 * Given a rod of length n inches and a table of prices pi for length di,
 * determine the maximum revenue rn obtainable by cutting up the rod.
 * Input: num of different rod
 *		  length of these rod
 *		  price of these rod
 *		  length of rod to cut
 *		  (next case...)
 *		  EOF
 * output: the maximum revenue and how to cut the rod
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define INPUT_MAX 100
#define LEN_MAX 1000

void input(int , int *, int *, int *);
void dp(int, int, int *, int *, int *, int *);
void output(int, int, int *, int *, int *);

int main()
{
	int num, lgh;
	int pri[INPUT_MAX], len[INPUT_MAX], rcd[LEN_MAX + 1], count[LEN_MAX + 1];

	while (scanf("%d", &num) != EOF) {
		input(num, &lgh, pri, len);
		dp(num, lgh, pri, len, rcd, count);
		output(num, lgh, len, rcd, count);
	}

	return 0;
}

void input(int num, int *lgh, int *pri, int *len)
{
	int i;

	for (i = 0; i < num; i++)
		scanf("%d", &len[i]);
	for (i = 0; i < num; i++)
		scanf("%d", &pri[i]);
	scanf("%d", lgh);
}

/*
 * c[l] = max{c[l - li] + pi, c[l]}
 */
void dp(int num, int lgh, int *pri, int *len, int *rcd, int *count)
{
	int i, j, k;

	count[0] = 0;
	rcd[0] = 0;
	for (i = 1; i <= lgh; i++)
		count[i] = -INT_MAX;

/*
 * Solution 1: Add a (li, pi) each time
	for (i = 0; i < num; i++)
		for (j = len[i]; j <= lgh; j++)
			if (count[j - len[i]] + pri[i] > count[j]) {
				count[j] = count[j - len[i]] + pri[i];
				rcd[j] = len[i];
			}
*/


/*
 * Solution 2: Add the length of rod to cut with 1 each time
 */
 	for (i = 1; i <= lgh; i++)
		for (j = 0; j < num; j++)
			if (i >= len[j] && count[i - len[j]] + pri[j] > count[i]) {
				count[i] = count[i - len[j]] + pri[j];
				rcd[i] = len[j];
			}
}

void output(int num, int lgh, int *len, int *rcd, int *count)
{
	int i, j;
	int pos, ans;

	pos = 0;
	ans = count[pos];
	for (i = 1; i <= lgh; i++)
		if (count[i] > ans) {
			ans = count[i];
			pos = i;
		}

	printf("The maximum revenue of length %d is %d, %d is obsoleted!\n",lgh,  ans, lgh - pos);
	printf("The rod was cut into: ");
	while (pos > 0) {
		printf("%d ", rcd[pos]);
		pos -= rcd[pos];
	}
	printf("\n");
}
