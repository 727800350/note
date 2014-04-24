# Example 
## [An simple example](http://www.cyberciti.biz/tips/linux-unix-connect-mysql-c-api-program.html)

	#include <mysql.h>
	#include <stdio.h>
	#include <stdlib.h>

	int main(void) {
	   MYSQL *conn;
	   MYSQL_RES *res;
	   MYSQL_ROW row;
	  /* Change me */
	   char *server = "localhost";
	   char *user = "";
	   char *password = "";
	   char *database = "test";
	   
	   conn = mysql_init(NULL);
	   
	   /* Connect to database */
	   if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
	      fprintf(stderr, "%s\n", mysql_error(conn));
	      exit(1);
	   }
	
	   /* send SQL query */
	   if (mysql_query(conn, "show tables")) {
	      fprintf(stderr, "%s\n", mysql_error(conn));
	      exit(1);
	   }
	
	   res = mysql_use_result(conn);
	   
	   /* output table name */
	   printf("MySQL Tables in mysql database:\n");
	   while ((row = mysql_fetch_row(res)) != NULL)
	      printf("%s \n", row[0]);
	
	   /* close connection */
	   mysql_free_result(res);
	   mysql_close(conn);
	   return 0;
	}

## [statement example](https://dev.mysql.com/doc/refman/5.0/en/mysql-stmt-execute.html)

	#include <stdio.h>
	#include <stdlib.h>
	#include <mysql.h>
	#include <string.h>
	
	#define STRING_SIZE 50
	
	#define DROP_SAMPLE_TABLE "DROP TABLE IF EXISTS test_table"
	#define CREATE_SAMPLE_TABLE "CREATE TABLE test_table(col1 INT,\
			col2 VARCHAR(40),\
	col3 SMALLINT,\
	col4 TIMESTAMP)"
	#define INSERT_SAMPLE "INSERT INTO \
			test_table(col1,col2,col3) \
	VALUES(?,?,?)"
	
	#define EXIST "select * from test_table where col1=?"
	
	int main() {
		MYSQL_STMT    *stmt;
		MYSQL_BIND    bind[3];
		my_ulonglong  affected_rows;
		int           param_count;
		short         small_data;
		int           int_data;
		char          str_data[STRING_SIZE];
		unsigned long str_length;
		my_bool       is_null;
		MYSQL *mysql;
		char *server = "localhost";
		char *user = "";
		char *password = "";
		char *database = "test";
		mysql = mysql_init(NULL);
	
		/* Connect to database */
		if (!mysql_real_connect(mysql, server,
								user, password, database, 0, NULL, 0)) {
			fprintf(stderr, "%s\n", mysql_error(mysql));
			exit(1);
		}
	
		if (mysql_query(mysql, DROP_SAMPLE_TABLE)) {
			fprintf(stderr, " DROP TABLE failed\n");
			fprintf(stderr, " %s\n", mysql_error(mysql));
			exit(0);
		}
	
		if (mysql_query(mysql, CREATE_SAMPLE_TABLE)) {
			fprintf(stderr, " CREATE TABLE failed\n");
			fprintf(stderr, " %s\n", mysql_error(mysql));
			exit(0);
		}
	
		/* Prepare an INSERT query with 3 parameters */
		/* (the TIMESTAMP column is not named; the server */
		/*  sets it to the current date and time) */
		stmt = mysql_stmt_init(mysql);
	
		if (!stmt) {
			fprintf(stderr, " mysql_stmt_init(), out of memory\n");
			exit(0);
		}
	
		if (mysql_stmt_prepare(stmt, INSERT_SAMPLE, strlen(INSERT_SAMPLE))) {
			fprintf(stderr, " mysql_stmt_prepare(), INSERT failed\n");
			fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
			exit(0);
		}
	
		fprintf(stdout, " prepare, INSERT successful\n");
	
		/* Get the parameter count from the statement */
		param_count = mysql_stmt_param_count(stmt);
		fprintf(stdout, " total parameters in INSERT: %d\n", param_count);
	
		if (param_count != 3) { /* validate parameter count */
			fprintf(stderr, " invalid parameter count returned by MySQL\n");
			exit(0);
		}
	
		/* Bind the data for all 3 parameters */
		memset(bind, 0, sizeof(bind));
	
		/* INTEGER PARAM */
		/* This is a number type, so there is no need
		   to specify buffer_length */
		bind[0].buffer_type = MYSQL_TYPE_LONG;
		bind[0].buffer = (char *) &int_data;
		bind[0].is_null = 0;
		bind[0].length = 0;
	
		/* STRING PARAM */
		bind[1].buffer_type = MYSQL_TYPE_STRING;
		bind[1].buffer = (char *)str_data;
		bind[1].buffer_length = STRING_SIZE;
		bind[1].is_null = 0;
		bind[1].length = &str_length;
	
		/* SMALLINT PARAM */
		bind[2].buffer_type = MYSQL_TYPE_SHORT;
		bind[2].buffer = (char *)&small_data;
		bind[2].is_null = &is_null;
		bind[2].length = 0;
	
		/* Bind the buffers */
		if (mysql_stmt_bind_param(stmt, bind)) {
			fprintf(stderr, " mysql_stmt_bind_param() failed\n");
			fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
			exit(0);
		}
	
		/* Specify the data values for the first row */
		int_data = 10;            /* integer */
		strncpy(str_data, "MySQL", STRING_SIZE); /* string  */
		str_length = strlen(str_data);
		/* INSERT SMALLINT data as NULL */
		is_null = 1;
	
		/* Execute the INSERT statement - 1*/
		if (mysql_stmt_execute(stmt)) {
			fprintf(stderr, " mysql_stmt_execute(), 1 failed\n");
			fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
			exit(0);
		}
	
		/* Get the number of affected rows */
		affected_rows = mysql_stmt_affected_rows(stmt);
		fprintf(stdout, " total affected rows(insert 1): %lu\n",
				(unsigned long) affected_rows);
	
		if (affected_rows != 1) { /* validate affected rows */
			fprintf(stderr, " invalid affected rows by MySQL\n");
			exit(0);
		}
	
		/* Specify data values for second row,
		   then re-execute the statement */
		int_data = 1000;
		strncpy(str_data, "The most popular Open Source database", STRING_SIZE);
		str_length = strlen(str_data);
		small_data = 1000;        /* smallint */
		is_null = 0;              /* reset */
	
		/* Execute the INSERT statement - 2*/
		if (mysql_stmt_execute(stmt)) {
			fprintf(stderr, " mysql_stmt_execute, 2 failed\n");
			fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
			exit(0);
		}
	
		/* Get the total rows affected */
		affected_rows = mysql_stmt_affected_rows(stmt);
		fprintf(stdout, " total affected rows(insert 2): %lu\n",
				(unsigned long) affected_rows);
	
		if (affected_rows != 1) { /* validate affected rows */
			fprintf(stderr, " invalid affected rows by MySQL\n");
			exit(0);
		}
	
		/* Close the statement */
		if (mysql_stmt_close(stmt)) {
			fprintf(stderr, " failed while closing the statement\n");
			fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
			exit(0);
		}
	
		mysql_close(mysql);
		return 0;
	}

## Excute 

多个文件时

    gcc $(mysql_config --cflags) $(mysql_config --libs) -c db.c  
    // 只是-c 时, 可以不用后面的$(mysql_config --libs). -o 的时候需要  
    gcc $(mysql_config --cflags) -c db.c  
    gcc $(mysql_config --cflags) $(mysql_config --libs) -c test.c  
    gcc $(mysql_config --cflags) $(mysql_config --libs) -o test db.o test.o 

**How do I compile and link program against MySQL libs?**

MySQL comes with a special script called `mysql_config`.  
It provides you with useful information for compiling your MySQL client and connecting it to MySQL database server. You need to **use following two options**.

Pass `--libs` option - Libraries and options required to link with the MySQL client library.

`$ mysql_config --libs`

Output:

`-L/usr/lib64/mysql -lmysqlclient -lz -lcrypt -lnsl -lm -L/usr/lib64 -lssl -lcrypto`

Pass `--cflags` option - Compiler flags to find include files and critical compiler flags and defines used when compiling the libmysqlclient library.

`$ mysql_config --cflags`

Output:

`-I/usr/include/mysql -g -pipe -m64 -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -fno-strict-aliasing`

You need to pass above option to GNU C compiler i.e. gcc. So to compile above program, enter:

`$ gcc -o output-file $(mysql_config --cflags) mysql-c-api.c $(mysql_config --libs)`

Now execute program:
`$ ./output-file`

#C API
## Database and table
    mysql_query(conn, "CREATE DATABASE ...")
    mysql_query(conn, "use ...")
    
    
## Collection
	MYSQL *mysql_init(MYSQL *mysql)

    /* Connect to database */
    MYSQL *mysql_real_connect(MYSQL *mysql, 
	const char *host, const char *user, const char *passwd, const char *db, 
	unsigned int port, const char *unix_socket, unsigned long client_flag)
    
	Return A MYSQL* connection if successful, NULL if unsuccessful. 
	
	port: If not 0, used as the port number for the TCP/IP connection  
	unix_socket: if not NULL, specifies the socket or named pipe to use.  
	client_flag: usually 0  
    
	/* Close the connection */
    void mysql_close(conn);
## Query and update
    /* send SQL query */
    int mysql_query(MYSQL *mysql, const char *stmt_str)
    return Zero for success
	stmt_str is without a terminating semicolon (";") or \g

    mysql_query(conn, "insert into ... values(...)")

## Result
    MYSQL_RES *mysql_use_result(MYSQL *mysql)
    MYSQL_RES *mysql_store_result(MYSQL *mysql)
 	void mysql_free_result(res);

	MYSQL_ROW mysql_fetch_row(MYSQL_RES *result)
    A MYSQL_ROW for the next row. NULL if no more rows to retrieve or if an error 

After invoking `mysql_query()` or `mysql_real_query()`, you **must call `mysql_store_result()` or `mysql_use_result()`** for every statement that successfully produces a result set (`SELECT, SHOW, DESCRIBE, EXPLAIN, CHECK TABLE`, and so forth). You **must also call `mysql_free_result()`** after you are done with the result set.

`mysql_use_result()` initiates a result set retrieval but does not actually read the result set into the client like `mysql_store_result()` does.  
Instead, each row must be retrieved individually by calling `mysql_fetch_row()`.  
This reads the result of a query directly from the server without storing it in a temporary table or local buffer, which is somewhat faster and uses much less memory than `mysql_store_result()`.   
The client allocates memory only for the current row and a communication buffer that may grow up to max_allowed_packet bytes.

## Transaction
	mysql_autocommit(conn, 0); 
	//Do stuff here
	mysql_commit(conn); //...or mysql_rollback(conn);

I would rather use the "query" method for all these calls:

	mysql_query(conn, "START TRANSACTION");
	//Do stuff here
	mysql_query(conn, "COMMIT"); //...or mysql_query(conn, "ROLLBACK"); 

## Timestamp
timestamp存储的是从Unix 纪元(格林威治时间 1970 年 1 月 1 日 00:00:00)到指定时间的秒数

[实验MySql TIMESTAMP 类型](http://www.xwuxin.com/?p=1642)

[Mysql中datetime和timestamp的相互转换](http://hi.baidu.com/tingli08/item/7151545b5ffeff9909be17ed)

[mysql多个TimeStamp设置](http://www.cnblogs.com/yjf512/archive/2012/11/02/2751058.html)

### Store timestamp into mysql  
`from_unixtime` converts int to datatime type  
`unix_timestamp` convets datetime type to int

	insert into timetest(time_stamp) values(integer); //error
	insert into timetest(time_stamp) values("2014-03-01");
	insert into timetest(time_stamp) values("2014-03-01 09:10:00");
	insert into timetest(time_stamp) values(FROM_UNIXTIME(1111000000));

### Timestamp bind example
	  MYSQL_TIME  ts;
	  MYSQL_BIND  bind[3];
	  MYSQL_STMT  *stmt;
	
	  strmov(query, "INSERT INTO test_table(date_field, time_field, \
	                               timestamp_field) VALUES(?,?,?");
	
	  stmt = mysql_stmt_init(mysql);
	  if (!stmt)
	  {
	    fprintf(stderr, " mysql_stmt_init(), out of memory\n");
	    exit(0);
	  }
	  if (mysql_stmt_prepare(mysql, query, strlen(query)))
	  {
	    fprintf(stderr, "\n mysql_stmt_prepare(), INSERT failed");
	    fprintf(stderr, "\n %s", mysql_stmt_error(stmt));
	    exit(0);
	  }
	
	  /* set up input buffers for all 3 parameters */
	  bind[0].buffer_type= MYSQL_TYPE_DATE;
	  bind[0].buffer= (char *)&ts;
	  bind[0].is_null= 0;
	  bind[0].length= 0;
	  ...
	  bind[1]= bind[2]= bind[0];
	  ...
	
	  mysql_stmt_bind_param(stmt, bind);
	
	  /* supply the data to be sent in the ts structure */
	  ts.year= 2002;
	  ts.month= 02;
	  ts.day= 03;
	
	  ts.hour= 10;
	  ts.minute= 45;
	  ts.second= 20;
	
	  mysql_stmt_execute(stmt);
	  ..
  
[struct tm 的结构](http://man7.org/linux/man-pages/man3/ctime.3.html)

     struct tm {
               int tm_sec;    /* Seconds (0-60) */
               int tm_min;    /* Minutes (0-59) */
               int tm_hour;   /* Hours (0-23) */
               int tm_mday;   /* Day of the month (1-31) */
               int tm_mon;    /* Month (0-11) */
               int tm_year;   /* Year - 1900 */
               int tm_wday;   /* Day of the week (0-6, Sunday = 0) */
               int tm_yday;   /* Day in the year (0-365, 1 Jan = 0) */
               int tm_isdst;  /* Daylight saving time */
           };

    char *ctime(const time_t *timep);
    char *ctime_r(const time_t *timep, char *buf);

    struct tm *gmtime(const time_t *timep);
    struct tm *gmtime_r(const time_t *timep, struct tm *result);