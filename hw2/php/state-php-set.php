<?php
header("Cache-Control: no-cache");
header("Content-Type: text/html");

if ($_SERVER["REQUEST_METHOD"] === "POST") {
    $name = $_POST["name"] ?? "";
    $favorite = $_POST["favorite"] ?? "";

    // store in cookies (simple server-side state mechanism for this demo)
    setcookie("state_name", $name, time() + 3600, "/");
    setcookie("state_favorite", $favorite, time() + 3600, "/");

    // redirect to view page after saving
    header("Location: state-php-view.php");
    exit();
}
?>
<!DOCTYPE html>
<html>
<head>
  <title>State Demo (PHP) - Set</title>
</head>
<body>
  <h1>State Demo (PHP) - Set</h1>
  <hr/>
  <p>Enter values to save (cookie-based state):</p>

  <form method="POST" action="state-php-set.php">
    <p>
      <label>Name:
        <input type="text" name="name" />
      </label>
    </p>
    <p>
      <label>Favorite:
        <input type="text" name="favorite" />
      </label>
    </p>
    <button type="submit">Save State</button>
  </form>

  <p><a href="state-php-view.php">Go to View State</a></p>
</body>
</html>
