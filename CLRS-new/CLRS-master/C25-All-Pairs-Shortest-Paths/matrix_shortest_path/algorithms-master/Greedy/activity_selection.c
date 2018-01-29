/*
 * Suppose we have a set S = {a1, a2, ..., an} of proposed activities
 * that wish to use a resource. Each activity ai has a start time Si
 * and a finish time Fi. If selected, activity ai takes place during
 * the half open time interval [Si, Fi). We wish to select a maximum
 * size subset of mutually compatible activities.
 * Input: the number of activities N
 *		  and then N lines, each contain a Si and a Fi
 * Output: the maximum size subset of mutually compatible activities
 * 		   and their No.
 */

#include <stdio.h>
#include <stdlib.h>

#define AVT_MAX 100

struct Activity {
	int no;
	int start_time;
	int end_time;
	char choosed;
};

struct AVTs {
	int activity_num;
	struct Activity activity[AVT_MAX];
};

extern void myqSort(void *base, size_t num, size_t width, \
					int64_t (*cmp)(const void *, const void *));
void initAVTs(struct AVTs *, int);
void input(struct AVTs *);
void choose_avt(struct AVTs *);
void output(struct AVTs *);


int main(void)
{
	int num;
	struct AVTs avts;

	while (scanf("%d", &num) != EOF) {
		initAVTs(&avts, num);
		input(&avts);
		choose_avt(&avts);
		output(&avts);
	}

	return 0;
}

void initAVTs(struct AVTs *avts, int num)
{
	int i;

	avts->activity_num = num;
	for (i = 0; i < avts->activity_num; i++) {
		avts->activity[i].no = i;
		avts->activity[i].choosed = 'n';
	}
}

void input(struct AVTs *avts)
{
	int i;
	int start, end;

	for (i = 0; i < avts->activity_num; i++) {
		scanf("%d %d", &start, &end);
		avts->activity[i].start_time = start;
		avts->activity[i].end_time = end;
	}
}

int64_t compare(const void *elem1, const void *elem2)
{
	return ((struct Activity *)elem1)->end_time - \
			((struct Activity *)elem2)->end_time;
}
	
void choose_avt(struct AVTs *avts)
{
	int i;
	int time;

	myqSort(avts->activity, avts->activity_num, \
			sizeof(struct Activity), compare);
	for (i = 0, time = 0; i < avts->activity_num; i++) {
		if (time <= avts->activity[i].start_time) {
			time = avts->activity[i].end_time;
			avts->activity[i].choosed = 'y';
		}
	}
}

void output(struct AVTs *avts)
{
	int i;

	for (i = 0; i < avts->activity_num; i++)
		if (avts->activity[i].choosed == 'y')
			printf("choose acticity %d, from %d to %d\n", \
					avts->activity[i].no + 1, avts->activity[i].start_time, \
					avts->activity[i].end_time);
}
