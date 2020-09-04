<?php
require_once('connections/connections.php');
session_start();
/*
CREATE SESSIONS AFTER LOGIN
CREATE SESSIONS AFTER LOGIN
CREATE SESSIONS AFTER LOGIN
CREATE SESSIONS AFTER LOGIN
CREATE SESSIONS AFTER LOGIN

*/

	if(isset($_POST['login'])){
		echo "<script type='text/javascript'>alert('Gotcha')</script>";

		$email = strip_tags(mysqli_real_escape_string($con, $_POST['email']));
		$password = strip_tags(mysqli_real_escape_string($con, $_POST['password']));

		$check_user = "SELECT * FROM `users` WHERE `email` = '$email' AND `password` = '$password'";
		$query_user = mysqli_query($con, $check_user);

		$is_user = mysqli_num_rows($query_user);

		if(mysqli_num_rows($query_user) >= '1'){//if user exists

			while($user = mysqli_fetch_assoc($query_user)){

				//redirect admin to dashboard
				header('location: ./dashboard.php');
				$_SESSION['user_id'] = $user['id'];
				$_SESSION['name'] = $user['name'];
			}
		}else{

			echo "<script type='text/javascript'>alert('Incorrect username or password')</script>";
		}

		}



?>