<?php
header("Cache-Control: no-cache");
header("Content-Type: text/html");

$method = $_SERVER["REQUEST_METHOD"];
$contentType = $_SERVER["CONTENT_TYPE"] ?? "";
$rawData = "";

if (in_array($method, ["POST", "PUT", "DELETE"])) {
    $rawData = file_get_contents("php://input");
} else {
    $rawData = $_SERVER["QUERY_STRING"] ?? "";
}

$params = [];

if (str_contains($contentType, "application/json")) {
    $params = json_decode($rawData, true) ?? [];
} else {
    parse_str($rawData, $params);
}

$ip = $_SERVER["REMOTE_ADDR"] ?? "unknown";
$agent = $_SERVER["HTTP_USER_AGENT"] ?? "unknown";
$date = date("D M d H:i:s Y");

?>
<!DOCTYPE html>
<html>
<head>
  <title>Echo PHP</title>
</head>
<body>
<h1>Echo (PHP)</h1><hr/>
<p><b>Method:</b> <?php echo $method; ?></p>
<p><b>Content-Type:</b> <?php echo $contentType; ?></p>
<p><b>Generated at:</b> <?php echo $date; ?></p>
<p><b>Your IP:</b> <?php echo $ip; ?></p>
<p><b>User Agent:</b> <?php echo $agent; ?></p>

<h2>Parameters</h2>
<ul>
<?php
foreach ($params as $key => $value) {
    if (is_array($value)) {
        foreach ($value as $v) {
            echo "<li>$key = $v</li>";
        }
    } else {
        echo "<li>$key = $value</li>";
    }
}
?>
</ul>
</body>
</html>
