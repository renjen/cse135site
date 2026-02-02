<?php
header("Cache-Control: no-cache");
header("Content-Type: text/html");

$team = "Renee";
$date = date("D M d H:i:s Y");
$ip = $_SERVER["REMOTE_ADDR"] ?? "unknown";
?>
<!DOCTYPE html>
<html>
<head>
  <title>Hello PHP</title>
</head>
<body>
  <h1>Hello <?php echo $team; ?></h1>
  <hr/>
  <p>This page was generated with the PHP programming language</p>
  <p>This program was generated at: <?php echo $date; ?></p>
  <p>Your current IP Address is: <?php echo $ip; ?></p>
</body>
</html>
