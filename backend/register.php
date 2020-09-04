<?php

require_once('connections/connections.php');

/**

HASH THE PASWORDS
HASH THE PASWORDS
HASH THE PASWORDS
HASH THE PASWORDS
HASH THE PASWORDS
HASH THE PASWORDS

*/

 if(isset($_POST['register'])){

 	$name = strip_tags(mysqli_real_escape_string($con, $_POST['name']));
 	$email =strip_tags(mysqli_real_escape_string($con, $_POST['email']));
 	$password = strip_tags(mysqli_real_escape_string($con, $_POST['password']));
 	$password_confirmation = strip_tags(mysqli_real_escape_string($con, $_POST['password_confirmation']));

 	//check that passwords match
 	if($password != $password_confirmation){

 		echo "<script type='text/javascript'>alert('Your passwords don\'t match')</script>";

 	}else{
 		//check that email is not taken
 		$check_email = $check_user = "SELECT * FROM `users` WHERE `email` = '$email'";
 		$query_email = mysqli_query($con, $check_email);

 		//if user exists		
 		if(mysqli_num_rows($query_email) >= '1'){

 			echo "<script type='text/javascript'>alert('Email has already been taken!')</script>";

 		}else{

 			$insert_user = "INSERT INTO `users` (`name`, `email`, `password`) VALUES ('$name', '$email', '$password')";

 			$save_user = mysqli_query($con, $insert_user);

 			if($save_user){

 				echo "<script type='text/javascript'>alert('Registration Successful! Login now!')</script>";
 			}

 		}
 	}

 }

 ?>