#include "db.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

void fun(){
	MYSQL *con = mysql_init(NULL);
	init_db_parameter(&para);
	db_connect(con, &para);
	printf("db connected.\n");
	MYSQL_RES *res;
	MYSQL_ROW row;

	if (mysql_query(con, "show tables")) {
		fprintf(stderr, "%s\n", mysql_error(con));
		exit(1);
	}
	
	res = mysql_use_result(con);
	
	/* output table name */
	printf("MySQL Tables in mysql database:\n");
	while ((row = mysql_fetch_row(res)) != NULL)
		printf("%s \n", row[0]);
	
	/* close conection */
	mysql_free_result(res);

	mysql_close(con);
	mysql_thread_end();
}

int main(){
	int r;
	mysql_library_init(0, NULL, NULL);
	pthread_t tid1;
	if((r = pthread_create(&tid1, NULL, (void *)&fun, NULL)) != 0){
		printf("create thread 1 failed\n.");
		exit(0);
	}

#if 1
	pthread_t tid2;
	if((r = pthread_create(&tid2, NULL, (void *)&fun, NULL)) != 0){
		printf("create thread 2 failed.\n");
		exit(0);
	}
#endif
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	printf("threads ended.\n");
	mysql_library_end();
	return 0;
}
