<?php
// Create connection
$con=mysqli_connect("localhost","test","123456","test");

// Check connection
if (mysqli_connect_errno()) {
  echo "Failed to connect to MySQL: " . mysqli_connect_error();
}

$result = mysqli_query($con,"SELECT * FROM test.news");

while($row = mysqli_fetch_array($result)) {
//   echo $row['Title'] . " " . $row['PubTime'];
	echo $row['Title']; echo "<br>";
	echo $row['PubTime']; echo "<br>";
	echo $row['Content']; echo "<br>";
	echo "<br/>";
}
mysqli_free_result($result);
mysqli_close($con);
?>
