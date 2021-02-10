#include <stdio.h>

typedef enum 
{
	FALSE=0,
	TRUE=1
} bool;

typedef enum
{
	MONDAY,
	TUESDAY,
	WEDNESDAY,
	THURSDAY,
	FRIDAY,
	SATURDAY,
	SUNDAY
} day;

int main()
{
	day week_day= FRIDAY;
	bool check=FALSE;
	if(week_day==SATURDAY)
		check=TRUE;

	printf("It's day: %d my dude!!\n",week_day);
	if(check==TRUE)
		printf("Weeken!!!\n");
	else
		printf(";{\n");
	return 0;
}

