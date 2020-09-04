<?php
session_start();
if(!$_SESSION){
  echo "<script type='text/javascript'>alert('You are not logged in')</script>";
  header('location: ./index.php');
}
?>