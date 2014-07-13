<?php
// Create connection
$con = @new mysqli("localhost","test","123456","test");

// Check connection
if ($con->connect_errno){
	die('Connect Error: ' . $con->connect_error);
}

$result = $con->query("SELECT * FROM test.news");

echo $result->num_rows;
echo "<br/>";

while($row = $result->fetch_array()){
	echo $row['Title']; echo "<br>";
	echo $row['PubTime']; echo "<br>";
// 	echo $row['Content']; echo "<br>";
	echo "<br/>";
}
$result->free();

echo "another query:";
echo "<br/>";

$result = $con->query("SELECT * FROM test.mach_news");
/* numeric array */
$row = $result->fetch_array(MYSQLI_NUM);
printf ("%s (%s)<br/>", $row[1], $row[3]);

/* associative array */
$row = $result->fetch_array(MYSQLI_ASSOC);
printf ("%s (%s)<br/>", $row["Title"], $row["PubTime"]);

/* associative and numeric array */
$row = $result->fetch_array(MYSQLI_BOTH);
printf ("%s (%s)<br/>", $row[1], $row["PubTime"]);

$finfo = mysqli_fetch_fields($result);
foreach($finfo as $val){
	printf("Name:     %s\n", $val->name);
	printf("Table:    %s\n", $val->table);
	printf("max. Len: %d\n", $val->max_length);
	printf("Flags:    %d\n", $val->flags);
    printf("Type:     %d\n\n", $val->type);
	echo "<br/>";
}
$result->free();
$con->close();
?>
