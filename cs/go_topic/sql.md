# [Retrieving Result Sets](http://go-database-sql.org/retrieving.html)
the sql.DB object is designed to be long-lived. Don’t Open() and Close() databases frequently. Instead, create one
sql.DB object for each distinct datastore you need to access, and keep it until the program is done accessing that
datastore. Pass it around as needed, or make it available somehow globally, but keep it open. 

If a function name includes Query, it is designed to ask a question of the database, and will return a set of rows,
even if it’s empty. Statements that don’t return rows should not use Query functions; they should use Exec().

## Fetching Data from the Database
```go
var (
	id int
	name string
)
rows, err := db.Query("select id, name from users where id = ?", 1)
if err != nil {
	log.Fatal(err)
}
defer rows.Close()
for rows.Next() {
	err := rows.Scan(&id, &name)
	if err != nil {
		log.Fatal(err)
	}
	log.Println(id, name)
}
err = rows.Err()
if err != nil {
	log.Fatal(err)
}
```

- You should always check for an error at the end of the for rows.Next() loop. If there’s an error during the loop, you
  need to know about it. Don’t just assume that the loop iterates until you’ve processed all the rows.
- as long as there’s an open result set (represented by rows), the underlying connection is busy and can’t be used for
  any other query. That means it’s not available in the connection pool. If you iterate over all of the rows with
  rows.Next(), eventually you’ll read the last row, and rows.Next() will encounter an internal EOF error and call
  rows.Close() for you. But if for some reason you exit that loop – an early return, or so on – then the rows doesn’t
  get closed, and the connection remains open. (It is auto-closed if rows.Next() returns false due to an error, though).
  This is an easy way to run out of resources.
- rows.Close() is a harmless no-op if it’s already closed, so you can call it multiple times.

## How Scan() Works
When you iterate over rows and scan them into destination variables, Go performs data type conversions work for you,
behind the scenes. It is based on the type of the destination variable. Being aware of this can clean up your code and
help avoid repetitive work.

# Statements
[Using Prepared Statements](http://go-database-sql.org/prepared.html)

## Prepared Statements And Connections
At the database level, a prepared statement is bound to a single database connection. The typical flow is that the
client sends a SQL statement with placeholders to the server for preparation, the server responds with a statement ID,
and then the client executes the statement by sending its ID and parameters.

In Go, however, connections are not exposed directly to the user of the database/sql package. You don’t prepare a
statement on a connection. You prepare it on a DB or a Tx. And database/sql has some convenience behaviors such as
automatic retries. For these reasons, the underlying association between prepared statements and connections, which
exists at the driver level, is hidden from your code.

Here’s how it works:

1. When you prepare a statement, it’s prepared on a connection in the pool.
1. The Stmt object remembers which connection was used.
1. When you execute the Stmt, it tries to use the connection. If it’s not available because it’s closed or busy doing
  something else, it gets another connection from the pool and re-prepares the statement with the database on another
  connection.

Because statements will be re-prepared as needed when their original connection is busy, it’s possible for
high-concurrency usage of the database, which may keep a lot of connections busy, to create a large number of prepared
statements. This can result in apparent leaks of statements, statements being prepared and re-prepared more often than
you think, and even running into server-side limits on the number of statements.

## Avoiding Prepared Statements
Go creates prepared statements for you under the covers. A simple `db.Query(sql, param1, param2)`, for example, works by
preparing the sql, then executing it with the parameters and finally closing the statement.

Sometimes a prepared statement is not what you want.

If you don’t want to use a prepared statement, you need to use fmt.Sprint() or similar to assemble the SQL, and pass
this as the only argument to db.Query() or db.QueryRow(). And your driver needs to support plaintext query execution,
which is added in Go 1.1 via the Execer and Queryer interfaces.

## [Multiple Statement Support](http://go-database-sql.org/surprises.html)
The database/sql doesn’t explicitly have multiple statement support, which means that the behavior of this is backend
dependent:
```go
_, err := db.Exec("DELETE FROM tbl1; DELETE FROM tbl2") // Error/unpredictable result
```
The server is allowed to interpret this however it wants, which can include returning an error, executing only the first
statement, or executing both.

Similarly, there is no way to batch statements in a transaction. Each statement in a transaction must be executed
serially, and the resources in the results, such as a Row or Rows, must be scanned or closed so the underlying
connection is free for the next statement to use. This differs from the usual behavior when you’re not working with a
transaction. In that scenario, it is perfectly possible to execute a query, loop over the rows, and within the loop
make a query to the database (which will happen on a new connection):
```go
rows, err := db.Query("select * from tbl1") // Uses connection 1
for rows.Next() {
	err = rows.Scan(&myvariable)
	// The following line will NOT use connection 1, which is already in-use
	db.Query("select * from tbl2 where id = ?", myvariable)
}
```
But transactions are bound to just one connection, so this isn’t possible with a transaction:

```go
tx, err := db.Begin()
rows, err := tx.Query("select * from tbl1") // Uses tx's connection
for rows.Next() {
	err = rows.Scan(&myvariable)
	// ERROR! tx's connection is already busy!
	tx.Query("select * from tbl2 where id = ?", myvariable)
}
```
Go doesn’t stop you from trying, though. For that reason, you may wind up with a corrupted connection if you attempt to
perform another statement before the first has released its resources and cleaned up after itself. This also means that
each statement in a transaction results in a separate set of network round-trips to the database.

# [Working with NULLs](http://go-database-sql.org/nulls.html)
Nullable columns are annoying and lead to a lot of ugly code. If you can, avoid them.

If you can’t avoid having NULL values in your database, there is another work around that most database systems support,
namely COALESCE(). Something like the following might be something that you can use, without introducing a myriad of
`sql.Null*` types.
```go
rows, err := db.Query(`
	SELECT
		name,
		COALESCE(other_field, '') as otherField
	WHERE id = ?
`, 42)

for rows.Next() {
	err := rows.Scan(&name, &otherField)
	// ..
	// If `other_field` was NULL, `otherField` is now an empty string. This works with other data types as well.
}
```

# The Connection Pool
You can also specify the maximum amount of time a connection may be reused by setting `db.SetConnMaxLifetime(duration)`
since reusing long lived connections may cause network issues. This closes the unused connections lazily i.e. closing
expired connection may be deferred.


