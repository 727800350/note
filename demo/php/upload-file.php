The "upload.html" file contains the user interface for uploading a file:

ref: http://blog.snsgou.com/post-298.html

<html>
<body>
<form action="upload_file.php" method="post" enctype="multipart/form-data">
	<label for="file">Filename:</label>
	<input type="file" name="file" id="file"><br>
	<input type="submit" name="submit" value="Submit">
</form>
</body>
</html>

The "upload_file.php" file contains the code for uploading a file:

<?php
// the user apache muse have the permission to write the directory 'UPLOADPATH'
define('UPLOADPATH', '/home/eric/git/tool/softflowd/analyse/');

if($_FILES["file"]["error"] > 0){
	echo "Error: " . $_FILES["file"]["error"] . "<br/>";
}
else{
	echo "Upload: " . $_FILES["file"]["name"] . "<br/>";
	echo "Type: " . $_FILES["file"]["type"] . "<br/>";
	echo "Size: " . ($_FILES["file"]["size"] / 1024) . " kB<br/>";
	echo "Stored in: " . $_FILES["file"]["tmp_name"] . "<br/>";
	$target = UPLOADPATH.$_FILES["attach_file"]["name"];
	echo "target:" . $target . "<br/>";
	if (file_exists($target)){
		echo "<font color = 'red'>Target already exists.</font>";
	}
	else{
		// store the file
		move_uploaded_file($_FILES["file"]["tmp_name"], $target);
		echo "Stored in: " . $target;
	}
}
?>

