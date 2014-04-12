# Example 
[An simple example](http://www.cyberciti.biz/tips/linux-unix-connect-mysql-c-api-program.html)

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
	stmt_str is without a terminating semicolon (“;”) or \g

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
  
# MySQL
## Storage
`MyISAM` is the default storage engine. It is based on the older (and no longer available) ISAM storage engine but **has many useful extensions**.  
Each MyISAM table is stored on disk in **three files**. The files have names that begin with the table name and have an extension to indicate the file type.  
An `.frm` file stores the table format. The data file has an `.MYD (MYData)` extension. The index file has an `.MYI (MYIndex)` extension.  
To specify explicitly that you want a MyISAM table, indicate that with an ENGINE table option:

    CREATE TABLE t (i INT) ENGINE = MYISAM;
老版本的MySQL使用TYPE而不是ENGINE(例如，TYPE = MYISAM), but ENGINE is the preferred term and TYPE is deprecated.


