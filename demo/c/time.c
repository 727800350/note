#include <stdio.h>
#include <time.h>
#include <mysql.h>

int main(void){
	struct tm *dt;
	time_t lt;
	lt =time(NULL);
	printf("time in seconds: %ld.\n", lt);
	
// 	software time
	dt=localtime(&lt);
	printf("localtime: %d-%d-%d %d:%d:%d\n", dt->tm_year + 1900, dt->tm_mon + 1, dt->tm_mday, dt->tm_hour, dt->tm_min, dt->tm_sec);

// 	hardware time
	dt=gmtime(&lt);
	printf("gmtime: %d-%d-%d %d:%d:%d\n", dt->tm_year + 1900, dt->tm_mon + 1, dt->tm_mday, dt->tm_hour, dt->tm_min, dt->tm_sec);

	MYSQL_TIME mt;
	mt.year = dt->tm_year + 1900;    
	mt.month = dt->tm_mon + 1;    
	mt.day = dt->tm_mday;    
	mt.hour = dt->tm_hour;    
	mt.minute = dt->tm_min;
	mt.second = dt->tm_sec;
	printf("mysql time: %d-%d-%d %d:%d:%d\n", mt.year, mt.month, mt.day, mt.hour, mt.minute, mt.second);
	return 0;
}
