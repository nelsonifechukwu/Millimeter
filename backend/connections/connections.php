<?php

$servername = "localhost";
$username = "symmetri_user";
$password = "jerrymanlim.";
$database = "symmetri_millimeter"; 


/*
$servername = "localhost";
$username = "root";
$password = "";
$database = "millimiter"; 
*/

 
// Create connection
$con = mysqli_connect($servername, $username, $password, $database);

// Check connection
if (!$con) {

  die("Connection failed: " . mysqli_connect_error());

}


?>