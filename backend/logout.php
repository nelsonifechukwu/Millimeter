<?php
session_start();

  	unset($_SESSION['user_id']);
  	unset($_SESSION['name']);
  	if(session_destroy()){
  	header("location: ../index.php");
  	}
 
?>