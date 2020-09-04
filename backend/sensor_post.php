<?php

require_once('connections/connections.php');
$api_key = "tPmAT5Ab3j7F9";

if ($_SERVER["REQUEST_METHOD"] == "POST"){

if(isset($_POST['api_key']))
{

if($api_key == strip_tags(mysqli_real_escape_string($con,$_POST['api_key']))) 
{

	$voltage = strip_tags(mysqli_real_escape_string($con, $_POST['voltage']));
	$user_id = 1;
	$created_at = date('Y-m-d H:i:s');

	$parameters = "INSERT INTO `electricity_readings` (`voltage`,`created_at`, `user_id`) VALUES('$voltage', '$created_at', '$user_id')";

	$save_parameters = mysqli_query($con, $parameters);

	if($save_parameters){

		echo "Sensor values have been saved";

	}else
	{
		echo mysqli_error($con);
	}		

}else{

	echo "Wrong API Key sent.";
}

}else{
echo "No API Key seen.";
}




}


?>