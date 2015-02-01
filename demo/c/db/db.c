// Created by Eric Wang 2014.04.10
// Database manipulations

#include "db.h" 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

db_t para;

void init_db_parameter(db_t *db_para){
	db_para->host = "localhost";
	db_para->port = 0;
	db_para->db = "test";
	db_para->user = "test";
	db_para->passwd = "123456";
	return;
}

MYSQL *db_connect(MYSQL *con, db_t *db_para){
	// the initialization of con can not be here
	if (mysql_real_connect(con, db_para->host, db_para->user, db_para->passwd, db_para->db, db_para->port, NULL, 0) == NULL){
		printf("%s\n", mysql_error(con));
		exit(0);
	}
	return con;
}

int db_query(MYSQL *con, const char *stmt_str){
	int ret;
	if ((ret = mysql_query(con, stmt_str)) != 0){
		printf("%s\n", mysql_error(con));
		exit(0);
	}
	return ret;
}

MYSQL_STMT *db_stmt_prepare(MYSQL_STMT *stmt, MYSQL *con, const char *sql, const int para_num){
// 	init the statement
	if((stmt = mysql_stmt_init(con)) == NULL){
		printf("Mysql statement initialization failed.\n");
		exit(0);
	}
// 	combine the statement with the sql
	if(mysql_stmt_prepare(stmt, sql, strlen(sql))){
	  printf("mysql_stmt_prepare(), INSERT failed\n");
	  printf("%s", mysql_stmt_error(stmt));
	  exit(0);
	}

// 	ensure the parameter number is correct
	if(mysql_stmt_param_count(stmt) != para_num){
		printf("invalid parameter count returned by MySQL\n");
		exit(0);
	}
	return stmt;
}

void db_stmt_bind_para(MYSQL_STMT *stmt, MYSQL_BIND *binds){
	if(mysql_stmt_bind_param(stmt, binds)){
	  printf("mysql_stmt_bind_param() failed\n");
	  printf("%s\n", mysql_stmt_error(stmt));
	  exit(0);
	}
}

void db_execute(MYSQL_STMT *stmt){
	if(mysql_stmt_execute(stmt)){
		printf("mysql_stmt_execute() failed\n");
		printf("%s\n", mysql_stmt_error(stmt));
		exit(0);
	}
}

void db_close(MYSQL_STMT *stmt, MYSQL *con){
	if(mysql_stmt_close(stmt)){
	  printf("failed while closing the statement\n");
	  printf("%s\n", mysql_stmt_error(stmt));
	  exit(0);
	}

	mysql_close(con);
}

void lock(pthread_mutex_t *mutex){
	if(pthread_mutex_lock(mutex) != 0){
		printf("lock error\n");
	}
}

void unlock(pthread_mutex_t *mutex){
	if(pthread_mutex_unlock(mutex) != 0){
		printf("unlock error\n");
	}
}
