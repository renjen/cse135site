<?php
header("Cache-Control: no-cache");
header("Content-Type: text/html");

$name = $_COOKIE["state_name"] ?? "(not set)";
$favorite = $_COOKIE["state_favorite"] ?? "(not set)";
?>
<!DOCTYPE html>
<html>
<head>
  <title>State Demo (PHP) - View</title>
</head>
<body>
  <h1>State Demo (PHP) - View</h1>
  <hr/>

  <p><b>Saved Name:</b> <?php echo htmlspecialchars($name); ?></p>
  <p><b>Saved Favorite:</b> <?php echo htmlspecialchars($favorite); ?></p>

  <p>
    <a href="state-php-set.php">Back to Set Page</a>
  </p>

  <form method="POST" action="state-php-clear.php">
    <button type="submit">Clear State</button>
  </form>
</body>
</html>
