// db_dim_server.c : Defines the entry point for the console application.
//
// Dietrich Beck
//
// This is a DIM test server.
#include <stdio.h>
#include <sys/timeb.h>
#include <time.h>
#include <dis.h>

int service_id1;
double data[8];
char buff[128];

int i=-1;

void main()
{
	service_id1 = dis_add_service ("bla1", "I:1", &i, 4, 0, 0);
	dis_start_serving("DIS_TESTsdfsdf");
	printf("service created\n");
	sleep(5);
	printf("start\n");
	for (i=0; i< 10000; i++)
	{
		dis_update_service(service_id1);
	}
	printf("stop\n");
	dis_stop_serving();
	sleep(5);
}