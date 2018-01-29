/*
 * Given two sequence X, Y, find lonest common subsequence of them.
 * Input: Two sequence X, Y
 * Output: The longest common subsequence
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN_MAX 100
#define max(a, b) ((a) > (b) ? (a) : (b))

void dp(char *, char *, char (*rcd)[LEN_MAX], int (*count)[LEN_MAX]);
void output(char *, char *, char (*rcd)[LEN_MAX], int (*count)[LEN_MAX]);
void printLCS(char *, char *, int, int, char (*rcd)[LEN_MAX]);

int main()
{
	int count[LEN_MAX][LEN_MAX];
	char str1[LEN_MAX], str2[LEN_MAX];
	char rcd[LEN_MAX][LEN_MAX];

	scanf("%s", str1);
	scanf("%s", str2);
	dp(str1, str2, rcd, count);
	output(str1, str2, rcd, count);

	return 0;
}

/*
 * if str1[i] = str2[j] common(str1[0..i], str2[0..j]) = common(str1[0..i-1], str2[0..j-1]) + 1
 * else common(str1[0..i], str2[0..j]) 
 *		= max(common(str1[0..i], str2[0..j-1]), common(str1[0..i-1],common(str2[0..j])))
 *
 * If there are limitation about memory usage, we can use count[2][LEN_MAX] or even count[LEN_MAX]
 * instead of count[LEN_MAX][LEN_MAX], pay attention to the initialization of count[j][0],
 * there should initilize in the correct position instead of the current place
 */
void dp(char *str1, char *str2, char (*rcd)[LEN_MAX], int (*count)[LEN_MAX])
{
	int i, j;
	int len1, len2;

	len1 = strlen(str1);
	len2 = strlen(str2);
	if (str1[0] == str2[0]) {
		count[0][0] = 1;
		rcd[0][0] = 'S';
	} else {
		count[0][0] = 0;
		rcd[0][0] = 'U';
	}
	for (i = 1; i < len1; i++) {
		count[i][0] = max(count[i - 1][0], (str1[i] == str2[0]));
		if (str1[i] == str2[0])
			rcd[i][0] = 'S';
		else if (count[i][0])
			rcd[i][0] = 'U';
		else
			rcd[i][0] = 'L';
	}
	for (j = 1; j < len2; j++) {
		count[0][j] = max(count[0][j - 1], (str1[0] == str2[j]));
		if (str1[0] == str2[j])
			rcd[0][j] = 'S';
		else if (count[0][j])
			rcd[0][j] = 'L';
		else
			rcd[0][j] = 'U';
	}

	for (i = 1; i < len1; i++)
		for (j = 1; j < len2; j++)
			if (str1[i] == str2[j]) {
				count[i][j] = count[i - 1][j - 1] + 1;
				rcd[i][j] = 'S';
			} else if (count[i - 1][j] > count[i][j - 1]) {
				count[i][j] = count[i - 1][j];
				rcd[i][j] = 'U';
			} else {
				count[i][j] = count[i][j - 1];
				rcd[i][j] = 'L';
			}
}

void output(char *str1, char *str2, char (*rcd)[LEN_MAX], int (*count)[LEN_MAX])
{
	int len1, len2;

	len1 = strlen(str1);
	len2 = strlen(str2);

	printf("%d: ", count[len1 - 1][len2 - 1]);
	printLCS(str1, str2, len1 - 1, len2 - 1, rcd);
	printf("\n");
}

void printLCS(char *str1, char *str2, int x, int y, char (*rcd)[LEN_MAX])
{
	if (x < 0 || y < 0)
		return ;
	if (rcd[x][y] == 'S') {
		printLCS(str1, str2, x - 1, y - 1, rcd);
		printf("%c", str1[x]);
	} else if (rcd[x][y] == 'U') {
		printLCS(str1, str2, x - 1, y, rcd);
	} else {
		printLCS(str1, str2, x, y - 1, rcd);
	}
}
