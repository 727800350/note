// Created by Eric Wang 2014.04.10
// Database manipulations

#ifndef _DB_H
#define _DB_H

#include <mysql.h>
#include <pthread.h>

#define CONFIG_FILE "/etc/softflowd.json"

typedef struct db_parameter{
	unsigned int port;
	char *host;
	char *db;
	char *user;
	char *passwd;
}db_t;

extern db_t para;

// init the db parameter
void init_db_parameter(db_t *db_para);

// connect the database
MYSQL *db_connect(MYSQL *con, db_t *db_para);

// database statement
MYSQL_STMT *db_stmt_prepare(MYSQL_STMT *stmt, MYSQL *con, const char *sql, const int para_num);
void db_stmt_bind_para(MYSQL_STMT *stmt, MYSQL_BIND *binds);
void db_execute(MYSQL_STMT *stmt);

// query
int db_query(MYSQL *conn, const char *stmt_str);

// close the statement and connection
void db_close(MYSQL_STMT *stmt, MYSQL *conn);

// mutex lock and unlock
void lock(pthread_mutex_t *mutex);
void unlock(pthread_mutex_t *mutex);

#endif
