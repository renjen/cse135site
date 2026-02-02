<?php
header("Cache-Control: no-cache");

// Expire cookies
setcookie("state_name", "", time() - 3600, "/");
setcookie("state_favorite", "", time() - 3600, "/");

// Go back to view page
header("Location: state-php-view.php");
exit();
