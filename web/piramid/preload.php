<?php

include 'db.php';

session_start();

$adminLogin = "superstatos";
$thisIsFlag = "#thisIsCtfFlag982314";

if(isset($_GET['referer'])){
	setcookie('referer', $_GET['referer']);
}

if(isset($_SESSION['login'])){
	$stmt = $pdo->prepare('SELECT * FROM users WHERE login = ?');
	$stmt->execute([$_SESSION['login']]);
	$user = $stmt->fetch();
	if($user)
		$_SESSION['referers'] = $user['referers'];
}
