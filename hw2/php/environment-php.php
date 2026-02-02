<?php
header("Cache-Control: no-cache");
header("Content-Type: text/plain");

foreach ($_SERVER as $key => $value) {
    echo $key . "=" . $value . "\n";
}
?>
