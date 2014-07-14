Create an Upload-File Form

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
if($_FILES["file"]["error"] > 0){
	echo "Error: " . $_FILES["file"]["error"] . "<br>";
}
else{
	echo "Upload: " . $_FILES["file"]["name"] . "<br>";
	echo "Type: " . $_FILES["file"]["type"] . "<br>";
	echo "Size: " . ($_FILES["file"]["size"] / 1024) . " kB<br>";
	echo "Stored in: " . $_FILES["file"]["tmp_name"];
	if(file_exists("upload/" . $_FILES["file"]["name"])){
		 echo $_FILES["file"]["name"] . " already exists. ";
	}
	else{
		// store the file
		// the user apache muse have the permission to write the directory 'upload'
		move_uploaded_file($_FILES["file"]["tmp_name"], "upload/" . $_FILES["file"]["name"]);
		echo "Stored in: " . "upload/" . $_FILES["file"]["name"];
	}
}
?>

