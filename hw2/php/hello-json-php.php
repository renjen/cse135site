<?php
header("Cache-Control: no-cache");
header("Content-Type: application/json");

$data = [
  "message" => "Hello Renee",
  "language" => "PHP",
  "generated_at" => date("D M d H:i:s Y"),
  "ip" => $_SERVER["REMOTE_ADDR"] ?? "unknown"
];

echo json_encode($data, JSON_PRETTY_PRINT);
?>
