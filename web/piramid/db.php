<?php

$host = 'localhost';
$db = 'kctf_web_piramid';
$user = 'root';
$pass = '';

$dsn = "mysql:host=$host;dbname=$db;";
$opt = array(
    PDO::ATTR_ERRMODE            => PDO::ERRMODE_EXCEPTION,
    PDO::ATTR_DEFAULT_FETCH_MODE => PDO::FETCH_ASSOC
);
$pdo = new PDO($dsn, $user, $pass, $opt);

