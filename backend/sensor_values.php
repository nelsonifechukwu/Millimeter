<?php
$user_id = $_SESSION['user_id'];
require_once('connections/connections.php');

$records = "SELECT * FROM `electricity_readings` WHERE `user_id` = '$user_id' ORDER BY `id` DESC limit 40";
$get_records = mysqli_query($con, $records);

if($get_records)
{
	while($param = mysqli_fetch_assoc($get_records))
	{
		$sensor_data[] = $param;
	}
}


$readings_time = array_column($sensor_data, 'created_at');

// ******* Uncomment to convert readings time array to your timezone ********
/*$i = 0;
foreach ($readings_time as $reading){
    // Uncomment to set timezone to - 1 hour (you can change 1 to any number)
    $readings_time[$i] = date("Y-m-d H:i:s", strtotime("$reading - 1 hours"));
    // Uncomment to set timezone to + 4 hours (you can change 4 to any number)
    //$readings_time[$i] = date("Y-m-d H:i:s", strtotime("$reading + 4 hours"));
    $i += 1;
}*/
$voltage_array = array_reverse(array_column($sensor_data, 'voltage'));
$voltage = json_encode(array_reverse(array_column($sensor_data, 'voltage')), JSON_NUMERIC_CHECK);
$reading_time = json_encode(array_reverse($readings_time), JSON_NUMERIC_CHECK);

$voltage_used = array_sum($voltage_array);
$total_voltage = 987;
$voltage_left = $total_voltage - $voltage_used;
$voltage_percentage_left = ( ($voltage_left/$total_voltage) * 100);

?>