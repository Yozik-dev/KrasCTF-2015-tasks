<?php 

function incrementReferer($login){
	global $pdo;
	$stmt = $pdo->prepare('SELECT COUNT(*) as count FROM users WHERE login = ?');
	$stmt->execute([$login]);
	$count = $stmt->fetch()['count'];
	if(!$count)
		return false;
	$stmt = $pdo->prepare('UPDATE users SET referers=referers+1 WHERE lower(login)=lower(?)');
	$stmt->execute([$login]);
}